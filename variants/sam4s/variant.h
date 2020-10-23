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

#ifndef _VARIANT_H
#define _VARIANT_H

#ifndef __SAM4S8C__
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

/**
 * Libc porting layers
 */

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

constexpr uint32_t MaxPinNumber = 67;						// last GPIO pin

// The following must be kept in step with the way we organise the pin table in variant.cpp
static inline constexpr Pin PortAPin(unsigned int pin) noexcept
{
	return (Pin)pin;							// PA0-24 are 0-25
}

static inline constexpr Pin PortBPin(unsigned int pin) noexcept
{
	return (Pin)(
					(pin <= 7) ? 26 + pin		// PB0-7 are 26-33
						: (34 - 13) + pin		// PB13-14 are 34-35
				);
}

static inline constexpr Pin PortCPin(unsigned int pin) noexcept
{
	return (Pin)(36 + pin);						// PC0-31 are 36-67
}

/*
 * SPI Interfaces
 */

// Main SPI interface
constexpr Pin APIN_SPI_MOSI = 13;
constexpr Pin APIN_SPI_MISO = 12;
constexpr Pin APIN_SPI_SCK = 14;
constexpr Pin APIN_SPI_SS0 = 11;

// USARTs used for SPI
constexpr Pin APIN_USART_SSPI_MOSI = 6;
constexpr Pin APIN_USART_SSPI_MISO = 5;
constexpr Pin APIN_USART_SSPI_SCK = 2;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT (1)		// SAM4S supports two I2C interfaces but we only have the first one available

constexpr Pin APINS_WIRE = 70;
#define WIRE_INTERFACE		TWI0
#define WIRE_INTERFACE_ID	ID_TWI0
#define WIRE_ISR_HANDLER	TWI0_Handler
#define WIRE_ISR_ID			TWI0_IRQn

// Serial

// UART0 used by TMC drivers
constexpr Pin APINS_UART0 = 71;
constexpr Pin APIN_UART0_RXD = 9;
constexpr Pin APIN_UART0_TXD = 10;

#ifdef PCCB

// UART1 also used by TMC drivers in PCCB build
constexpr Pin APINS_UART1 = 72;
constexpr Pin APIN_UART1_RXD = 28;
constexpr Pin APIN_UART1_TXD = 29;

#else

// Serial0 uses UART1
constexpr Pin APINS_Serial0 = 72;
constexpr Pin APIN_Serial0_RXD = 28;
constexpr Pin APIN_Serial0_TXD = 29;

#endif

// HSMCI
constexpr Pin APIN_HSMCI_CLOCK = 68;
constexpr Pin APINS_HSMCI_DATA = 69;

// TWI
constexpr Pin APINS_TWI = 70;

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifndef PCCB
extern UARTClass Serial;
#endif

// We need to use "extern C++" here so that it compiles even if this file was #included inside an "extern C" block
extern "C++" {
	void ConfigurePin(const PinDescription& pinDesc) noexcept;
	void ConfigurePin(Pin pin) noexcept;
	bool IsPwmCapable(Pin pin) noexcept;						// Return true if this pin exists and can do PWM
}

#endif

#endif /* _VARIANT_H */
