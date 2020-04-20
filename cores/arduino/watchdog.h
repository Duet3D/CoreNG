/*
  Copyright (c) 2014 Arduino.  All right reserved.

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

#ifndef _WATCHDOG_
#define _WATCHDOG_

#include <stdint.h>

// Watchdog functions

/*
 * \brief Enable the watchdog with the specified timeout. Should only be called once.
 *
 * \param timeout in milliseconds.
 */
void watchdogEnable (uint32_t timeout) noexcept;

/*
 * \brief Disable the watchdog timer. Should only be called once.
 *
 */
void watchdogDisable (void) noexcept;

/*
 * \brief Reset the watchdog counter.
 *
 */
void watchdogReset (void) noexcept;

#endif /* _WATCHDOG_ */

