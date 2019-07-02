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
# include "UARTClass.h"
# include "USARTClass.h"
#endif

/**
 * Libc porting layers
 */

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#ifdef __GNUC__			// GCC CS3
# include <syscalls.h>	// RedHat Newlib minimal stub
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// The following must be kept in step with the way we organise the pin table in variant.cpp
static inline constexpr Pin PortAPin(unsigned int pin)
{
	return (Pin)pin;
}

static inline constexpr Pin PortBPin(unsigned int pin)
{
	return (Pin)(pin + 32);
}

static inline constexpr Pin PortCPin(unsigned int pin)
{
	return (Pin)(pin + 64);
}

static inline constexpr Pin PortDPin(unsigned int pin)
{
	return (Pin)(pin + 96);
}

static inline constexpr Pin PortEPin(unsigned int pin)
{
	return (Pin)(pin + 128);
}

constexpr uint32_t MaxPinNumber = 133;						// last GPIO pin (PE05)

#ifdef SAME70XPLD

constexpr Pin APIN_GMAC_PHY_INTERRUPT = PortAPin(14);
constexpr Pin APIN_GMAC_PHY_RESET = PortCPin(10);

// Serial0
constexpr Pin APIN_Serial0_RXD = PortAPin(9);
constexpr Pin APIN_Serial0_TXD = PortAPin(10);

// Serial1
constexpr Pin APIN_Serial1_RXD = PortAPin(5);
constexpr Pin APIN_Serial1_TXD = PortAPin(6);

// Shared SPI
#define SHARED_SPI					SPI0
#define SHARED_SPI_INTERFACE_ID		ID_SPI0
constexpr Pin APIN_SHARED_SPI_MOSI = PortDPin(21);
constexpr Pin APIN_SHARED_SPI_MISO = PortDPin(20);
constexpr Pin APIN_SHARED_SPI_SCK = PortDPin(22);
constexpr Pin APIN_SHARED_SPI_SS0 = PortBPin(2);

#else

constexpr Pin APIN_GMAC_PHY_INTERRUPT = PortCPin(6);
constexpr Pin APIN_GMAC_PHY_RESET = PortDPin(11);

// Serial0
constexpr Pin APIN_Serial0_RXD = PortDPin(25);
constexpr Pin APIN_Serial0_TXD = PortDPin(26);

// Serial1
constexpr Pin APIN_Serial1_RXD = PortDPin(18);
constexpr Pin APIN_Serial1_TXD = PortDPin(19);

// Shared SPI (USART 1 on Duet 3)
constexpr Pin APIN_USART_SSPI_SCK = PortBPin(13);
constexpr Pin APIN_USART_SSPI_MOSI = PortBPin(1);
constexpr Pin APIN_USART_SSPI_MISO = PortBPin(0);

// ESP SPI on Duet 3 version 0.3
#define ESP_SPI					SPI0
#define ESP_SPI_INTERFACE_ID	ID_SPI0
#define ESP_SPI_IRQn			SPI0_IRQn
#define ESP_SPI_HANDLER			SPI0_Handler
constexpr Pin APIN_ESP_SPI_MOSI = PortDPin(21);
constexpr Pin APIN_ESP_SPI_MISO = PortDPin(20);
constexpr Pin APIN_ESP_SPI_SCK = PortDPin(22);
constexpr Pin APIN_ESP_SPI_SS0 = PortBPin(2);

// Single Board Computer SPI
#define SBC_SPI					SPI1
#define SBC_SPI_INTERFACE_ID	ID_SPI1
#define SBC_SPI_IRQn			SPI1_IRQn
#define SBC_SPI_HANDLER			SPI1_Handler
constexpr Pin APIN_SBC_SPI_MOSI = PortCPin(27);
constexpr Pin APIN_SBC_SPI_MISO = PortCPin(26);
constexpr Pin APIN_SBC_SPI_SCK = PortCPin(24);
constexpr Pin APIN_SBC_SPI_SS0 = PortCPin(25);

#endif

// HSMCI
constexpr Pin APIN_HSMCI_CLOCK = 134;
constexpr Pin APIN_HSMCI_DATA = 135;

// GMAC
constexpr Pin APINS_GMAC_PHY = 136;

// UART/USART and SPI Interfaces
constexpr Pin APINS_Serial0 = 137;
constexpr Pin APINS_Serial1 = 138;

// CAN
constexpr Pin APIN_CAN1_RX = PortCPin(12);
constexpr Pin APIN_CAN1_TX = PortDPin(12);

// Serial interface objects
extern UARTClass Serial;
extern UARTClass Serial1;

// Pin configuration
extern void ConfigurePin(const PinDescription& pinDesc);
extern void ConfigurePin(Pin pin);
extern bool IsPwmCapable(Pin pin);						// Return true if this pin exists and can do PWM

#endif

#endif /* _VARIANT_SAME70_H */
