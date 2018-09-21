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
#include "matrix/matrix.h"

/* For the SAME70Q20/21 we use a logical pin numbering scheme:
 * Pins   0-31  PA0-31
 * Pins  32-63  PB0-31
 * Pins  64-95  PC0-31
 * Pins  96-127 PD0-31
 * Pins  128-133  PE0-5
 */

/*
 * Pin descriptions
 */
extern const PinDescription g_APinDescription[] =
{
  // PIO A

  // 0-2
  { PIOA, PIO_PA0,			   ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA1,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA2,             ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 3-4 TWI0, connected to EEPROM on the XPLD
  { PIOA, PIO_PA3,		       ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                    NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA4,		       ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                    NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 5-6 UART1
#ifdef SAME70XPLD
  { PIOA, PIO_PA5C_URXD1,      ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD1
  { PIOA, PIO_PA6C_UTXD1,      ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD1
#else
  { PIOA, PIO_PA5,		       ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA6,		       ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#endif

  // 7-8  connected to XTAL32 on the XPLD
  { PIOA, PIO_PA7B_PWMC0_PWMH3, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, PWM_CH3,	 NOT_ON_TIMER },
  { PIOA, PIO_PA8A_PWMC1_PWMH3, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, PWM_CH7,	 NOT_ON_TIMER },

  // 9-10 UART0
#ifdef SAME70XPLD
  { PIOA, PIO_PA9A_URXD0,      ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD0
  { PIOA, PIO_PA10A_UTXD0,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD0
#else
  { PIOA, PIO_PA9,		       ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Driver ENN
  { PIOA, PIO_PA10,		       ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PS_ON
#endif

  // 11-13
  // PA11 is connected to USER SWITCH on the XPLD
  { PIOA, PIO_PA11B_PWMC0_PWMH0, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, PWM_CH0,	 NOT_ON_TIMER },
  { PIOA, PIO_PA12C_PWMC1_PWMH0, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, PWM_CH4,     NOT_ON_TIMER },
  { PIOA, PIO_PA13A_QIO0,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 14
#ifdef SAME70XPLD
  { PIOA, PIO_PA14,            ID_PIOA, PIO_INPUT,    PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY INTERRUPT
#else
  { PIOA, PIO_PA14A_QSCK,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#endif

  // PA15-16 are connected to RAM on the XPLD
  { PIOA, PIO_PA15B_TIOA1,     ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  TC0_CHA1 },
  { PIOA, PIO_PA16C_PWMC0_PWML2, ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                 NO_ADC, PWM_CH2,	 NOT_ON_TIMER },

  // 17-18 (ADC)
  { PIOA, PIO_PA17,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC6,   NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA18,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC7,   NOT_ON_PWM,  NOT_ON_TIMER },

  // 19 (ESP transfer request)
  { PIOA, PIO_PA19,            ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC8,  NOT_ON_PWM,  NOT_ON_TIMER },

  // 20 PA20 (connected to RAM on the xpld)
  { PIOA, PIO_PA20,            ID_PIOA, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC9,  NOT_ON_PWM,  NOT_ON_TIMER },

  // 21-24
  { PIOA, PIO_PA21A_RXD1,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC1,   NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA22,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA23A_SCK1,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA24B_PWMC0_PWMH1, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, PWM_CH1,	 NOT_ON_TIMER },

  // PA25-28 are used for HSMCI
  { PIOA, PIO_PA25,            ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA26,            ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA27,            ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA28,            ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 29 PA29
  { PIOA, PIO_PA29,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PA30-31 are used for HSMCI
  { PIOA, PIO_PA30,            ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOA, PIO_PA31,            ID_PIOA, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO B

  // 32-33 USART0 (shared SPI)
  { PIOB, PIO_PB0C_RXD0,       ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, PIO_PB1C_TXD0,       ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 34 NPCS0
  { PIOB, PIO_PB2D_SPI0_NPCS0, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ESP CS

  // 35 PB03
  { PIOB, PIO_PB3,             ID_PIOB, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC2,   NOT_ON_PWM,  NOT_ON_TIMER },

  // 36-37 (TWI1?)
  { PIOB, PIO_PB4D_TXD1,       ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, PIO_PB5,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 38-39
  { PIOB, PIO_PB6,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SWDIO
  { PIOB, PIO_PB7,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SWCLK

  // 40-44
  // PB8-PB9 are not available (Crystal)
  // PB10-PB11 are not present on chip
  // PB12 is not available (Erase)
  { PIOB, 0,	           	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,          		   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 45 PB13 USART0 clock
  { PIOB, PIO_PB13C_SCK0,      ID_PIOB, PIO_PERIPH_C,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 46-63 no present on chip
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOB, 0,            	   ID_PIOB, PIO_NOT_A_PIN, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO C

  // 64-71 PC0-7 (connected to RAM on the XPLD)
  { PIOC, PIO_PC0,             ID_PIOC, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC25,  NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC1,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC2,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC3,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC4,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC5B_TIOA6,      ID_PIOC, PIO_PERIPH_B,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  TC2_CHA6 },
#ifdef SAME70XPLD
  { PIOC, PIO_PC6,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#else
  { PIOC, PIO_PC6,             ID_PIOC, PIO_INPUT,	   PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY_INTERRUPT on Duet 3
#endif
  { PIOC, PIO_PC7,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 72-73 PC8-9
  // PC8 is connected to USER LED on the XPLD (active low)
  { PIOC, PIO_PC8B_TIOA7,      ID_PIOC, PIO_PERIPH_B,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  TC2_CHA7 },
  { PIOC, PIO_PC9,             ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 74-75 PC10-11 (connected to EEPROM on the XPLD)
#ifdef SAME70XPLD
  { PIOC, PIO_PC10,            ID_PIOC, PIO_OUTPUT_1,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY RESET (active low)
#else
  { PIOC, PIO_PC10,            ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#endif
  { PIOC, PIO_PC11B_TIOA8,     ID_PIOC, PIO_PERIPH_B,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  TC2_CHA8 },

  // 76-79 PC12-15 (PC15 is connected to RAM on the XPLD)
#ifdef SAME70XPLD
  { PIOC, PIO_PC12,            ID_PIOC, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC19,  NOT_ON_PWM,  NOT_ON_TIMER },
#else
  { PIOC, PIO_PC12C_CANRX1,    ID_PIOC, PIO_PERIPH_C,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#endif
  { PIOC, PIO_PC13,            ID_PIOC, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC17,  NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC14,            ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC15,            ID_PIOC, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC18,  NOT_ON_PWM,  NOT_ON_TIMER },

  // 80
  { PIOC, PIO_PC16,            ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI CD on the XPLD

  // 81-82 PC17-18 (PC18 is connected to RAM on the XPLD)
  { PIOC, PIO_PC17,            ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC18,            ID_PIOC, PIO_OUTPUT_0,  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 83 PC19
  { PIOC, PIO_PC19,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 84-93 PC20-29 (connected to RAM on the XPLD)
  { PIOC, PIO_PC20,            ID_PIOC, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC21,            ID_PIOC, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC22,            ID_PIOC, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC23B_TIOA3,     ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  TC1_CHA3 },
  { PIOC, PIO_PC24C_SPI1_SPCK, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC25C_SPI1_NPCS0, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC26C_SPI1_MISO, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC27C_SPI1_MOSI, ID_PIOC, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC28,            ID_PIOC, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC29B_TIOA5,     ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC20,  NOT_ON_PWM,  TC1_CHA5 },

  // 94-95
  { PIOC, PIO_PC30,            ID_PIOC, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC21,  NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOC, PIO_PC31,            ID_PIOC, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC22,  NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO D

  // 96-105 PD0-9 are connected to the Ethernet PHY
  { PIOD, PIO_PD0,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD1,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD2,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD3,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD4,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD5,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD6,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD7,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD8,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD9,             ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 106-108
  { PIOD, PIO_PD10,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#ifdef SAME70XPLD
  { PIOD, PIO_PD11,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD12,			   ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#else
  { PIOD, PIO_PD11,            ID_PIOD, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY_RESET on Duet 3
  { PIOD, PIO_PD12B_CANTX1,    ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // CANTX1
#endif

  // 109-113 PD13-17 (connected to RAM on the XPLD)
  { PIOD, PIO_PD13,            ID_PIOD, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD14,            ID_PIOD, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD15,            ID_PIOD, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD16,            ID_PIOD, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD17,            ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 114-115
#ifdef SAME70XPLD
  { PIOD, PIO_PD18,			   ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD19,			   ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#else
  { PIOD, PIO_PD18C_URXD4,     ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UART 4
  { PIOD, PIO_PD19C_UTXD4,     ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UART 4
#endif

  // 116-118 SPI0
  { PIOD, PIO_PD20B_SPI0_MISO, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD21B_SPI0_MOSI, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD22B_SPI0_SPCK, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 119 PD23 (connected to RAM on the XPLD)
  { PIOD, PIO_PD23,            ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 120-124
  { PIOD, PIO_PD24,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#ifdef SAME70XPLD
  { PIOD, PIO_PD25,		       ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD26,			   ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#else
  { PIOD, PIO_PD25C_URXD2,     ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UART2
  { PIOD, PIO_PD26C_UTXD2,     ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UART2
#endif

  { PIOD, PIO_PD27,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#ifdef SAME70XPLD
  { PIOD, PIO_PD28,			   ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
#else
  { PIOD, PIO_PD28,			   ID_PIOD, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // connected to CANRX1 on prototype
#endif

  // 125 PD29 (connected to RAM on the XPLD)
  { PIOD, PIO_PD29,            ID_PIOD, PIO_NOT_A_PIN, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // 126-127
  { PIOD, PIO_PD30,            ID_PIOD, PIO_INPUT,	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC0, 	NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOD, PIO_PD31,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },

  // PIO E

  // 128-133 PE0-5 (connected to RAM on the XPLD)
  { PIOE, PIO_PE0,             ID_PIOE, PIO_INPUT,	 PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC27,  NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOE, PIO_PE1B_TIOB9,      ID_PIOE, PIO_PERIPH_B, PIO_DEFAULT, PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  TC3_CHA10 },
  { PIOE, PIO_PE2,             ID_PIOE, PIO_INPUT,	 PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOE, PIO_PE3,             ID_PIOE, PIO_INPUT,	 PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC26,  NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOE, PIO_PE4,             ID_PIOE, PIO_INPUT,	 PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC4,   NOT_ON_PWM,  NOT_ON_TIMER },
  { PIOE, PIO_PE5,             ID_PIOE, PIO_INPUT,	 PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC3,   NOT_ON_PWM,  NOT_ON_TIMER },

  // Combinations

  // 134-135 HSMCI all pins
  { PIOA, PIO_PA25D_MCCK,      ID_PIOA, PIO_PERIPH_D, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCK
  { PIOA, PIO_PA28C_MCCDA | PIO_PA30C_MCDA0 | PIO_PA31C_MCDA1 | PIO_PA26C_MCDA2 | PIO_PA27C_MCDA3,
		  	  	  	  	  	   ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCDA, MCDA0-3

  // 136 GMAC PHY
  { PIOD, (PIO_PD0A_GTXCK | PIO_PD1A_GTXEN | PIO_PD2A_GTX0 | PIO_PD3A_GTX1 | PIO_PD4A_GRXDV | PIO_PD5A_GRX0 | PIO_PD6A_GRX1 | PIO_PD7A_GRXER | PIO_PD8A_GMDC | PIO_PD9A_GMDIO),
		  	  	  	  	  	   ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PHY ETHERNET

  // 137 UART0 or 2 all pins
#ifdef SAME70XPLD
  { PIOA, PIO_PA9A_URXD0|PIO_PA10A_UTXD0,  ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
#else
  { PIOD, PIO_PD25C_URXD2|PIO_PD26C_UTXD2, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
#endif

  // 138 UART1 or 4 all pins
#ifdef SAME70XPLD
  { PIOA, PIO_PA5C_URXD1|PIO_PA6C_UTXD1,   ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
#else
  { PIOD, PIO_PD18C_URXD4|PIO_PD19C_UTXD4, ID_PIOD, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }
#endif
};

static_assert(ARRAY_SIZE(g_APinDescription) == 139, "incorrect pin count");		// check on pin numbering

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;
RingBuffer rx_buffer2;
RingBuffer tx_buffer2;

#ifdef SAME70XPLD

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

#else

UARTClass Serial(UART2, UART2_IRQn, ID_UART2, &rx_buffer1, &tx_buffer1);

void UART2_Handler(void)
{
	Serial.IrqHandler();
}

UARTClass Serial1(UART4, UART4_IRQn, ID_UART4, &rx_buffer2, &tx_buffer2);

void UART4_Handler(void)
{
	Serial1.IrqHandler();
}

#endif

// ----------------------------------------------------------------------------

void ConfigurePin(const PinDescription& pinDesc)
{
	pio_configure(pinDesc.pPort, pinDesc.ulPinType, pinDesc.ulPin, pinDesc.ulPinConfiguration);
}

extern "C" void init( void )
{
	// Initialize Serial port U(S)ART pins
	ConfigurePin(g_APinDescription[APINS_Serial0]);
	setPullup(APIN_Serial0_RXD, true); 							// Enable pullup for RX0

	// No need to initialize the USB pins on the SAME70 because they are USB by default

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

#ifndef SAME70XPLD
	// Set up PB4..PB5 as normal I/O, not JTAG
	matrix_set_system_io(CCFG_SYSIO_SYSIO4 | CCFG_SYSIO_SYSIO5);
	// Set up PB4..PB7 as normal I/O, not JTAG/SWDEBUG
//	matrix_set_system_io(CCFG_SYSIO_SYSIO4 | CCFG_SYSIO_SYSIO5 | CCFG_SYSIO_SYSIO6 | CCFG_SYSIO_SYSIO7);
#endif
}

// End
