/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "WInterrupts.h"

const uint32_t pioInterruptPriority = 5;

typedef void (*interruptCB)(CallbackParameter);

struct InterruptCallback
{
	interruptCB func;
	CallbackParameter param;

	InterruptCallback() : func(nullptr) { }
};

static InterruptCallback callbacksPioA[32];
static InterruptCallback callbacksPioB[32];
static InterruptCallback callbacksPioC[32];
#ifdef ID_PIOD
static InterruptCallback callbacksPioD[32];
#endif
#ifdef ID_PIOE
static InterruptCallback callbacksPioE[32];
#endif

/* Configure PIO interrupt sources */
static void __initialize()
{
	pmc_enable_periph_clk(ID_PIOA);
	NVIC_DisableIRQ(PIOA_IRQn);
	NVIC_ClearPendingIRQ(PIOA_IRQn);
	NVIC_EnableIRQ(PIOA_IRQn);

	pmc_enable_periph_clk(ID_PIOB);
	NVIC_DisableIRQ(PIOB_IRQn);
	NVIC_ClearPendingIRQ(PIOB_IRQn);
	NVIC_EnableIRQ(PIOB_IRQn);

	pmc_enable_periph_clk(ID_PIOC);
	NVIC_DisableIRQ(PIOC_IRQn);
	NVIC_ClearPendingIRQ(PIOC_IRQn);
	NVIC_EnableIRQ(PIOC_IRQn);

#ifdef ID_PIOD
	pmc_enable_periph_clk(ID_PIOD);
	NVIC_DisableIRQ(PIOD_IRQn);
	NVIC_ClearPendingIRQ(PIOD_IRQn);
	NVIC_EnableIRQ(PIOD_IRQn);
#endif

#ifdef ID_PIOE
	pmc_enable_periph_clk(ID_PIOE);
	NVIC_DisableIRQ(PIOE_IRQn);
	NVIC_ClearPendingIRQ(PIOE_IRQn);
	NVIC_EnableIRQ(PIOE_IRQn);
#endif
}

// Get the number of the highest bit that is set in a 32-bit word. Returns 0 if no bit set (same as if lowest bit is set).
// This needs to be fast. Hopefully the ARM conditional instructions will be used to advantage here.
static unsigned int GetHighestBit(uint32_t bits)
{
	unsigned int bitNum = (bits >= 0x00010000) ? 16 : 0;
	if ((bits >> bitNum) >= 0x0100u)
	{
		bitNum += 8;
	}
	if ((bits >> bitNum) >= 0x0010u)
	{
		bitNum += 4;
	}
	if ((bits >> bitNum) >= 0x0004u)
	{
		bitNum += 2;
	}
	if ((bits >> bitNum) >= 0x0002u)
	{
		bitNum += 1;
	}
	return bitNum;
}

// Attach an interrupt to the specified pin returning true if successful
bool attachInterrupt(uint32_t pin, void (*callback)(CallbackParameter), uint32_t mode, CallbackParameter param)
{
	if (pin > MaxPinNumber)
	{
		return false;
	}

	static bool enabled = false;
	if (!enabled)
	{
		__initialize();
		enabled = true;
	}

	// Retrieve pin information
	Pio * const pio = g_APinDescription[pin].pPort;
	const uint32_t mask = g_APinDescription[pin].ulPin;
	const uint32_t pos = GetHighestBit(mask);

	// Set callback function and parameter
	const irqflags_t flags = cpu_irq_save();

	if (pio == PIOA)
	{
		callbacksPioA[pos].func = callback;
		callbacksPioA[pos].param = param;
	}
	else if (pio == PIOB)
	{
		callbacksPioB[pos].func = callback;
		callbacksPioB[pos].param = param;
	}
	else if (pio == PIOC)
	{
		callbacksPioC[pos].func = callback;
		callbacksPioC[pos].param = param;
	}
#ifdef ID_PIOD
	else if (pio == PIOD)
	{
		callbacksPioD[pos].func = callback;
		callbacksPioD[pos].param = param;
	}
#endif
#ifdef ID_PIOE
	else if (pio == PIOE)
	{
		callbacksPioE[pos].func = callback;
		callbacksPioE[pos].param = param;
	}
#endif

	// Configure the interrupt mode
	if (mode == CHANGE)
	{
		// Disable additional interrupt mode (detects both rising and falling edges)
		pio->PIO_AIMDR = mask;
	}
	else
	{
		// Enable additional interrupt mode
		pio->PIO_AIMER = mask;

		// Select mode of operation
		switch(mode)
		{
		case LOW:
			pio->PIO_LSR = mask;    // "Level" Select Register
			pio->PIO_FELLSR = mask; // "Falling Edge / Low Level" Select Register
			break;

		case HIGH:
			pio->PIO_LSR = mask;    // "Level" Select Register
			pio->PIO_REHLSR = mask; // "Rising Edge / High Level" Select Register
			break;

		case FALLING:
			pio->PIO_ESR = mask;    // "Edge" Select Register
			pio->PIO_FELLSR = mask; // "Falling Edge / Low Level" Select Register
			break;

		case RISING:
			pio->PIO_ESR = mask;    // "Edge" Select Register
			pio->PIO_REHLSR = mask; // "Rising Edge / High Level" Select Register
			break;

		default:
			break;
		}
	}

	// Enable interrupt
	pio->PIO_IFER = mask;			// enable glitch filter on this pin
	pio->PIO_IER = mask;			// enable interrupt on this pin

	cpu_irq_restore(flags);
	return true;
}

void detachInterrupt(uint32_t pin)
{
	// Retrieve pin information
	Pio * const pio = g_APinDescription[pin].pPort;
	const uint32_t mask = g_APinDescription[pin].ulPin;

	// Disable interrupt
	pio->PIO_IDR = mask;
}

// Return true if we are in any interrupt service routine
bool inInterrupt()
{
	return (__get_IPSR() & 0x01FF) != 0;
}

// Common PIO interrupt handler
void CommonPioHandler(Pio *pio, const InterruptCallback callbacks[])
{
	uint32_t isr = pio->PIO_ISR & pio->PIO_IMR;
	while (isr != 0)
	{
		const unsigned int pos = GetHighestBit(isr);
		if (callbacks[pos].func != nullptr)
		{
			callbacks[pos].func(callbacks[pos].param);
		}
		isr &= ~(1u << pos);
	}
}

extern "C" void PIOA_Handler(void)
{
	CommonPioHandler(PIOA, callbacksPioA);
}

extern "C" void PIOB_Handler(void)
{
	CommonPioHandler(PIOB, callbacksPioB);
}

extern "C" void PIOC_Handler(void)
{
	CommonPioHandler(PIOC, callbacksPioC);
}

#ifdef ID_PIOD
extern "C" void PIOD_Handler(void)
{
	CommonPioHandler(PIOD, callbacksPioD);
}
#endif

#ifdef ID_PIOE
extern "C" void PIOE_Handler(void)
{
	CommonPioHandler(PIOE, callbacksPioE);
}
#endif

// End
