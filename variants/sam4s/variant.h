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

// Number of pins defined in PinDescription array
#define APINS_COUNT			(68u)

/*
 * SPI Interfaces
 */

#define SPI_INTERFACE_ID	ID_SPI
static const Pin APIN_SPI_MOSI = 13;
static const Pin APIN_SPI_MISO = 12;
static const Pin APIN_SPI_SCK = 14;
static const Pin APIN_SPI_SS0 = 11;

static const Pin APIN_USART_SSPI_MOSI = 6;
static const Pin APIN_USART_SSPI_MISO = 5;
static const Pin APIN_USART_SSPI_SCK = 2;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT (1)		// SAM4S supports two I2C interfaces but we only have the first one available

static const Pin APINS_WIRE = 70;
#define WIRE_INTERFACE		TWI0
#define WIRE_INTERFACE_ID	ID_TWI0
#define WIRE_ISR_HANDLER	TWI0_Handler
#define WIRE_ISR_ID			TWI0_IRQn

/*
 * UART/USART Interfaces
 */

// Serial

// UART0 used by TMC drivers
static const Pin APINS_UART0 = 71;
static const Pin APIN_UART0_RXD = 9;
static const Pin APIN_UART0_TXD = 10;

#ifdef PCCB

// UART1 also used by TMC drivers in PCCB build
static const Pin APINS_UART1 = 72;
static const Pin APIN_UART1_RXD = 28;
static const Pin APIN_UART1_TXD = 29;

#else

// Serial0 uses UART1
static const Pin APINS_Serial0 = 72;
static const Pin APIN_Serial0_RXD = 28;
static const Pin APIN_Serial0_TXD = 29;

#endif

// HSMCI
static const Pin APIN_HSMCI_CLOCK = 68;
static const Pin APINS_HSMCI_DATA = 69;

// TWI
static const Pin APINS_TWI = 70;

static const uint32_t MaxPinNumber = 67;						// last GPIO pin

static const uint32_t PwmFastClock = 25000 * 255;				// fast PWM clock for Intel spec PWM fans that need 25kHz PWM
static const uint32_t PwmSlowClock = (25000 * 255) / 256;		// slow PWM clock to allow us to get slow speeds

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

#ifndef PCCB
extern UARTClass Serial;
#endif

extern void ConfigurePin(const PinDescription& pinDesc);

#endif

#endif /* _VARIANT_ARDUINO_DUET_X_ */
