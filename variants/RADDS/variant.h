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

#ifndef _VARIANT_ARDUINO_DUET_X_
#define _VARIANT_ARDUINO_DUET_X_

#ifndef __SAM3X8E__
#error Wrong variant.h file included!
#endif

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		12000000

/** Master clock frequency */
#define VARIANT_MCK			84000000

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

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define APINS_COUNT				(79u)

/*
 * SPI Interfaces
 */

#define SHARED_SPI				SPI0
#define SHARED_SPI_INTERFACE_ID	ID_SPI0
#define APIN_SPI_SS0			(77u)
#define APIN_SPI_SS1			(87u)
#define APIN_SPI_SS2 			(86u)
#define APIN_SPI_SS3			(78u)
#define APIN_SHARED_SPI_MOSI	(75u)
#define APIN_SHARED_SPI_MISO	(74u)
#define APIN_SHARED_SPI_SCK		(76u)

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 2

#define APINS_WIRE	         (80u)
#define WIRE_INTERFACE       TWI1
#define WIRE_INTERFACE_ID    ID_TWI1
#define WIRE_ISR_HANDLER     TWI1_Handler
#define WIRE_ISR_ID          TWI1_IRQn

#define APINS_WIRE1			 (79u)
#define WIRE1_INTERFACE      TWI0
#define WIRE1_INTERFACE_ID   ID_TWI0
#define WIRE1_ISR_HANDLER    TWI0_Handler
#define WIRE1_ISR_ID         TWI0_IRQn

/*
 * UART/USART Interfaces
 */
// Serial
#define APINS_UART           (81u)
#define APIN_UART_RXD		 (0u)
// Serial1
#define APINS_USART0         (82u)
#define APIN_USART0_RXD		 (19u)
// Serial2
#define APINS_USART1         (83u)
// Serial3
#define APINS_USART3         (84u)

/*
 * USB Interfaces
 */
#define APINS_USB            (85u)

/*
 * Analog pins
 */
static const uint8_t A0  = 54;
static const uint8_t A1  = 55;
static const uint8_t A2  = 56;
static const uint8_t A3  = 57;
static const uint8_t A4  = 58;
static const uint8_t A5  = 59;
static const uint8_t A6  = 60;
static const uint8_t A7  = 61;
static const uint8_t A8  = 62;
static const uint8_t A9  = 63;
static const uint8_t A10 = 64;
static const uint8_t A11 = 65;
static const uint8_t DAC0 = 66;
static const uint8_t DAC1 = 67;
static const uint8_t CANRX = 68;
static const uint8_t CANTX = 69;

/*
 * Complementary CAN pins
 */
static const uint8_t CAN1RX = 88;
static const uint8_t CAN1TX = 89;

// CAN0
#define APINS_CAN0           (90u)
// CAN1
#define APINS_CAN1           (91u)

/*
 * Duet pins
 */

static const uint8_t X0  = 92;
static const uint8_t X1  = 93;
static const uint8_t X2  = 94;
static const uint8_t X3  = 95;
static const uint8_t X4  = 96;
static const uint8_t X5  = 97;
static const uint8_t X6  = 98;
static const uint8_t X7  = 99;
static const uint8_t X8  = 100;
static const uint8_t X9  = 101;
static const uint8_t X10 = 102;
static const uint8_t X11 = 103;
static const uint8_t X12 = 104;
static const uint8_t X13 = 105;
static const uint8_t X14 = 106;
static const uint8_t X15 = 106;
static const uint8_t X16 = 108;
static const uint8_t X17 = 109;

static const uint32_t MaxPinNumber = 109;					// X17

// HSMCI
static const uint8_t APIN_HSMCI_CLOCK = 110;
static const uint8_t APINS_HSMCI_DATA = 111;

// EMAC
static const uint8_t APINS_EMAC = 112;

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

extern UARTClass Serial;
extern USARTClass Serial1;
extern USARTClass Serial2;

// We need to use "extern C++" here so that it compiles even if this file was #included inside an "extern C" block
extern "C++" {
	void ConfigurePin(const PinDescription& pinDesc);
	void ConfigurePin(Pin pin);
	bool IsPwmCapable(Pin pin);						// Return true if this pin exists and can do PWM
}

#endif

#endif /* _VARIANT_ARDUINO_DUET_X_ */
