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

/* For the Duet NG we use a logical pin numbering scheme:
 * Pins   0-25  PA0-25
 * Pins  26-31  PB0-3,13,14
 * Pins  32-63  PC0-31
 * Pins  64-95  PD0-31
 * Pins  96-101 PE0-5
 * Pin  102     PB6 (this got added late)
 * Pins 103-107 Various composite devices
 * Pins 200-215	IO0-IO15 on the SX1509B port expander on the DueXn
 */

/*
 * Pins descriptions
 */
extern const PinDescription g_APinDescription[]=
{
  // PIO A
  // Pins 0-25 are PA0-PA25

  // 0-2
  { PIOA, PIO_PA0B_TIOA0,      ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),	NO_ADC, NOT_ON_PWM,  TC0_CHA0	  }, // Fan 2
  { PIOA, PIO_PA1,		       ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                	NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E0_Dir
  { PIOA, PIO_PA2,			   ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Y stop

  // 3-4 TWI
  { PIOA, PIO_PA3A_TWD0,   	   ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,				 	NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWD0
  { PIOA, PIO_PA4A_TWCK0,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWCK0

  // 5-6 UART 1
  { PIOA, PIO_PA5C_URXD1,      ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD1
  { PIOA, PIO_PA6C_UTXD1,      ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD1

  // 7-8
  { PIOA, PIO_PA7,			   ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SD/LCD/Encoder
  { PIOA, PIO_PA8,   		   ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SD/LCD/Encoder

  // 9-10 UART 0
  { PIOA, PIO_PA9A_URXD0,      ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // URXD0 PanelDue Dout
  { PIOA, PIO_PA10A_UTXD0,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL, 					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD0 PanelDue Din

  // 11-14 high speed SPI
  { PIOA, PIO_PA11A_NPCS0,     ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS0
  { PIOA, PIO_PA12A_MISO,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MISO
  { PIOA, PIO_PA13A_MOSI,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MOSI
  { PIOA, PIO_PA14A_SPCK,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                 	NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPCK

  // 15-20 general I/O
  { PIOA, PIO_PA15B_TIOA1,     ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),   NO_ADC, NOT_ON_PWM,	 TC0_CHA1	  }, // Heater 7
  { PIOA, PIO_PA16C_PWML2,     ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   	NO_ADC, PWM_CH2,	 NOT_ON_TIMER }, // Heater 2
  { PIOA, PIO_PA17X1_AFE0_AD0, ID_PIOA, PIO_INPUT, 	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                  	ADC0,   NOT_ON_PWM,  NOT_ON_TIMER }, // PA17/ADC0
  { PIOA, PIO_PA18X1_AFE0_AD1, ID_PIOA, PIO_INPUT, 	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC1,   NOT_ON_PWM,  NOT_ON_TIMER }, // PA18/ADC1
  { PIOA, PIO_PA19B_PWML0,     ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),     NO_ADC, PWM_CH0,  	 NOT_ON_TIMER }, // Heater 0
  { PIOA, PIO_PA20B_PWML1,     ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),     NO_ADC, PWM_CH1,	 NOT_ON_TIMER }, // Heater 1

  // 21-23 SPI bus 1
  { PIOA, PIO_PA21A_RXD1,      ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI bus 1 MISO
  { PIOA, PIO_PA22A_TXD1,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI bus 1 MOSI
  { PIOA, PIO_PA23A_SCK1,      ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI bus 1 SPCK

  // 24-25
  { PIOA, PIO_PA24,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI bus 0 CS5
  { PIOA, PIO_PA25,            ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI bus 0 CS6

  // PA26-31 are used by HSMCI

  // PIO B
  // Pins 26-29 are PB0-PB3
  // PB4-PB12 not available (JTAG, crystal, USB, Erase)
  // Pins 30-31 are PB13-PB14
  // PB15-31 not present on chip

  // 26-27 SPI bus 0
  { PIOB, PIO_PB0C_RXD0,       ID_PIOB, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 MISO
  { PIOB, PIO_PB1C_TXD0,       ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 MOSI

  // 28-29
  { PIOB, PIO_PB2,             ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 CS1
  { PIOB, PIO_PB3X1_AFE1_AD1,  ID_PIOB, PIO_INPUT, 	  PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC17,  NOT_ON_PWM,  NOT_ON_TIMER }, // Power fail detect 5V regulator input

  // 30 SPI bus 0
  { PIOB, PIO_PB13C_SCK0,      ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 SPCK

  // 31 DAC 1
  { PIOB, PIO_PB14X1_DAC1,     ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DAC,	                    DA1,    NOT_ON_PWM,  NOT_ON_TIMER }, // DAC 1 (E1 motor current)

  // PIO C
  // Pins 32-63 are PC0-PC31

  // 32-39
  { PIOC, PIO_PC0,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   ADC14, 	NOT_ON_PWM,  NOT_ON_TIMER }, // PC0/AFE0_AD14 used for E6_DIR
  { PIOC, PIO_PC1X1_AFE1_AD4,  ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC20,  NOT_ON_PWM,  NOT_ON_TIMER }, // Z probe analog in
  { PIOC, PIO_PC2,  		   ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z probe mod and LED
  { PIOC, PIO_PC3B_PWML3,  	   ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),     NO_ADC, PWM_CH3,  	 NOT_ON_TIMER }, // Heater 3
  { PIOC, PIO_PC4X1_AFE1_AD7,  ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC23,  NOT_ON_PWM,  NOT_ON_TIMER }, // Vin power fail detect
  { PIOC, PIO_PC5B_TIOA6,      ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),   NO_ADC, NOT_ON_PWM,  TC2_CHA6     }, // Heater 4
  { PIOC, PIO_PC6,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TMC2660 EN
  { PIOC, PIO_PC7,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ENC_B

  // 40-47
  { PIOC, PIO_PC8B_TIOA7,      ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),   NO_ADC, NOT_ON_PWM,  TC2_CHA7     }, // Heater 5
  { PIOC, PIO_PC9,             ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Y en
  { PIOC, PIO_PC10,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z en
  { PIOC, PIO_PC11B_TIOA8,     ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),   NO_ADC, NOT_ON_PWM,  TC2_CHA8     }, // Heater 6
  { PIOC, PIO_PC12X1_AFE0_AD8, ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC8,   NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 2
  { PIOC, PIO_PC13X1_AFE0_AD6, ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC6,   NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 0
  { PIOC, PIO_PC14,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X endstop
  { PIOC, PIO_PC15X1_AFE0_AD7, ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC7,   NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 1

  // 48-55
  { PIOC, PIO_PC16, 		   ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 endstop
  { PIOC, PIO_PC17,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E0 EN
  { PIOC, PIO_PC18,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 CS2
  { PIOC, PIO_PC19,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 CS3
  { PIOC, PIO_PC20,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 CS4
  { PIOC, PIO_PC21,            ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI CD
  { PIOC, PIO_PC22,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Unused!
  { PIOC, PIO_PC23B_TIOA3,     ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),   NO_ADC, NOT_ON_PWM,  TC1_CHA3     }, // Fan 0

  // 56-63
  { PIOC, PIO_PC24,            ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0 CS0
  { PIOC, PIO_PC25,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 EN
  { PIOC, PIO_PC26B_TIOA4,     ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),   NO_ADC, NOT_ON_PWM,  TC1_CHA4     }, // Fan 1
  { PIOC, PIO_PC27X1_AFE0_AD13,ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC13,  NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 6
  { PIOC, PIO_PC28,            ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD RS
  { PIOC, PIO_PC29X1_AFE0_AD9, ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC9,   NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 3
  { PIOC, PIO_PC30X1_AFE0_AD10,ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC10,  NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 4
  { PIOC, PIO_PC31X1_AFE0_AD11,ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,                    ADC11,  NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 5

  // PIO D
  // Pins 64-95 are PD0-PD31

  // 64-71
  { PIOD, PIO_PD0,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E4 step
  { PIOD, PIO_PD1,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 step
  { PIOD, PIO_PD2,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E2 step
  { PIOD, PIO_PD3,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E5 step
  { PIOD, PIO_PD4,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 step
  { PIOD, PIO_PD5,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E0 step
  { PIOD, PIO_PD6,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X step
  { PIOD, PIO_PD7,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Y step

  // 72-79
  { PIOD, PIO_PD8,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z step
  { PIOD, PIO_PD9,             ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 dir
  { PIOD, PIO_PD10, 		   ID_PIOD, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E0 endstop
  { PIOD, PIO_PD11,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X dir
  { PIOD, PIO_PD12,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Y dir
  { PIOD, PIO_PD13,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z dir
  { PIOD, PIO_PD14,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X en
  { PIOD, PIO_PD15,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PS On

  // 80-87
  { PIOD, PIO_PD16,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E4 dir
  { PIOD, PIO_PD17,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E5 dir
  { PIOD, PIO_PD18,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD DB7
  { PIOD, PIO_PD19,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD DB6
  { PIOD, PIO_PD20,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD DB5
  { PIOD, PIO_PD21,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD DB4
  { PIOD, PIO_PD22,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 dir
  { PIOD, PIO_PD23,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E2 en

  // 88-95
  { PIOD, PIO_PD24,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 en
  { PIOD, PIO_PD25,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E4 en
  { PIOD, PIO_PD26,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E5 en
  { PIOD, PIO_PD27,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Expansion PD27
  { PIOD, PIO_PD28,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E2 dir
  { PIOD, PIO_PD29, 		   ID_PIOD, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z endstop
  { PIOD, PIO_PD30,            ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SAM transfer Ready
  { PIOD, PIO_PD31, 		   ID_PIOD, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ESP data ready

  // PIO E
  // Pins 96-101 are PE0-5
  { PIOE, PIO_PE0, 		   	   ID_PIOE, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E2 endstop
  { PIOE, PIO_PE1, 		   	   ID_PIOE, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 endstop
  { PIOE, PIO_PE2, 		   	   ID_PIOE, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E4 endstop
  { PIOE, PIO_PE3, 		   	   ID_PIOE, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E5 endstop
  { PIOE, PIO_PE4,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ESP NRST
  { PIOE, PIO_PE5,             ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ESP PD/EN

  // 102 PB6 - was thought to be dedicated to JTAG but is also available on the expansion connector
  { PIOB, PIO_PB6,				ID_PIOB, PIO_INPUT,	  PIO_DEFAULT,	PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,	 NOT_ON_TIMER }, // expansion

  // 103-104 HSMCI
  { PIOA, PIO_PA29C_MCCK,      ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCK
  { PIOA, PIO_PA28C_MCCDA | PIO_PA30C_MCDA0 | PIO_PA31C_MCDA1 | PIO_PA26C_MCDA2 | PIO_PA27C_MCDA3,
		  	  	  	  	  	   ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCDA, MCDA0-3

  // 105 - TWI0 all pins
  { PIOA, PIO_PA3A_TWD0|PIO_PA4A_TWCK0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 106 - UART0 all pins
  { PIOA, PIO_PA9A_URXD0|PIO_PA10A_UTXD0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 107 - UART1 all pins
  { PIOA, PIO_PA5C_URXD1|PIO_PA6C_UTXD1, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }
};

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;
RingBuffer rx_buffer2;
RingBuffer tx_buffer2;

UARTClass Serial(UART0, UART0_IRQn, ID_UART0, &rx_buffer1, &tx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }

UARTClass Serial1(UART1, UART1_IRQn, ID_UART1, &rx_buffer2, &tx_buffer2);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }

// IT handlers
void UART0_Handler(void)
{
  Serial.IrqHandler();
}

// IT handlers
void UART1_Handler(void)
{
  Serial1.IrqHandler();
}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
  if (Serial1.available()) serialEvent1();
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

	// Set Systick to 1ms interval, common to all SAM3 variants
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		// Capture error
		while (true);
	}

	UrgentInit();			// initialise anything in the main application that can't wait

	// Initialize C library (I think this calls C++ constructors for static data too)
	__libc_init_array();

	// We no longer disable pullups on all pins here, better to leave them enabled until the port is initialised

	// Initialize Serial port U(S)ART pins
	ConfigurePin(g_APinDescription[APINS_UART0]);
	setPullup(APIN_UART0_RXD, true); 							// Enable pullup for RX0
	ConfigurePin(g_APinDescription[APINS_UART1]);
	setPullup(APIN_UART1_RXD, true); 							// Enable pullup for RX1

	// No need to initialize the USB pins on the SAM4E because they are USB by default

	// Initialize Analog Controller
	AnalogInInit();

	// Initialize analogOutput module
	AnalogOutInit();

	// Initialize HSMCI pins
	ConfigurePin(g_APinDescription[APIN_HSMCI_CLOCK]);
	ConfigurePin(g_APinDescription[APINS_HSMCI_DATA]);

	// Start the USB
	udc_start();
}

// End
