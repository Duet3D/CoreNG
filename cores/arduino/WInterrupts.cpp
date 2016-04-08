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

static interruptCB callbacksPioA[32];
static interruptCB callbacksPioB[32];
static interruptCB callbacksPioC[32];
static interruptCB callbacksPioD[32];
#ifdef ID_PIOE
static interruptCB callbacksPioE[32];
#endif

/* Configure PIO interrupt sources */
static void __initialize() {
	for (size_t i=0; i<32; i++) {
		callbacksPioA[i] = NULL;
		callbacksPioB[i] = NULL;
		callbacksPioC[i] = NULL;
		callbacksPioD[i] = NULL;
#ifdef ID_PIOE
		callbacksPioE[i] = NULL;
#endif
	}

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
	Pio *pio = g_APinDescription[pin].pPort;
	uint32_t mask = g_APinDescription[pin].ulPin;
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
	Pio *pio = g_APinDescription[pin].pPort;
	uint32_t mask = g_APinDescription[pin].ulPin;

	// Disable interrupt
	pio->PIO_IDR = mask;
}

// Return true if we are in an interrupt service routine
bool inInterrupt()
{
	return (__get_IPSR() & 0x01FF) != 0;
}

extern "C" void PIOA_Handler(void)
{
	uint32_t isr = PIOA->PIO_ISR;
	for (uint32_t i=0; i<32; i++, isr>>=1)
	{
		if ((isr & 0x1) != 0 && callbacksPioA[i] != nullptr)
		{
			callbacksPioA[i]();
		}
	}
}

extern "C" void PIOB_Handler(void)
{
	uint32_t isr = PIOB->PIO_ISR;
	for (uint32_t i=0; i<32; i++, isr>>=1)
	{
		if ((isr & 0x1) != 0 && callbacksPioB[i] != nullptr)
		{
			callbacksPioB[i]();
		}
	}
}

extern "C" void PIOC_Handler(void)
{
	uint32_t isr = PIOC->PIO_ISR;
	for (uint32_t i=0; i<32; i++, isr>>=1)
	{
		if ((isr & 0x1) != 0 && callbacksPioC[i] != nullptr)
		{
			callbacksPioC[i]();
		}
	}
}

extern "C" void PIOD_Handler(void)
{
	uint32_t isr = PIOD->PIO_ISR;
	for (uint32_t i=0; i<32; i++, isr>>=1)
	{
		if ((isr & 0x1) != 0 && callbacksPioD[i] != nullptr)
		{
			callbacksPioD[i]();
		}
	}
}

#ifdef ID_PIOE
extern "C" void PIOE_Handler(void)
{
	uint32_t isr = PIOE->PIO_ISR;
	for (uint32_t i=0; i<32; i++, isr>>=1)
	{
		if ((isr & 0x1) != 0 && callbacksPioE[i] != nullptr)
		{
			callbacksPioE[i]();
		}
	}
}
#endif

// End
