/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#ifndef _WIRING_INTERRUPTS_
#define _WIRING_INTERRUPTS_

#include "Core.h"

union CallbackParameter
{
	void *vp;
	uint32_t u32;
	int32_t i32;

	CallbackParameter(void *pp) noexcept : vp(pp) { }
	CallbackParameter(uint32_t pp) noexcept : u32(pp) { }
	CallbackParameter(int32_t pp) noexcept : i32(pp) { }
	CallbackParameter() noexcept : u32(0) { }
};

typedef void (*StandardCallbackFunction)(CallbackParameter) noexcept;

bool attachInterrupt(uint32_t pin, StandardCallbackFunction callback, enum InterruptMode mode, CallbackParameter param) noexcept;

void detachInterrupt(uint32_t pin) noexcept;

// Return true if we are in an interrupt service routine
bool inInterrupt() noexcept;

#ifdef __cplusplus

// Atomic section locker, alternative to InterruptCriticalSectionLocker (is safe to call from within an ISR, and may be faster)
class AtomicCriticalSectionLocker
{
public:
	AtomicCriticalSectionLocker() noexcept : flags(cpu_irq_save())
	{
	}

	~AtomicCriticalSectionLocker()
	{
		cpu_irq_restore(flags);
	}

private:
	irqflags_t flags;
};

// Functions to change the base priority, to shut out interrupts up to a priority level

// Get the base priority and shut out interrupts lower than or equal to a specified priority
inline uint32_t ChangeBasePriority(uint32_t prio) noexcept
{
	const uint32_t oldPrio = __get_BASEPRI();
	__set_BASEPRI_MAX(prio << (8 - __NVIC_PRIO_BITS));
	return oldPrio;
}

// Restore the base priority following a call to ChangeBasePriority
inline void RestoreBasePriority(uint32_t prio) noexcept
{
	__set_BASEPRI(prio);
}

// Set the base priority when we are not interested in the existing value i.e. definitely in non-interrupt code
inline void SetBasePriority(uint32_t prio) noexcept
{
	__set_BASEPRI(prio << (8 - __NVIC_PRIO_BITS));
}

#endif

#endif /* _WIRING_INTERRUPTS_ */
