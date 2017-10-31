// ASF 3.27.0

// Note:
// ASF 3.31 includes spi_master.c, .h in folder common/services/spi/sam_spi.
// Unfortunately, that version assumes that only he "official" SPI CS pins are used.
// So instead we use this version, which allows any pin to be used for CS.

/**
 * \file
 *
 * \brief Shared SPI bus services for Duet and other Atmel SAM-based controller electronics
 *
 * This module provides access to the SPI bus used to access peripheral devices in RepRapFirmware, in particular thermocouple and RTD readers.
 * Depending on the board, we may use either the main SPI channel or one of the USARTs in SPI mode.
 *
 */

#ifndef _GSPI_MASTER_H_
#define _GSPI_MASTER_H_

#include "compiler.h"
#include "spi/spi.h"

#if defined(USE_SAM3X_DMAC)
#include "dmac/dmac.h"
#include "pmc/pmc.h"
#endif

/**
 * \brief Clock phase.
 */
#define SPI_CPHA  (1 << 0)

/**
 * \brief Clock polarity.
 */
#define SPI_CPOL  (1 << 1)

/**
 * \brief SPI mode 0.
 */
#define SPI_MODE_0  0
/**
 * \brief SPI mode 1.
 */
#define SPI_MODE_1  (SPI_CPHA)
/**
 * \brief SPI mode 2.
 */
#define SPI_MODE_2  (SPI_CPOL)
/**
 * \brief SPI mode 3.
 */
#define SPI_MODE_3  (SPI_CPOL | SPI_CPHA)

//! \brief Polled SPI device definition.
struct sspi_device {
	Pin csPin;
	uint8_t bitsPerTransferControl;
	uint8_t spiMode;
	uint32_t clockFrequency;
};

#ifdef __cplusplus
// Use C linkage because the RADDS port may need to call these functions from the ASF SD card code
extern "C" {
#endif

// Gain exclusive use of the GSPI bus
// Returning true if successful, false if GSPI bus is busy
extern bool sspi_acquire();

// Release the GSPI bus
extern void sspi_release();

/** \brief Initialize the SPI in master mode.
 *
 */
extern void sspi_master_init(struct sspi_device *device, uint32_t bits);

/**
 * \brief Set up an SPI device.
 *
 * The returned device descriptor structure must be passed to the driver
 * whenever that device should be used as current slave device.
 *
 * \param device    Pointer to SPI device struct that should be initialized.
 */
extern void sspi_master_setup_device(const struct sspi_device *device);

/**
 * \brief Select the given device on the SPI bus.
 *
 * Set device specific setting and call board chip select.
 *
 * \param device SPI device.
 *
 */
extern void sspi_select_device(const struct sspi_device *device);

/**
 * \brief Deselect the given device on the SPI bus.
 *
 * Call board chip deselect.
 *
 * \param device SPI device.
 *
 * \pre SPI device must be selected with spi_select_device() first and in 8-bit data mode.
 */
extern void sspi_deselect_device(const struct sspi_device *device);

/**
 * \brief Send and receive a sequence of bytes from the shared SPI device.
 *
 * \param tx_data   Data buffer to send.
 * \param rx_data   Data buffer to read.
 * \param len       Length of data to be read.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t sspi_transceive_packet(const uint8_t *tx_data, uint8_t *rx_data, size_t len);

// Receive a packet
static inline spi_status_t sspi_read_packet(uint8_t *buf, size_t len)
{
	return sspi_transceive_packet(NULL, buf, len);
}

// Send a packet
static inline spi_status_t sspi_write_packet(uint8_t *buf, size_t len)
{
	return sspi_transceive_packet(buf, NULL, len);
}

#if SAM3XA
/**
 * \brief Send and receive a sequence of 16-bit words from the shared SPI device.
 *
 * \param tx_data   Data buffer to send.
 * \param rx_data   Data buffer to read.
 * \param len       Length of data to send and receive in 16-bit words.
 *
 * \pre SPI device must be selected with spi_select_device() first and in 16-bit data mode.
 */
spi_status_t sspi_transceive_packet16(const uint16_t *tx_data, uint16_t *rx_data, size_t len);
#endif

#if defined(USE_SAM3X_DMAC)

#define SPI_TX_IDX 1 // DMAC HW interface id for SPI TX (Table 22-2. DMA Controller)
#define SPI_RX_IDX 2 // DMAC HW interface id for SPI RX (Table 22-2. DMA Controller)

void sspi_start_transmit_dma(Dmac *p_dmac, uint32_t ul_num, const void *src, uint32_t nb_bytes);

void sspi_start_receive_dma(Dmac *p_dmac, uint32_t ul_num, const void *dest, uint32_t nb_bytes);

static inline uint32_t sspi_get_peripheral_chip_select_value()
{
	// return 3 & ((SSPI->SPI_MR & SPI_MR_PCS_Msk) >> SPI_MR_PCS_Pos);
	return 3 & (SSPI->SPI_MR >> SPI_MR_PCS_Pos);
}

static inline uint32_t sspi_get_clock_divisor()
{
	uint32_t ul_pcs_ch = spi_get_peripheral_chip_select_value(SSPI);
	if (ul_pcs_ch > 3) return 255;
	return (SSPI->SPI_CSR[ul_pcs_ch] & SPI_CSR_SCBR_Msk) >> SPI_CSR_SCBR_Pos;
}

#endif

#ifdef __cplusplus
}
#endif

#endif // _GSPI_MASTER_H_
