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
#undef array
#undef yield			// eCv definition clashes with function 'yield' in wiring.c (can use _ecv_yield instead within annotations)
#undef value			// needed because we include <optional> in some projects
#undef result

#include "compiler.h"
#define SAME5x		0	// projects for SAME5x use CoreN2G instead

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "binary.h"
#include "itoa.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "asf.h"
#include "wiring_constants.h"

void yield(void) noexcept;
void CoreSysTick(void) noexcept;

typedef uint8_t Pin;
static const Pin NoPin = 0xFF;
typedef uint8_t DmaChannel;
typedef uint16_t PwmFrequency;		// type used to represent a PWM frequency. 0 sometimes means "default".
typedef uint32_t NvicPriority;

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus

extern const char *sysStackLimit;

// Definitions for PWM channels
enum EPWMChannel : int8_t
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
};

// Definitions for TC channels
enum ETCChannel : int8_t
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
#if SAM3XA || SAM4E || SAME70
  TC2_CHA6,
  TC2_CHB6,
  TC2_CHA7,
  TC2_CHB7,
  TC2_CHA8,
  TC2_CHB8,
#endif
#if SAME70
  TC3_CHA9,
  TC3_CHB9,
  TC3_CHA10,
  TC3_CHB10,
  TC3_CHA11,
  TC3_CHB11
#endif
};

// The analog input module uses the scheduler in the SAM processor to convert a number of channels.
// Usage:
// 1. Enable the channels you need by making calls to AnalogEnableChannel.
// 2. If desired, call AnalogSetCallback to set a callback for when conversion is complete.
// 3. Call AnalogStartConversion. This may be done e.g. in a tick interrupt if regular conversions are wanted.
// 4. Either use the callback to determine when conversion is complete, or call AnalogCheckReady to poll the status.
// 5. Call AnalogReadChannel to read the most recent converted result for each channel of interest.

enum AnalogChannelNumber : int8_t
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
constexpr uint8_t PIN_ATTR_NONE = 0;
constexpr uint8_t PIN_ATTR_COMBO = 1 << 0;
constexpr uint8_t PIN_ATTR_ANALOG = 1 << 1;
constexpr uint8_t PIN_ATTR_DIGITAL = 1 << 2;
constexpr uint8_t PIN_ATTR_PWM = 1 << 3;
constexpr uint8_t PIN_ATTR_TIMER = 1 << 4;
constexpr uint8_t PIN_ATTR_DAC = 1 << 5;

// Types used for the tables below
struct PinDescription
{
	Pio* pPort;
	uint32_t ulPin;
	uint32_t ulPeripheralId;
	pio_type_t ulPinType;
	uint32_t ulPinConfiguration;
	uint8_t ulPinAttribute;
	AnalogChannelNumber ulADCChannelNumber; // ADC or DAC channel number in the SAM device
	EPWMChannel ulPWMChannel;
	ETCChannel ulTCChannel;
};

/* Pins table to be instantiated into variant.cpp */
extern const PinDescription g_APinDescription[];

#include "WCharacter.h"
#include "HardwareSerial.h"
#include "WInterrupts.h"

// Optimised version of memcpy for use when the source and destination are known to be 32-bit aligned and a whole number of 32-bit words is to be copied
void memcpyu32(uint32_t *dst, const uint32_t *src, size_t numWords) noexcept;

// memcpy for int32_t arrays
inline void memcpyi32(int32_t *dst, const int32_t *src, size_t numWords) noexcept
{
	static_assert(sizeof(int32_t) == sizeof(uint32_t));
	static_assert(alignof(int32_t) == alignof(uint32_t));
	memcpyu32(reinterpret_cast<uint32_t*>(dst), reinterpret_cast<const uint32_t*>(src), numWords);
}

// memcpy for float arrays
inline void memcpyf(float *dst, const float *src, size_t numFloats) noexcept
{
	static_assert(sizeof(float) == sizeof(uint32_t));
	static_assert(alignof(float) == alignof(uint32_t));
	memcpyu32(reinterpret_cast<uint32_t*>(dst), reinterpret_cast<const uint32_t*>(src), numFloats);
}

#endif // __cplusplus

// Include board variant
#include "variant.h"

#include "wiring.h"
#include "wiring_digital.h"
#include "watchdog.h"
#include "WMath.h"
#include "Reset.h"

#ifdef __cplusplus
#include "AnalogIn.h"
#include "AnalogOut.h"
#include "USB/USBSerial.h"
#endif

#endif // Arduino_h
