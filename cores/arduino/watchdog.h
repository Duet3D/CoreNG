/*
  watchdog.h
  D. Crocker, 2020-12-08
*/

#ifndef WATCHDOG_
#define _WATCHDOG_

#include <asf.h>

// Watchdog functions

static inline void WatchdogReset() noexcept
{
	WDT->WDT_CR = WDT_CR_KEY_PASSWD | WDT_CR_WDRSTT;
}

#endif /* _WATCHDOG_ */

