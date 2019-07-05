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
#ifdef  __GNUC__		// GCC CS3
# include <syscalls.h>	// RedHat Newlib minimal stub
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

constexpr uint32_t MaxPinNumber = 105;						// last GPIO pin

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

/*
 * SPI Interfaces
 */

#define ESP_SPI					SPI
#define ESP_SPI_INTERFACE_ID	ID_SPI
#define ESP_SPI_IRQn			SPI_IRQn
#define ESP_SPI_HANDLER			SPI_Handler

constexpr Pin APIN_ESP_SPI_MOSI = 13;
constexpr Pin APIN_ESP_SPI_MISO = 12;
constexpr Pin APIN_ESP_SPI_SCK = 14;
constexpr Pin APIN_ESP_SPI_SS0 = 11;

#define W5500_SPI				SPI
#define W5500_SPI_INTERFACE_ID	ID_SPI
#define W5500_SPI_IRQn			SPI_IRQn
#define W5500_SPI_HANDLER		SPI_Handler
constexpr Pin APIN_W5500_SPI_MOSI = 13;
constexpr Pin APIN_W5500_SPI_MISO = 12;
constexpr Pin APIN_W5500_SPI_SCK = 14;
constexpr Pin APIN_W5500_SPI_SS0 = 11;

constexpr Pin APIN_USART_SSPI_MOSI = 27;
constexpr Pin APIN_USART_SSPI_MISO = 26;
constexpr Pin APIN_USART_SSPI_SCK = 30;

constexpr Pin APIN_USART1_MOSI = 22;
constexpr Pin APIN_USART1_MISO = 21;
constexpr Pin APIN_USART1_SCK = 23;

/*
 * TWI Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

constexpr Pin APINS_WIRE = 108;

#define WIRE_INTERFACE		TWI0
#define WIRE_INTERFACE_ID	ID_TWI0
#define WIRE_ISR_HANDLER	TWI0_Handler
#define WIRE_ISR_ID			TWI0_IRQn

// Serial
constexpr Pin APINS_Serial0 = 109;
constexpr Pin APIN_Serial0_RXD = 9;
constexpr Pin APIN_Serial0_TXD = 10;

// Serial1
constexpr Pin APINS_Serial1 = 110;
constexpr Pin APIN_Serial1_RXD = 5;
constexpr Pin APIN_Serial1_TXD = 6;

/*
 * Duet NG pins
 */

// HSMCI
constexpr Pin APIN_HSMCI_CLOCK = 106;
constexpr Pin APINS_HSMCI_DATA = 107;

// Serial interface objects
extern UARTClass Serial;
extern UARTClass Serial1;

// Pin configuration
extern void ConfigurePin(const PinDescription& pinDesc);
extern void ConfigurePin(Pin pin);
extern bool IsPwmCapable(Pin pin);						// Return true if this pin exists and can do PWM

#endif

#endif /* _VARIANT_H */
