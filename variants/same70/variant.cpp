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

#include "variant.h"
#include "udc.h"

/* For the SAME70Q21 test board we use a logical pin numbering scheme:
 * Pins   0-17  PA0-6, 9-13, 17-19, 21-24, 29
 * Pins  18-26  PB0-7, 13
 * Pins  27-36  PC8-9, 12-14, 16-17, 19, 30-31
 * Pins  37-50  PD11-12, 18-22, 24-28, 30-31
 */

/*
 * Pins descriptions
 */
extern const PinDescription g_APinDescription[] =
{
  // PIO A

  // 0-2
  { PIOA, PIO_PA0,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA1,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA2,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PA3-4 are connected to EEPROM on the test board (TWI0)
  //{ PIOA, PIO_PA3A_TWD0,       ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOA, PIO_PA4A_TWCK0,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 3-4 UART1
  { PIOA, PIO_PA5C_URXD1,      ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD1
  { PIOA, PIO_PA6C_UTXD1,      ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD1

  // PA7-8 are connected to XTAL32 on the test board
  //{ PIOA, PIO_PA7,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOA, PIO_PA8,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 5-6 UART0
  { PIOA, PIO_PA9A_URXD0,      ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD0
  { PIOA, PIO_PA10A_UTXD0,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD0

  // 7-9
  // PA11 is connected to USER SWITCH on the test board (active low)
  { PIOA, PIO_PA11,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA12,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA13,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PA15-16 are connected to RAM on the test board
  //{ PIOA, PIO_PA15,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOA, PIO_PA16,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 10-12 (ADC)
  { PIOA, PIO_PA17,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC6,   NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA18,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC7,   NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA19,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ESP transfer request

  // PA20 is connected to RAM on the test board
  //{ PIOA, PIO_PA20,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC9,  NOT_ON_PWM,  NOT_ON_TIMER },

  // 13-16
  { PIOA, PIO_PA21,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC1,   NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA22,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA23,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA24,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PA25-28 are used for HSMCI

  // 17
  { PIOA, PIO_PA29,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PA30 is connected to SD_D0 on the test board
  // PA31 is connected to SD_D1 on the test board

  // PIO B

  // 18-19
  { PIOB, PIO_PB0,             ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, PIO_PB1,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 20-21 (CAN0?) [currently used for CS from ESP]
  { PIOB, PIO_PB2,             ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ESP CS
  { PIOB, PIO_PB3,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 22-23 (TWI1?)
  { PIOB, PIO_PB4,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, PIO_PB5,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 24-25
  { PIOB, PIO_PB6,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, PIO_PB7,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PB8-PB9 are not available (Crystal) [assuming we use two pins for the crystal, else we get a spare pin]
  // PB10-PB11 are not present on chip
  // PB12 is not available (Erase)

  // 26
  { PIOB, PIO_PB13,            ID_PIOB, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO C

  // PC0-7 are connected to RAM on the test board
  //{ PIOC, PIO_PC0,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC1,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC2,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC3,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC4,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC5,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC6,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC7,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 27-28
  // PC8 is connected to USER LED on the test board (active low)
  { PIOC, PIO_PC8,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC9,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PC11 is connected to EEPROM on the test board
  //{ PIOC, PIO_PC11,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 29-31
  { PIOC, PIO_PC12,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC13,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC14,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PC15 is connected to RAM on the test board

  // 32
  { PIOC, PIO_PC16,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },	// HSMCI CD on the test board

  // 33
  { PIOC, PIO_PC17,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PC18 is connected to RAM on the test board

  // 34
  { PIOC, PIO_PC19,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PC20-29 are connected to RAM on the test board
  //{ PIOC, PIO_PC20,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC21,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC22,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC23,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC24,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC25,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC26,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC27,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC28,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOC, PIO_PC29,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 35-36
  { PIOC, PIO_PC30,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC31,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO D

  // PD0-10 are connected to the Ethernet PHY

  // 37-38
  { PIOD, PIO_PD11,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD12,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PD13-17 are connected to RAM on the test board
  //{ PIOD, PIO_PD13,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOD, PIO_PD14,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOD, PIO_PD15,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOD, PIO_PD16,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOD, PIO_PD17,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 39-40
  { PIOD, PIO_PD18,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD19,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 41-43 SPI0
  { PIOD, PIO_PD20B_SPI0_MISO, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD21B_SPI0_MOSI, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD22B_SPI0_SPCK, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PD23 is connected to RAM on the test board
  //{ PIOD, PIO_PD23,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 44-48
  { PIOD, PIO_PD24,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD25,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD26,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD27,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD28,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PD29 is connected to RAM on the test board
  //{ PIOD, PIO_PD29,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 49-50
  { PIOD, PIO_PD30,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD31,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO E

  // PE0-5 are connected to RAM on the test board
  //{ PIOE, PIO_PE0,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOE, PIO_PE1,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOE, PIO_PE2,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOE, PIO_PE3,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOE, PIO_PE4,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  //{ PIOE, PIO_PE5,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // OTHER

  // 51-52 HSMCI
  { PIOA, PIO_PA25D_MCCK,      ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCK
  { PIOA, PIO_PA28C_MCCDA | PIO_PA30C_MCDA0 | PIO_PA31C_MCDA1 | PIO_PA26C_MCDA2 | PIO_PA27C_MCDA3,
		  	  	  	  	  	   ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCDA, MCDA0-3

  // 53-55 GMAC PHY
  { PIOA, PIO_PA14,            ID_PIOA, PIO_INPUT,    PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY INTERRUPT
  { PIOC, PIO_PC10,            ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY RESET (active low)
  { PIOD, (PIO_PD0A_GTXCK | PIO_PD1A_GTXEN | PIO_PD2A_GTX0 | PIO_PD3A_GTX1 | PIO_PD4A_GRXDV | PIO_PD5A_GRX0 | PIO_PD6A_GRX1 | PIO_PD7A_GRXER | PIO_PD8A_GMDC | PIO_PD9A_GMDIO),
		  	  	  	  	  	   ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY ETHERNET

  // 56 UART0 all pins
  { PIOA, PIO_PA9A_URXD0|PIO_PA10A_UTXD0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },

  // 57 UART1 all pins
  { PIOA, PIO_PA5C_URXD1|PIO_PA6C_UTXD1,  ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }

#if 0
  // xx - TWI0 all pins
  { PIOA, PIO_PA3A_TWD0|PIO_PA4A_TWCK0,   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
#endif
};

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;
RingBuffer rx_buffer2;
RingBuffer tx_buffer2;

UARTClass Serial(UART0, UART0_IRQn, ID_UART0, &rx_buffer1, &tx_buffer1);

void UART0_Handler(void)
{
  Serial.IrqHandler();
}

UARTClass Serial1(UART1, UART1_IRQn, ID_UART1, &rx_buffer2, &tx_buffer2);

void UART1_Handler(void)
{
  Serial1.IrqHandler();
}

// ----------------------------------------------------------------------------

extern "C" void __libc_init_array(void);
extern void UrgentInit();

void ConfigurePin(const PinDescription& pinDesc)
{
	pio_configure(pinDesc.pPort, pinDesc.ulPinType, pinDesc.ulPin, pinDesc.ulPinConfiguration);
}

extern "C" void init( void )
{
	SystemInit();

	// Set Systick to 1ms interval, common to all SAME70 variants
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		// Capture error
		while (true);
	}

	UrgentInit();			// initialise anything in the main application that can't wait

	// Initialize C library (this calls C++ constructors for static data too)
	__libc_init_array();

	// We no longer disable pullups on all pins here, better to leave them enabled until the port is initialised

	// Initialize Serial port U(S)ART pins
	ConfigurePin(g_APinDescription[APINS_UART0]);
	setPullup(APIN_UART0_RXD, true); 							// Enable pullup for RX0

	// No need to initialize the USB pins on the SAM4E because they are USB by default

	// Initialize Analog Controller
	AnalogInInit();

	// Initialize analogOutput module
	AnalogOutInit();

	// Initialize HSMCI pins
	ConfigurePin(g_APinDescription[APIN_HSMCI_CLOCK]);
	ConfigurePin(g_APinDescription[APIN_HSMCI_DATA]);

	// Initialize Ethernet pins
	ConfigurePin(g_APinDescription[APIN_GMAC_PHY_RESET]);
	ConfigurePin(g_APinDescription[APIN_GMAC_PHY_INTERRUPT]);
	ConfigurePin(g_APinDescription[APINS_GMAC_PHY]);

	// Initialize TRNG
	pmc_enable_periph_clk(ID_TRNG);
	TRNG->TRNG_IDR = TRNG_IDR_DATRDY;							// Disable all interrupts
	TRNG->TRNG_CR = TRNG_CR_KEY(0x524e47) | TRNG_CR_ENABLE;		// Enable TRNG with security key (required)
}

// End
