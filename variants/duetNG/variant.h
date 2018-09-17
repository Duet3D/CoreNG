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

/*
 * SPI Interfaces
 */

#define SPI_INTERFACE_ID	ID_SPI
#define APIN_SPI_MOSI		(13u)
#define APIN_SPI_MISO		(12u)
#define APIN_SPI_SCK		(14u)
#define APIN_SPI_SS0		(11u)

#define APIN_USART0_MOSI	(27u)
#define APIN_USART0_MISO	(26u)
#define APIN_USART0_SCK		(30u)

#define APIN_USART1_MOSI	(22u)
#define APIN_USART1_MISO	(21u)
#define APIN_USART1_SCK		(23u)

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define APIN_WIRE_SDA		(3u)
#define APIN_WIRE_SCL		(4u)
#define WIRE_INTERFACE		TWI0
#define WIRE_INTERFACE_ID	ID_TWI0
#define WIRE_ISR_HANDLER	TWI0_Handler
#define WIRE_ISR_ID			TWI0_IRQn


/*
 * UART/USART Interfaces
 */
// SerialUSB
#define USB_VBUS_PIN		(54u)

// Serial
#define APINS_UART0			(109u)
#define APIN_UART0_RXD		(9u)
#define APIN_UART0_TXD		(10u)

// Serial1
#define APINS_UART1			(110u)
#define APIN_UART1_RXD		(5u)
#define APIN_UART1_TXD		(6u)

/*
 * Duet NG pins
 */

// Serial1
static const uint8_t APINS_Serial1 = APINS_UART1;
static const uint8_t APIN_Serial1_RXD = APIN_UART1_RXD;
static const uint8_t APIN_Serial1_TXD = APIN_UART1_TXD;

// Shared SPI (USART 0 on Duet NG)
static const uint8_t APIN_USART_SSPI_SCK = APIN_USART0_SCK;
static const uint8_t APIN_USART_SSPI_MOSI = APIN_USART0_MOSI;
static const uint8_t APIN_USART_SSPI_MISO = APIN_USART0_MISO;

// HSMCI
static const uint8_t APIN_HSMCI_CLOCK = 106;
static const uint8_t APINS_HSMCI_DATA = 107;

// TWI
static const uint8_t APINS_TWI = 108;

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
