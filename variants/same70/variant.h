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

#ifndef _VARIANT_SAME70_H
#define _VARIANT_SAME70_H

#ifndef __SAME70Q21__
#error Wrong variant.h file included!
#endif

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		12000000

/** Master clock frequency */
#define VARIANT_MCK			150000000

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "Core.h"

#ifdef __cplusplus
#include "UARTClass.h"
#include "USARTClass.h"
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

/*
 * SPI Interfaces
 */

#define SPI_INTERFACE_ID	ID_SPI
#define APIN_SPI_MOSI		(42u)
#define APIN_SPI_MISO		(41u)
#define APIN_SPI_SCK		(43u)
#define APIN_SPI_SS0		(20u)

#if 0	// TODO
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
#endif

/*
 * UART/USART Interfaces
 */
// SerialUSB
//#define USB_VBUS_PIN		(??)		// not present on the test board, can be any free pin
// Serial
static const uint8_t APINS_UART0 = 56;
static const uint8_t APIN_UART0_RXD = 5;
static const uint8_t APIN_UART0_TXD = 6;
// Serial1
static const uint8_t APINS_UART1 = 57;
static const uint8_t APIN_UART1_RXD = 3;
static const uint8_t APIN_UART1_TXD = 4;

/*
 * SAM E70 test board pins
 */

// HSMCI
static const uint8_t APIN_HSMCI_CLOCK = 51;
static const uint8_t APIN_HSMCI_DATA = 52;

// PHY
static const uint8_t APIN_GMAC_PHY_INTERRUPT = 53;
static const uint8_t APIN_GMAC_PHY_RESET = 54;
static const uint8_t APINS_GMAC_PHY = 55;

static const uint32_t MaxPinNumber = 50;						// last GPIO pin

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

#endif /* _VARIANT_SAME70_H */
