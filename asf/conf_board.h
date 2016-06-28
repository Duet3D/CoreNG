/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

// HSMCI configuration
#define SD_MMC_ENABLE
#define CONF_BOARD_SD_MMC_HSMCI		1
#define SD_MMC_HSMCI_MEM_CNT		1			// can't use () around this because it gets used as an argument to MREPEAT
#define SD_MMC_HSMCI_SLOT_0_SIZE	4			// 4-bits connector pin

// Note that in the original ASF code, SD_MMC_0_CD_GPIO is a PIO pin number. We have modified the code to make it a logical pin number instead.
#if defined(__SAM3X8E__)

// card detect pin
# define SD_MMC_0_CD_GPIO			13			// Arduino Due digital pin 13
# define SD_MMC_0_CD_DETECT_VALUE	0

#elif defined(__SAM4E8E__)

# define SD_MMC_0_CD_GPIO			53			// logical pin 49
# define SD_MMC_0_CD_DETECT_VALUE	false

#else

# error Configuration info missing for this board/processor

#endif

#define ACCESS_MEM_TO_RAM_ENABLED

#endif // CONF_BOARD_H
