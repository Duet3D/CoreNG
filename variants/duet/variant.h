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
# include "UARTClass.h"
# include "USARTClass.h"
#endif

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

/*
 * SPI Interfaces
 */

#define SHARED_SPI					SPI0
#define SHARED_SPI_INTERFACE_ID     ID_SPI0
constexpr Pin APIN_SPI_SS0 = 77;
constexpr Pin APIN_SPI_SS1 = 87;
constexpr Pin APIN_SPI_SS2 = 86;
constexpr Pin APIN_SPI_SS3 = 78;
constexpr Pin APIN_SHARED_SPI_MOSI = 75;
constexpr Pin APIN_SHARED_SPI_MISO = 74;
constexpr Pin APIN_SHARED_SPI_SCK = 76;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 2

constexpr Pin APINS_WIRE = 80;
#define WIRE_INTERFACE       TWI1
#define WIRE_INTERFACE_ID    ID_TWI1
#define WIRE_ISR_HANDLER     TWI1_Handler
#define WIRE_ISR_ID          TWI1_IRQn

constexpr Pin APINS_WIRE1 = 79;
#define WIRE1_INTERFACE      TWI0
#define WIRE1_INTERFACE_ID   ID_TWI0
#define WIRE1_ISR_HANDLER    TWI0_Handler
#define WIRE1_ISR_ID         TWI0_IRQn

/*
 * UART/USART Interfaces
 */
// Serial, Serial1,2,3
constexpr Pin APINS_UART = 81;
constexpr Pin APIN_UART_RXD	= 0;
constexpr Pin APINS_USART0 = 82;
constexpr Pin APINS_USART1 = 83;
constexpr Pin APINS_USART3 = 84;

/*
 * USB Interfaces
 */
constexpr Pin APINS_USB = 85;

/*
 * Analog pins
 */
constexpr Pin A0  = 54;
constexpr Pin A1  = 55;
constexpr Pin A2  = 56;
constexpr Pin A3  = 57;
constexpr Pin A4  = 58;
constexpr Pin A5  = 59;
constexpr Pin A6  = 60;
constexpr Pin A7  = 61;
constexpr Pin A8  = 62;
constexpr Pin A9  = 63;
constexpr Pin A10 = 64;
constexpr Pin A11 = 65;
constexpr Pin DAC0 = 66;
constexpr Pin DAC1 = 67;

/*
 * Duet pins
 */

constexpr Pin X0  = 92;
constexpr Pin X1  = 93;
constexpr Pin X2  = 94;
constexpr Pin X3  = 95;
constexpr Pin X4  = 96;
constexpr Pin X5  = 97;
constexpr Pin X6  = 98;
constexpr Pin X7  = 99;
constexpr Pin X8  = 100;
constexpr Pin X9  = 101;
constexpr Pin X10 = 102;
constexpr Pin X11 = 103;
constexpr Pin X12 = 104;
constexpr Pin X13 = 105;
constexpr Pin X14 = 106;
constexpr Pin X15 = 106;
constexpr Pin X16 = 108;
constexpr Pin X17 = 109;

constexpr uint32_t MaxPinNumber = 109;					// X17

// HSMCI
constexpr Pin APIN_HSMCI_CLOCK = 110;
constexpr Pin APINS_HSMCI_DATA = 111;

// EMAC
constexpr Pin APINS_EMAC = 112;

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

extern UARTClass Serial;
extern USARTClass Serial1;
extern USARTClass Serial2;

// We need to use "extern C++" here so that it compiles even if this file was #included inside an "extern C" block
extern "C++" {
	// Pin configuration
	void ConfigurePin(const PinDescription& pinDesc);
	void ConfigurePin(Pin pin);
	bool IsPwmCapable(Pin pin);						// Return true if this pin exists and can do PWM
}

#endif

#endif /* _VARIANT_ARDUINO_DUET_X_ */
