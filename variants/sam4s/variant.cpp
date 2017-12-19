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
  { PIOA, PIO_PA0B_TIOA0,		ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),	NO_ADC, NOT_ON_PWM,  TC0_CHA0	  }, // PS_ON
  { PIOA, PIO_PA1,				ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                	NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ENN to all stepper drivers
  { PIOA, PIO_PA2B_SCK0,		ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SCK0 (daughter boards, external SD card)

  // 3-4 TWI
  { PIOA, PIO_PA3A_TWD0,		ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,  PIN_ATTR_DIGITAL,				 	NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWD0 (expansion)
  { PIOA, PIO_PA4A_TWCK0,		ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TWCK0 (expansion)

  // 5-6 USART
  { PIOA, PIO_PA5A_RXD0,		ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // RXD0 (daughter boards, external SD card)
  { PIOA, PIO_PA6A_TXD0,		ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL, 	  				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // TXD0 (daughter boards, external SD card)

  // 7-8
  { PIOA, PIO_PA7,				ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD ENC_SW
  { PIOA, PIO_PA8,				ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Y dir

  // 9-10 UART 0
  { PIOA, PIO_PA9A_URXD0,		ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,   				NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Stepper drivers UART
  { PIOA, PIO_PA10A_UTXD0,		ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL, 					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Stepper drivers UART

  // 11-14 high speed SPI
  { PIOA, PIO_PA11A_NPCS0,		ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // NPCS0 (W5500)
  { PIOA, PIO_PA12A_MISO,		ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MISO (W5500)
  { PIOA, PIO_PA13A_MOSI,		ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MOSI (W5500)
  { PIOA, PIO_PA14A_SPCK,		ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                 	NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPCK (W5500)

  // 15-20 general I/O
  { PIOA, PIO_PA15B_TIOA1,		ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),	NO_ADC, NOT_ON_PWM,	 TC0_CHA1	  }, // LCD beep
  { PIOA, PIO_PA16C_PWML2,		ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),   	NO_ADC, PWM_CH2,	 NOT_ON_TIMER }, // Heater 2
  { PIOA, PIO_PA17X1_AD0, 		ID_PIOA, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC0,   NOT_ON_PWM,  NOT_ON_TIMER }, // VREF_MON
  { PIOA, PIO_PA18X1_AD1,		ID_PIOA, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC1,   NOT_ON_PWM,  NOT_ON_TIMER }, // E2 dir
  { PIOA, PIO_PA19B_PWML0,		ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),	NO_ADC, PWM_CH0,  	 NOT_ON_TIMER }, // VSSA_MON
  { PIOA, PIO_PA20B_PWML1,		ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),	NO_ADC, PWM_CH1,	 NOT_ON_TIMER }, // Thermistor 0

  // 21-23 SPI bus 1
  { PIOA, PIO_PA21A_RXD1,		ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_ANALOG,					ADC8,   NOT_ON_PWM,  NOT_ON_TIMER }, // Analogue, digital or UART expansion
  { PIOA, PIO_PA22A_TXD1,		ID_PIOA, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_ANALOG,					ADC9,   NOT_ON_PWM,  NOT_ON_TIMER }, // Analogue, digital or UART expansion
  { PIOA, PIO_PA23,				ID_PIOA, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // W5500 interrupt

  // 24-25
  { PIOA, PIO_PA24,				ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X stop
  { PIOA, PIO_PA25,				ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E0 stop

  // PA26-31 are used by HSMCI

  // PIO B
  // Pins 26-33 are PB0-PB7
  // PB8-PB12 not available (crystal, USB, Erase)

  // 26-33
  { PIOB, PIO_PB0X1_AD4,		ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC4,   NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 1
  { PIOB, PIO_PB1X1_AD5,		ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC5,   NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 3
  { PIOB, PIO_PB2A_URXD1,		ID_PIOB, PIO_PERIPH_A, PIO_PULLUP,   PIN_ATTR_DIGITAL,					ADC6,   NOT_ON_PWM,  NOT_ON_TIMER }, // URXD0 PanelDue Dout
  { PIOB, PIO_PB3A_UTXD1,		ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					ADC7,   NOT_ON_PWM,  NOT_ON_TIMER }, // UTXD0 PanelDue Din
  { PIOB, PIO_PB4,      		ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z dir
  { PIOB, PIO_PB5,		  		ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,	 NOT_ON_TIMER }, // LCD ENC_A
  { PIOB, PIO_PB6,      		ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Y stop
  { PIOB, PIO_PB7,		  		ID_PIOB, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,	 NOT_ON_TIMER }, // E0 dir

  // Pins 34-35 are PB13-PB14
  { PIOB, PIO_PB13X1_DAC0,		ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DAC,	                    DA0,    NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0_CS0 (external SD card)
  { PIOB, PIO_PB14X1_DAC1,		ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DAC,	                    DA1,    NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0_CS1 (daughter boards)

  // PB15-31 not present on chip

  // PIO C
  // Pins 36-67 are PC0-PC31

  // 36-43
  { PIOC, PIO_PC0B_PWML0,		ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),	NO_ADC, PWM_CH0,  	 NOT_ON_TIMER }, // Heater 0
  { PIOC, PIO_PC1B_PWML1,		ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),	NO_ADC, PWM_CH1,  	 NOT_ON_TIMER }, // Heater 1
  { PIOC, PIO_PC2B_PWML2,		ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),	NO_ADC, PWM_CH2,  	 NOT_ON_TIMER }, // Y step
  { PIOC, PIO_PC3,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // ENC_B
  { PIOC, PIO_PC4,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                  NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E0 step
  { PIOC, PIO_PC5,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 step
  { PIOC, PIO_PC6,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 dir
  { PIOC, PIO_PC7,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E1 stop

  // 44-51
  { PIOC, PIO_PC8,				ID_PIOC, PIO_INPUT,    PIO_PULLUP,	 PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SD card detect
  { PIOC, PIO_PC9,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // LCD CS
  { PIOC, PIO_PC10,				ID_PIOC, PIO_INPUT,	   PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z stop
  { PIOC, PIO_PC11,				ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // USB Vbus monitor
  { PIOC, PIO_PC12X1_AD12,		ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC12,  NOT_ON_PWM,  NOT_ON_TIMER }, // VIN voltage monitor
  { PIOC, PIO_PC13,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // W5500 reset
  { PIOC, PIO_PC14,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MUX0
  { PIOC, PIO_PC15X1_AD11,		ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC11,  NOT_ON_PWM,  NOT_ON_TIMER }, // Z probe input

  // 52-59
  { PIOC, PIO_PC16,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MUX1
  { PIOC, PIO_PC17,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // MUX2
  { PIOC, PIO_PC18,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X dir
  { PIOC, PIO_PC19,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // SPI0_CS2
  { PIOC, PIO_PC20,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // X step
  { PIOC, PIO_PC21,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 step
  { PIOC, PIO_PC22B_PWML3,		ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM),	NO_ADC, PWM_CH3,  	 NOT_ON_TIMER }, // Fan 1
  { PIOC, PIO_PC23B_TIOA3,		ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),	NO_ADC, NOT_ON_PWM,  TC1_CHA3     }, // Fan 0

  // 60-67
  { PIOC, PIO_PC24,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 dir
  { PIOC, PIO_PC25,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E3 en
  { PIOC, PIO_PC26B_TIOA4,		ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),	NO_ADC, NOT_ON_PWM,  TC1_CHA4     }, // Z probe mod/servo/diag LED
  { PIOC, PIO_PC27,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E2 en
  { PIOC, PIO_PC28,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // Z step
  { PIOC, PIO_PC29B_TIOA5,		ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_TIMER),	NO_ADC, NOT_ON_PWM,  TC1_CHA5	  }, // Fan 2
  { PIOC, PIO_PC30X1_AD14,		ID_PIOC, PIO_INPUT,    PIO_DEFAULT,  PIN_ATTR_ANALOG,					ADC14,  NOT_ON_PWM,  NOT_ON_TIMER }, // Thermistor 2
  { PIOC, PIO_PC31,				ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT,  PIN_ATTR_DIGITAL,					NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // E2 step

  // 68-69 HSMCI
  { PIOA, PIO_PA29C_MCCK,      ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT,  PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCK
  { PIOA, PIO_PA28C_MCCDA | PIO_PA30C_MCDA0 | PIO_PA31C_MCDA1 | PIO_PA26C_MCDA2 | PIO_PA27C_MCDA3,
		  	  	  	  	  	   ID_PIOA, PIO_PERIPH_C, PIO_PULLUP,   PIN_ATTR_DIGITAL,                   NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // HSMCI MCCDA, MCDA0-3

  // 70 - TWI0 all pins
  { PIOA, PIO_PA3A_TWD0|PIO_PA4A_TWCK0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 71 - UART0 all pins
  { PIOA, PIO_PA9A_URXD0|PIO_PA10A_UTXD0, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER },
  // 72 - UART1 all pins
  { PIOA, PIO_PB2A_URXD1|PIO_PB3A_UTXD1, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT, (PIN_ATTR_DIGITAL|PIN_ATTR_COMBO), NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }
};

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;
RingBuffer rx_buffer2;
RingBuffer tx_buffer2;

// UART0 is used to control the stepper drivers. We don't use the core support for this.

// UART1 is used to interface with PanelDue
UARTClass Serial(UART1, UART1_IRQn, ID_UART1, &rx_buffer2, &tx_buffer2);

void UART1_Handler(void)
{
  Serial.IrqHandler();
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

	// Set Systick to 1ms interval, common to all SAM4 variants
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

	// No need to initialize the USB pins on the SAM4E because they are USB by default

	// Initialize Analog Controller
	AnalogInInit();

	// Initialize analogOutput module
	AnalogOutInit();

	// Initialize HSMCI pins
	ConfigurePin(g_APinDescription[APIN_HSMCI_CLOCK]);
	ConfigurePin(g_APinDescription[APINS_HSMCI_DATA]);
}

// End
