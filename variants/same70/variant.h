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

#if !(defined(__SAME70Q21__) || defined(__SAME70Q20B__) || defined(__SAME70Q21B__))
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
#if defined (__GNUC__) /* GCC CS3 */
# include <syscalls.h> /** RedHat Newlib minimal stub */
#endif

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

#define PORTA_PIN(n)	(n)
#define PORTB_PIN(n)	(32+n)
#define PORTC_PIN(n)	(64+n)
#define PORTD_PIN(n)	(96+n)
#define PORTE_PIN(n)	(128+n)

// UART/USART and SPI Interfaces

static const uint8_t APINS_Serial0 = 137;
static const uint8_t APINS_Serial1 = 138;

#ifdef SAME70XPLD

// No USB_VBUS_PIN on the XPLD
static const uint8_t APIN_GMAC_PHY_INTERRUPT = PORTA_PIN(14);
static const uint8_t APIN_GMAC_PHY_RESET = PORTC_PIN(10);

// Serial
static const uint8_t APIN_Serial0_RXD = PORTA_PIN(9);
static const uint8_t APIN_Serial0_TXD = PORTA_PIN(10);

// Serial1
static const uint8_t APIN_Serial1_RXD = PORTA_PIN(5);
static const uint8_t APIN_Serial1_TXD = PORTA_PIN(6);

// ESP SPI
#define SPI_INTERFACE_ID	ID_SPI0
static const uint8_t APIN_SPI_MOSI = PORTD_PIN(21);
static const uint8_t APIN_SPI_MISO = PORTD_PIN(20);
static const uint8_t APIN_SPI_SCK = PORTD_PIN(22);
static const uint8_t APIN_SPI_SS0 = PORTB_PIN(2);

#else

static const uint8_t APIN_GMAC_PHY_INTERRUPT = PORTC_PIN(6);
static const uint8_t APIN_GMAC_PHY_RESET = PORTD_PIN(11);

// Serial
static const uint8_t APIN_Serial0_RXD = PORTD_PIN(25);
static const uint8_t APIN_Serial0_TXD = PORTD_PIN(26);

// Serial1
static const uint8_t APIN_Serial1_RXD = PORTD_PIN(18);
static const uint8_t APIN_Serial1_TXD = PORTD_PIN(19);

// Shared SPI (USART 1 on Duet 3)
static const uint8_t APIN_USART_SSPI_SCK = PORTB_PIN(13);
static const uint8_t APIN_USART_SSPI_MOSI = PORTB_PIN(1);
static const uint8_t APIN_USART_SSPI_MISO = PORTB_PIN(0);

// ESP SPI
#define SPI_INTERFACE_ID	ID_SPI0
static const uint8_t APIN_SPI_MOSI = PORTD_PIN(21);
static const uint8_t APIN_SPI_MISO = PORTD_PIN(20);
static const uint8_t APIN_SPI_SCK = PORTD_PIN(22);
static const uint8_t APIN_SPI_SS0 = PORTB_PIN(2);

#endif

// HSMCI
static const uint8_t APIN_HSMCI_CLOCK = 134;
static const uint8_t APIN_HSMCI_DATA = 135;

static const uint8_t APINS_GMAC_PHY = 136;

// CAN
static const uint8_t APIN_CAN1_RX = PORTC_PIN(12);
static const uint8_t APIN_CAN1_TX = PORTD_PIN(12);

static const uint32_t MaxPinNumber = 133;						// last GPIO pin (PE05)

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
