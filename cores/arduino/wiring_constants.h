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

#ifndef _WIRING_CONSTANTS_
#define _WIRING_CONSTANTS_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

enum BitOrder {
	LSBFIRST = 0,
	MSBFIRST = 1
};

enum InterruptMode {
	INTERRUPT_MODE_NONE = 0,
	INTERRUPT_MODE_LOW,
	INTERRUPT_MODE_HIGH,
	INTERRUPT_MODE_CHANGE,
	INTERRUPT_MODE_FALLING,
	INTERRUPT_MODE_RISING
};

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

typedef bool boolean;

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* _WIRING_CONSTANTS_ */
