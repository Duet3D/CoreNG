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


// Note that in the original ASF code, SD_MMC_0_CD_GPIO is a PIO pin number. We have modified the code to make it a logical pin number instead.
#if defined(__SAM3X8E__)

// HSMCI and SPI SD card configuration
# define SD_MMC_ENABLE
# define CONF_BOARD_SD_MMC_HSMCI	1			// Number of HSMCI card slots supported
# define SD_MMC_HSMCI_MEM_CNT		1			// can't use () around this because it gets used as an argument to MREPEAT
# define SD_MMC_HSMCI_SLOT_0_SIZE	4			// 4-bit bus
# define SD_MMC_SPI_MEM_CNT			0			// Number of SPI card slots supported
# define SD_MMC_MEM_CNT				1			// can't compute this or use () around it because it gets used as an argument to MREPEAT

// Define the card detect pins
# define SD_MMC_0_CD_GPIO			13			// Arduino Due digital pin 13
# define SD_MMC_0_WP_GPIO			(-1)
# define SD_MMC_CD_DETECT_VALUE		false
# define SD_MMC_WP_DETECT_VALUE		false

#elif defined(__SAM4E8E__)

// HSMCI and SPI SD card configuration
# define SD_MMC_ENABLE
# define CONF_BOARD_SD_MMC_HSMCI	1			// Number of HSMCI card slots supported
# define SD_MMC_HSMCI_MEM_CNT		1			// can't use () around this because it gets used as an argument to MREPEAT
# define SD_MMC_HSMCI_SLOT_0_SIZE	4			// 4-bit bus
# define SD_MMC_SPI_MEM_CNT			1			// Number of SPI card slots supported
# define SD_MMC_MEM_CNT				2			// can't use () around this because it gets used as an argument to MREPEAT

# define SD_MMC_0_CD_GPIO			53			// logical pin 53
# define SD_MMC_0_WP_GPIO			(-1)
# define SD_MMC_1_CD_GPIO			(-1)		// No CD pin on card accessed over SPI
# define SD_MMC_1_WP_GPIO			(-1)
# define SD_MMC_CD_DETECT_VALUE		false
# define SD_MMC_WP_DETECT_VALUE		false
# define SD_MMC_SPI_0_CS			56			// logical pin 56

#else

# error Configuration info missing for this board/processor

#endif

#if (SD_MMC_MEM_CNT != SD_MMC_HSMCI_MEM_CNT + SD_MMC_SPI_MEM_CNT)
#error  "Incorrect SD_MMC_MEM_CNT"
#endif

#define ACCESS_MEM_TO_RAM_ENABLED

#endif // CONF_BOARD_H
