/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Core_h
#define Core_h

#include "ecv.h"		// macros for Escher C/C++ Verifier design-by-contract annotations
#undef yield			// eCv definition clashes with function 'yield' in wiring.c (can use _ecv_yield instead within annotations)

#include "compiler.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "binary.h"
#include "itoa.h"

// Define attribute
#if defined(__GNUC__)
# define WEAK __attribute__ ((weak))
#endif

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Includes Atmel CMSIS
#include "asf.h"

#include "wiring_constants.h"

#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )

void yield(void);

/* sketch */
extern void setup(void);
extern void loop(void);

typedef uint8_t Pin;
static const Pin NoPin = 0xFF;

//#define NOT_A_PIN 0  // defined in pio.h/EPioType
#define NOT_A_PORT			(0)

#define NOT_AN_INTERRUPT	(-1)

typedef enum _EExt_Interrupts
{
  EXTERNAL_INT_0=0,
  EXTERNAL_INT_1=1,
  EXTERNAL_INT_2=2,
  EXTERNAL_INT_3=3,
  EXTERNAL_INT_4=4,
  EXTERNAL_INT_5=5,
  EXTERNAL_INT_6=6,
  EXTERNAL_INT_7=7,
  EXTERNAL_NUM_INTERRUPTS
} EExt_Interrupts ;

typedef void (*voidFuncPtr)(void) ;

// Definitions for PWM channels
typedef enum _EPWMChannel
{
  NOT_ON_PWM=-1,
  PWM_CH0=0,
  PWM_CH1,
  PWM_CH2,
  PWM_CH3,
  PWM_CH4,
  PWM_CH5,
  PWM_CH6,
  PWM_CH7
} EPWMChannel ;

// Definitions for TC channels
typedef enum _ETCChannel
{
  NOT_ON_TIMER=-1,
  TC0_CHA0=0,
  TC0_CHB0,
  TC0_CHA1,
  TC0_CHB1,
  TC0_CHA2,
  TC0_CHB2,
  TC1_CHA3,
  TC1_CHB3,
  TC1_CHA4,
  TC1_CHB4,
  TC1_CHA5,
  TC1_CHB5,
  TC2_CHA6,
  TC2_CHB6,
  TC2_CHA7,
  TC2_CHB7,
  TC2_CHA8,
  TC2_CHB8
} ETCChannel ;

#ifdef __cplusplus
} // extern "C"

// This analog input module uses the scheduler in the SAM processor to convert a number of channels.
// Usage:
// 1. Enable the channels you need by making calls to AnalogEnableChannel.
// 2. If desired, call AnalogSetCallback to set a callback for when conversion is complete.
// 3. Call AnalogStartConversion. This may be done e.g. in a tick interrupt if regular conversions are wanted.
// 4. Either use the callback to determine when conversion is complete, or call AnalogCheckReady to poll the status.
// 5. Call AnalogReadChannel to read the most recent converted result for each channel of interest.

enum AnalogChannelNumber
{
  NO_ADC=-1,
  ADC0=0,
  ADC1,
  ADC2,
  ADC3,
  ADC4,
  ADC5,
  ADC6,
  ADC7,
  ADC8,
  ADC9,
  ADC10,
  ADC11,
  ADC12,
  ADC13,
  ADC14,
  ADC15,
  ADC16,
  ADC17,
  ADC18,
  ADC19,
  ADC20,
  ADC21,
  ADC22,
  ADC23,
  ADC24,
  ADC25,
  ADC26,
  ADC27,
  ADC28,
  ADC29,
  ADC30,
  ADC31,
  DA0,
  DA1
};

// Pin Attributes to be OR-ed
#define PIN_ATTR_NONE		(0UL)
#define PIN_ATTR_COMBO		(1UL<<0)
#define PIN_ATTR_ANALOG		(1UL<<1)
#define PIN_ATTR_DIGITAL	(1UL<<2)
#define PIN_ATTR_PWM		(1UL<<3)
#define PIN_ATTR_TIMER		(1UL<<4)
#define PIN_ATTR_DAC		(1UL<<5)

// Types used for the tables below
struct PinDescription
{
	Pio* pPort;
	uint32_t ulPin;
	uint32_t ulPeripheralId;
	pio_type_t ulPinType;
	uint32_t ulPinConfiguration;
	uint32_t ulPinAttribute;
	AnalogChannelNumber ulADCChannelNumber; // ADC or DAC channel number in the SAM device
	EPWMChannel ulPWMChannel;
	ETCChannel ulTCChannel;
};

/* Pins table to be instantiated into variant.cpp */
extern const PinDescription g_APinDescription[];

#include "WCharacter.h"
#include "HardwareSerial.h"

#endif // __cplusplus

// Include board variant
#include "variant.h"

#include "wiring.h"
#include "wiring_digital.h"
#include "wiring_shift.h"
#include "WInterrupts.h"
#include "watchdog.h"
#include "WMath.h"
#include "Reset.h"

#ifdef __cplusplus
#include "AnalogIn.h"
#include "AnalogOut.h"
#include "USB/USBSerial.h"
#endif

#endif // Arduino_h
