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

/** Master clock frequency */
#define VARIANT_MCK			150000000

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

// The following must be kept in step with the way we organise the pin table in variant.cpp
static inline constexpr Pin PortAPin(unsigned int pin) noexcept
{
	return (Pin)pin;
}

static inline constexpr Pin PortBPin(unsigned int pin) noexcept
{
	return (Pin)(pin + 32);
}

static inline constexpr Pin PortCPin(unsigned int pin) noexcept
{
	return (Pin)(pin + 64);
}

static inline constexpr Pin PortDPin(unsigned int pin) noexcept
{
	return (Pin)(pin + 96);
}

static inline constexpr Pin PortEPin(unsigned int pin) noexcept
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

// SPI1 pin definitions
constexpr Pin APIN_SPI1_MOSI = PortCPin(27);
constexpr Pin APIN_SPI1_MISO = PortCPin(26);
constexpr Pin APIN_SPI1_SCK = PortCPin(24);
constexpr Pin APIN_SPI1_SS0 = PortCPin(25);

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
constexpr Pin APIN_CAN0_RX = PortBPin(3);
constexpr Pin APIN_CAN0_TX = PortBPin(2);

constexpr Pin APIN_CAN1_RX = PortCPin(12);
constexpr Pin APIN_CAN1_TX = PortDPin(12);

// Serial interface objects
extern UARTClass Serial;
extern USARTClass Serial1;

// We need to use "extern C++" here so that it compiles even if this file was #included inside an "extern C" block
extern "C++" {
	// Pin configuration
	void ConfigurePin(const PinDescription& pinDesc) noexcept;
	void ConfigurePin(Pin pin) noexcept;
	bool IsPwmCapable(Pin pin) noexcept;						// Return true if this pin exists and can do PWM
}

#endif

#endif /* _VARIANT_SAME70_H */
