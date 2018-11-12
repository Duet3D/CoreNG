/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Wire.h"

#if SAME70
// TWI disabled for now
#else

#if SAME70
#include "twihs/twihs.h"
#else
#include "twi/twi.h"
#endif

#include <cstring>

void TwoWireErrors::Clear()
{
	naks = sendTimeouts = recvTimeouts = finishTimeouts = 0;
}

// Wait for a status bit or NAK to be set, returning true if successful and it wasn't NAK
bool TwoWire::WaitForStatus(uint32_t statusBit, uint32_t timeout, uint32_t& timeoutErrorCounter)
{
	const uint32_t startClocks = SysTick->VAL & 0x00FFFFFF;
	const uint32_t loadVal = (SysTick->LOAD & 0x00FFFFFF) + 1;
	timeout *= VARIANT_MCK/1000000;
	uint32_t sr;
	do
	{
		sr = twi->TWI_SR & (TWI_SR_NACK | statusBit);
	} while (sr == 0 && ((startClocks - (SysTick->VAL & 0x00FFFFFF)) % loadVal) < timeout);

	sr |= twi->TWI_SR;										// in case we were descheduled between checking the status register and checking for timeout
	if ((sr & TWI_SR_NACK) != 0)
	{
		++errorCounts.naks;
		return false;
	}
	if ((sr & statusBit) != 0)
	{
		return true;
	}
	++timeoutErrorCounter;
	return false;
}

inline bool TwoWire::WaitTransferComplete(uint32_t timeout)
{
	return WaitForStatus(TWI_SR_TXCOMP, timeout, errorCounts.finishTimeouts);
}

inline bool TwoWire::WaitByteSent(uint32_t timeout)
{
	return WaitForStatus(TWI_SR_TXRDY, timeout, errorCounts.sendTimeouts);
}

inline bool TwoWire::WaitByteReceived(uint32_t timeout)
{
	return WaitForStatus(TWI_SR_RXRDY, timeout, errorCounts.recvTimeouts);
}

TwoWire::TwoWire(Twi *_twi, void(*_beginCb)(void)) : twi(_twi), onBeginCallback(_beginCb)
{
}

// Begin in master mode
void TwoWire::BeginMaster(uint32_t clockFrequency)
{
	if (onBeginCallback != nullptr)
	{
		onBeginCallback();
	}

	// Disable PDC channel
	twi->TWI_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	twi_options_t opt;
	opt.speed = clockFrequency;
	opt.master_clk = VARIANT_MCK;
	opt.chip = opt.smbus = 0;
	twi_master_init(twi, &opt);

	errorCounts.Clear();
}

// Write then read data
size_t TwoWire::Transfer(uint16_t address, uint8_t *buffer, size_t numToWrite, size_t numToRead, uint32_t timeout)
{
	// If an empty transfer, nothing to do
	if (numToRead + numToWrite == 0)
	{
		return 0;
	}

	// Set up the mode register and address
	if (address >= 0x80)
	{
		// 10-bit address
		const uint32_t topAddress = 0b01111000 | ((address >> 8) & 3);
	    twi->TWI_MMR = (1 << 8) | (topAddress << 16);
	    twi->TWI_IADR = address & 0x00FF;
	}
	else
	{
		// 7-bit address
	    twi->TWI_MMR = (uint32_t)address << 16;
	    twi->TWI_IADR = 0;
	}

	size_t bytesSent = 0;
	if (numToWrite != 0)
	{
		// Send all bytes except the last one.
		// Ideally, if there are bytes to read as well as write, we would not send a STOP after sending all the bytes.
		// Unfortunately, the SAM TWI peripheral doesn't provide any means of reporting when the transmission is complete if we don't send STOP after it.
		while (bytesSent + 1 < numToWrite)
		{
			twi->TWI_THR = *buffer++;
			if (!WaitByteSent(timeout))
			{
				twi->TWI_CR = TWI_CR_STOP;
				(void)WaitTransferComplete(timeout);
				return bytesSent;
			}
			++bytesSent;
		}

		twi->TWI_THR = *buffer++;
		twi->TWI_CR = TWI_CR_STOP;
		if (WaitByteSent(timeout))
		{
			++bytesSent;
		}
		(void)WaitTransferComplete(timeout);
		if (bytesSent < numToWrite || numToRead == 0)
		{
			return bytesSent;
		}
	}

	// There are bytes to read, and if there were any bytes to send then we have sent them all
    twi->TWI_MMR |= TWI_MMR_MREAD;							// change the mode to read
	size_t bytesReceived = 0;
	if (numToRead == 1)
	{
		twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
		if (WaitByteReceived(timeout))
		{
			*buffer = twi->TWI_RHR;
			++bytesReceived;
		}

		(void)WaitTransferComplete(timeout);
		return bytesSent + bytesReceived;
	}

	// Multi-byte read. We must set the STOP flag before we read the penultimate byte from the RHR.
	twi->TWI_CR = TWI_CR_START;
	for (;;)
	{
		if (!WaitByteReceived(timeout))
		{
			twi->TWI_CR = TWI_CR_STOP;						// this may not do any good
			(void)WaitTransferComplete(timeout);		// neither may this
			return bytesSent + bytesReceived;
		}

		++bytesReceived;
		if (bytesReceived + 1 == numToRead)
		{
			break;
		}

		*buffer++ = twi->TWI_RHR;
	}

	// The penultimate byte is in the RHR
	twi->TWI_CR = TWI_CR_STOP;
	*buffer++ = twi->TWI_RHR;
	if (WaitByteReceived(timeout))
	{
		*buffer++ = twi->TWI_RHR;
		++bytesReceived;
	}
	(void)WaitTransferComplete(timeout);
	return bytesSent + bytesReceived;

}

TwoWireErrors TwoWire::GetErrorCounts(bool clear)
{
	const irqflags_t flags = cpu_irq_save();
	const TwoWireErrors ret = errorCounts;
	if (clear)
	{
		errorCounts.Clear();
	}
	cpu_irq_restore(flags);
	return ret;
}

#if WIRE_INTERFACES_COUNT > 0
static void Wire_Init(void)
{
	pmc_enable_periph_clk(WIRE_INTERFACE_ID);
	ConfigurePin(g_APinDescription[APIN_WIRE_SDA]);
	ConfigurePin(g_APinDescription[APIN_WIRE_SCL]);

	NVIC_DisableIRQ(WIRE_ISR_ID);
	NVIC_ClearPendingIRQ(WIRE_ISR_ID);
}

TwoWire Wire = TwoWire(WIRE_INTERFACE, Wire_Init);
#endif

#if WIRE_INTERFACES_COUNT > 1
static void Wire1_Init(void)
{
	pmc_enable_periph_clk(WIRE1_INTERFACE_ID);
	ConfigurePin(g_APinDescription[APIN_WIRE1_SDA]);
	ConfigurePin(g_APinDescription[APIN_WIRE1_SCL]);

	NVIC_DisableIRQ(WIRE1_ISR_ID);
	NVIC_ClearPendingIRQ(WIRE1_ISR_ID);
}

TwoWire Wire1 = TwoWire(WIRE1_INTERFACE, Wire1_Init);
}
#endif

#endif

// End
