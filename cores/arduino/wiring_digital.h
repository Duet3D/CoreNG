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

#ifndef _WIRING_DIGITAL_
#define _WIRING_DIGITAL_

#ifdef __cplusplus
extern "C" {
#endif

enum PinMode
{
	INPUT,				// pin is a digital input
	INPUT_PULLUP,		// pin is a digital input with pullup enabled
#if SAM4E
	INPUT_PULLDOWN,		// pin is a digital input with pulldown enabled
#else
	INPUT_PULLDOWN_NOT_AVAILABLE,
#endif
	OUTPUT_LOW,			// pin is an output with initial state LOW
	OUTPUT_HIGH,		// pin is an output with initial state HIGH
	AIN,				// pin is an analog input, digital input buffer is disabled if possible
	SPECIAL				// pin is used for the special function defined for it in the variant.cpp file
};

/**
 * \brief Configures the specified pin to behave either as an input or an output. See the description of digital pins for details.
 *
 * \param ulPin The number of the pin whose mode you wish to set
 * \param ulMode Either INPUT or OUTPUT
 * \param debounceCutoff Debounce cutoff frequency (only one can be set per PIO)
 */
extern void pinModeDuet(Pin pin, enum PinMode dwMode, uint32_t debounceCutoff);

inline void pinMode(Pin pin, enum PinMode dwMode)
{
	pinModeDuet(pin, dwMode, 0);
}

/**
 * \brief Write a HIGH or a LOW value to a digital pin.
 *
 * If the pin has been configured as an OUTPUT with pinMode(), its voltage will be set to the
 * corresponding value: 5V (or 3.3V on 3.3V boards) for HIGH, 0V (ground) for LOW.
 *
 * \param dwPin the pin number
 * \param dwVal true to set the pin HIGH, false to set it LOW
 */
extern void digitalWrite(Pin pin, bool dwVal);

/**
 * \brief Reads the value from a specified digital pin, either HIGH or LOW.
 *
 * \param ulPin The number of the digital pin you want to read (int)
 *
 * \return true for HIGH, false for LOW
 */
extern bool digitalRead(Pin pin);

/**
 * \brief Enable or disable the pullup resistor on a pin.
 *
 * \param ulPin The number of the digital pin you want to change
 *
 * \param en Whether to enable (true) or disable (false) the pullup resistor
 */
extern void setPullup(Pin pin, bool en);

#ifdef __cplusplus
}
#endif

// The remaining functionality is only available to C++ clients
#ifdef __cplusplus

inline const PinDescription& GetPinDescription(Pin pin)
{
	return g_APinDescription[pin];
}

#endif

#endif /* _WIRING_DIGITAL_ */
