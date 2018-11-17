/*
  Copyright (c) 2011 Arduino.  All right reserved.

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

#ifndef _VARIANT_DUET_NG_H
#define _VARIANT_DUET_NG_H

#ifndef __SAM4E8E__
#error Wrong variant.h file included!
#endif

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		12000000

/** Master clock frequency */
#define VARIANT_MCK			120000000

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "Core.h"

#ifdef __cplusplus
# include "UARTClass.h"
# include "USARTClass.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/**
 * Libc porting layers
 */
#if defined (  __GNUC__  ) /* GCC CS3 */
#    include <syscalls.h> /** RedHat Newlib minimal stub */
#endif

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

static const uint32_t MaxPinNumber = 105;						// last GPIO pin

#ifdef __cplusplus

// The following must be kept in step with the way we organise the pin table in variant.cpp
static inline constexpr Pin PortAPin(unsigned int pin)
{
	return (Pin)pin;
}

static inline constexpr Pin PortBPin(unsigned int pin)
{
	return (Pin)(
					(pin <= 3) ? 26 + pin
						: (pin <= 5) ? (104 - 4) + pin
							: (pin <= 7) ? (102 - 6) + pin
								: (30 - 13) + pin
				);
}

static inline constexpr Pin PortCPin(unsigned int pin)
{
	return (Pin)(32 + pin);
}

static inline constexpr Pin PortDPin(unsigned int pin)
{
	return (Pin)(64 + pin);
}

static inline constexpr Pin PortEPin(unsigned int pin)
{
	return (Pin)(96 + pin);
}

#endif

/*
 * SPI Interfaces
 */

#define SPI_INTERFACE_ID	ID_SPI
static const Pin APIN_SPI_MOSI = 13;
static const Pin APIN_SPI_MISO = 12;
static const Pin APIN_SPI_SCK = 14;
static const Pin APIN_SPI_SS0 = 11;

static const Pin APIN_USART_SSPI_MOSI = 27;
static const Pin APIN_USART_SSPI_MISO = 26;
static const Pin APIN_USART_SSPI_SCK = 30;

static const Pin APIN_USART1_MOSI = 22;
static const Pin APIN_USART1_MISO = 21;
static const Pin APIN_USART1_SCK = 23;

/*
 * TWI Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

static const Pin APINS_WIRE = 108;

#define WIRE_INTERFACE		TWI0
#define WIRE_INTERFACE_ID	ID_TWI0
#define WIRE_ISR_HANDLER	TWI0_Handler
#define WIRE_ISR_ID			TWI0_IRQn

/*
 * UART/USART Interfaces
 */

// Serial
static const Pin APINS_Serial0 = 109;
static const Pin APIN_Serial0_RXD = 9;
static const Pin APIN_Serial0_TXD = 10;

// Serial1
static const Pin APINS_Serial1 = 110;
static const Pin APIN_Serial1_RXD = 5;
static const Pin APIN_Serial1_TXD = 6;

/*
 * Duet NG pins
 */

// HSMCI
static const Pin APIN_HSMCI_CLOCK = 106;
static const Pin APINS_HSMCI_DATA = 107;

// TWI
static const Pin APINS_TWI = 108;

static const uint32_t PwmFastClock = 25000 * 255;				// fast PWM clock for Intel spec PWM fans that need 25kHz PWM
static const uint32_t PwmSlowClock = (25000 * 255) / 256;		// slow PWM clock to allow us to get slow speeds

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

extern UARTClass Serial;
extern UARTClass Serial1;

extern void ConfigurePin(const PinDescription& pinDesc);

#endif

#endif /* _VARIANT_DUET_NG_H */
