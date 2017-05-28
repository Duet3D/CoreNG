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

typedef void (*interruptCB)(void);

static interruptCB callbacksPioA[32] = { 0 };
static interruptCB callbacksPioB[32] = { 0 };
static interruptCB callbacksPioC[32] = { 0 };
static interruptCB callbacksPioD[32] = { 0 };
#ifdef ID_PIOE
static interruptCB callbacksPioE[32] = { 0 };
#endif

/* Configure PIO interrupt sources */
static void __initialize()
{
	pmc_enable_periph_clk(ID_PIOA);
	NVIC_DisableIRQ(PIOA_IRQn);
	NVIC_ClearPendingIRQ(PIOA_IRQn);
	NVIC_SetPriority(PIOA_IRQn, pioInterruptPriority);
	NVIC_EnableIRQ(PIOA_IRQn);

	pmc_enable_periph_clk(ID_PIOB);
	NVIC_DisableIRQ(PIOB_IRQn);
	NVIC_ClearPendingIRQ(PIOB_IRQn);
	NVIC_SetPriority(PIOB_IRQn, pioInterruptPriority);
	NVIC_EnableIRQ(PIOB_IRQn);

	pmc_enable_periph_clk(ID_PIOC);
	NVIC_DisableIRQ(PIOC_IRQn);
	NVIC_ClearPendingIRQ(PIOC_IRQn);
	NVIC_SetPriority(PIOC_IRQn, pioInterruptPriority);
	NVIC_EnableIRQ(PIOC_IRQn);

	pmc_enable_periph_clk(ID_PIOD);
	NVIC_DisableIRQ(PIOD_IRQn);
	NVIC_ClearPendingIRQ(PIOD_IRQn);
	NVIC_SetPriority(PIOD_IRQn, pioInterruptPriority);
	NVIC_EnableIRQ(PIOD_IRQn);

#ifdef ID_PIOE
	pmc_enable_periph_clk(ID_PIOE);
	NVIC_DisableIRQ(PIOE_IRQn);
	NVIC_ClearPendingIRQ(PIOE_IRQn);
	NVIC_SetPriority(PIOE_IRQn, pioInterruptPriority);
	NVIC_EnableIRQ(PIOE_IRQn);
#endif
}

extern "C" void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	static int enabled = 0;
	if (!enabled)
	{
		__initialize();
		enabled = 1;
	}

	// Retrieve pin information
	Pio * const pio = g_APinDescription[pin].pPort;
	const uint32_t mask = g_APinDescription[pin].ulPin;
	uint32_t pos = 0;

	for (uint32_t t = mask; t>1; t>>=1, pos++) {}

	// Set callback function
	if (pio == PIOA)
	{
		callbacksPioA[pos] = callback;
	}
	else if (pio == PIOB)
	{
		callbacksPioB[pos] = callback;
	}
	else if (pio == PIOC)
	{
		callbacksPioC[pos] = callback;
	}
	else if (pio == PIOD)
	{
		callbacksPioD[pos] = callback;
	}
#ifdef ID_PIOE
	else if (pio == PIOE)
	{
		callbacksPioE[pos] = callback;
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
		if (mode == LOW) {
			pio->PIO_LSR = mask;    // "Level" Select Register
			pio->PIO_FELLSR = mask; // "Falling Edge / Low Level" Select Register
		}
		if (mode == HIGH) {
			pio->PIO_LSR = mask;    // "Level" Select Register
			pio->PIO_REHLSR = mask; // "Rising Edge / High Level" Select Register
		}
		if (mode == FALLING) {
			pio->PIO_ESR = mask;    // "Edge" Select Register
			pio->PIO_FELLSR = mask; // "Falling Edge / Low Level" Select Register
		}
		if (mode == RISING) {
			pio->PIO_ESR = mask;    // "Edge" Select Register
			pio->PIO_REHLSR = mask; // "Rising Edge / High Level" Select Register
		}
	}

	// Enable interrupt
	pio->PIO_IER = mask;
}

extern "C" void detachInterrupt(uint32_t pin)
{
	// Retrieve pin information
	Pio * const pio = g_APinDescription[pin].pPort;
	const uint32_t mask = g_APinDescription[pin].ulPin;

	// Disable interrupt
	pio->PIO_IDR = mask;
}

// Return true if we are in an interrupt service routine
bool inInterrupt()
{
	return (__get_IPSR() & 0x01FF) != 0;
}

// Common PIO interrupt handler
void CommonPioHandler(Pio *pio, const interruptCB callbacks[])
{
	uint32_t isr = pio->PIO_ISR & pio->PIO_IMR;
	// The following loop is partially unwound for better speed
	for (unsigned int i = 0; i < 8; ++i)
	{
		if ((isr & 0x01) != 0 && callbacks[0] != nullptr)
		{
			callbacks[0]();
		}
		if ((isr & 0x02) != 0 && callbacks[1] != nullptr)
		{
			callbacks[1]();
		}
		if ((isr & 0x04) != 0 && callbacks[2] != nullptr)
		{
			callbacks[2]();
		}
		if ((isr & 0x08) != 0 && callbacks[3] != nullptr)
		{
			callbacks[3]();
		}
		isr >>= 4;
		callbacks += 4;
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

extern "C" void PIOD_Handler(void)
{
	CommonPioHandler(PIOD, callbacksPioD);
}

#ifdef ID_PIOE
extern "C" void PIOE_Handler(void)
{
	CommonPioHandler(PIOE, callbacksPioE);
}
#endif

// End
