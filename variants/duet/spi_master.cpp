// ASF 3.27.0

/**
 * \file
 *
 * \brief SPI master common service for SAM.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "Arduino.h"
#include "compiler.h"
#include "spi_master.h"
#include "variant.h"
#include "sam/drivers/spi/spi.h"


/**
 * \brief Max number when the chip selects are connected to a 4- to 16-bit decoder.
 */
#define MAX_NUM_WITH_DECODER 0x10

/**
 * \brief Max number when the chip selects are directly connected to peripheral device.
 */
#define MAX_NUM_WITHOUT_DECODER 0x04

/**
 * \brief Max number when the chip selects are directly connected to peripheral device.
 */
#define NONE_CHIP_SELECT_ID 0x0f

/**
 * \brief The default chip select id.
 */
#define DEFAULT_CHIP_ID 1

/** \brief Initialize the SPI in master mode.
 *
 * \param p_spi  Base address of the SPI instance.
 *
 */

void spi_master_init(Spi *p_spi, int ul_cs_pin)
{
	static bool init_comms = true;

	if (init_comms)
	{
		ConfigurePin(g_APinDescription[APIN_SPI_SCK]);
		ConfigurePin(g_APinDescription[APIN_SPI_MOSI]);
		ConfigurePin(g_APinDescription[APIN_SPI_MISO]);

		pmc_enable_periph_clk(SPI_INTERFACE_ID);

		spi_reset(p_spi);

		// set master mode, peripheral select, disable fault detection
		spi_set_master_mode(p_spi);
		spi_disable_mode_fault_detect(p_spi);
		spi_disable_loopback(p_spi);
		spi_set_peripheral_chip_select_value(p_spi, DEFAULT_CHIP_ID);
		spi_set_fixed_peripheral_select(p_spi);
		spi_disable_peripheral_select_decode(p_spi);

		init_comms = false;
	}

	if (ul_cs_pin >= 0)
	{
		pinMode(ul_cs_pin, OUTPUT);
		digitalWrite(ul_cs_pin, HIGH);
	}

#if defined(USE_SAM3X_DMAC)
	pmc_enable_periph_clk(ID_DMAC);
	dmac_disable(DMAC);
	dmac_set_priority_mode(DMAC, DMAC_GCFG_ARB_CFG_FIXED);
	dmac_enable(DMAC);
#endif

}

/**
 * \brief Set up an SPI device.
 *
 * The returned device descriptor structure must be passed to the driver
 * whenever that device should be used as current slave device.
 *
 * \param p_spi     Base address of the SPI instance.
 * \param device    Pointer to SPI device struct that should be initialized.
 * \param flags     SPI configuration flags. Common flags for all
 *                  implementations are the SPI modes SPI_MODE_0 ...
 *                  SPI_MODE_3.
 * \param baud_rate Baud rate for communication with slave device in Hz.
 * \param sel_id    Board specific select id.
 */
void spi_master_setup_device(Spi *p_spi, const struct spi_device *device,
		spi_flags_t flags, uint32_t baud_rate, board_spi_select_id_t sel_id)
{
	/* avoid Cppcheck Warning */
	UNUSED(sel_id);
	UNUSED(flags);

	spi_reset(p_spi);
	spi_set_transfer_delay(p_spi, device->id, CONFIG_SPI_MASTER_DELAY_BS,
			CONFIG_SPI_MASTER_DELAY_BCT);
	spi_set_bits_per_transfer(p_spi, device->id,
			CONFIG_SPI_MASTER_BITS_PER_TRANSFER);
	int16_t baud_div = spi_calc_baudrate_div(baud_rate, SystemCoreClock);
	spi_set_baudrate_div(p_spi, device->id, baud_div);
	spi_configure_cs_behavior(p_spi, device->id, SPI_CS_KEEP_LOW);
	spi_set_clock_polarity(p_spi, device->id, flags >> 1);
	spi_set_clock_phase(p_spi, device->id, ((flags & 0x1) ^ 0x1));
}

/**
 * \brief Select the given device on the SPI bus.
 *
 * Set device specific setting and call board chip select.
 *
 * \param p_spi   Base address of the SPI instance.
 * \param device  SPI device.
 *
 */
void spi_select_device(Spi *p_spi, const struct spi_device *device)
{
	if (spi_get_peripheral_select_decode_setting(p_spi)) {
		if (device->id < MAX_NUM_WITH_DECODER) {
			spi_set_peripheral_chip_select_value(p_spi, device->id);
		}
	} else {
		if (device->id < MAX_NUM_WITHOUT_DECODER) {
			spi_set_peripheral_chip_select_value(p_spi, (~(1 << device->id)));
		}
    }

	// Enable the CS line
	digitalWrite(device->cs, LOW);
}

/**
 * \brief Deselect the given device on the SPI bus.
 *
 * Call board chip deselect.
 *
 * \param p_spi   Base address of the SPI instance.
 * \param device  SPI device.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
void spi_deselect_device(Spi *p_spi, const struct spi_device *device)
{
	uint32_t timeout = SPI_TIMEOUT;
	while (!spi_is_tx_empty(p_spi)) {
		if (!timeout--) {
			return;
		}
	}

	// Last transfer, so de-assert the current NPCS if CSAAT is set.
	spi_set_lastxfer(p_spi);

	// Disable the CS line
	digitalWrite(device->cs, HIGH);

	// Assert all lines; no peripheral is selected.
	spi_set_peripheral_chip_select_value(p_spi, NONE_CHIP_SELECT_ID);
}

/** \brief Get one data to a SPI peripheral.
 *
 * \param p_spi Base address of the SPI instance.
 * \return The data byte
 *
 */
spi_status_t spi_read_single(Spi *p_spi, uint8_t *b)
{
	// wait for transmit register empty
	uint32_t timeout = SPI_TIMEOUT;
	while (!spi_is_tx_ready(p_spi)) {
		if (--timeout == 0) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	// write dummy byte with address and end transmission flag
	p_spi->SPI_TDR = 0x000000FF | SPI_TDR_LASTXFER;

	// wait for receive register
	timeout = SPI_TIMEOUT;
	while (!spi_is_rx_ready(p_spi)) {
		if (--timeout == 0) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	// get byte from receive register
	*b = (uint8_t)p_spi->SPI_RDR;

	return SPI_OK;

	// return (p_spi->SPI_RDR & SPI_RDR_RD_Msk);
}

/** \brief Get one data to a SPI peripheral.
 *
 * \param p_spi Base address of the SPI instance.
 * \return The data byte
 *
 */
spi_status_t spi_read_single16(Spi *p_spi, uint16_t *b)
{
	// wait for transmit register empty
	uint32_t timeout = SPI_TIMEOUT;
	while (!spi_is_tx_ready(p_spi)) {
		if (--timeout == 0) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	// write dummy byte with address and end transmission flag
	p_spi->SPI_TDR = 0x0000FFFF | SPI_TDR_LASTXFER;

	// wait for receive register
	timeout = SPI_TIMEOUT;
	while (!spi_is_rx_ready(p_spi)) {
		if (--timeout == 0) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	// get byte from receive register
	*b = (uint16_t)p_spi->SPI_RDR;

	return SPI_OK;
}

/**
 * \brief Put one data to a SPI peripheral.
 *
 * \param p_spi Base address of the SPI instance.
 * \param data The data byte to be loaded
 *
 */
spi_status_t spi_write_single(Spi *p_spi, uint8_t data)
{
	// p_spi->SPI_TDR = SPI_TDR_TD(data);

	// wait for transmit register empty
	uint32_t timeout = SPI_TIMEOUT;
	while (!spi_is_tx_ready(p_spi)) {
		if (!timeout--)
		{
			return SPI_ERROR_TIMEOUT;
		}
	}

	// write byte with address and end transmission flag
 	p_spi->SPI_TDR = (uint32_t)data | SPI_TDR_LASTXFER;

	// wait for receive register
	timeout = SPI_TIMEOUT;
	while (!spi_is_rx_ready(p_spi)) {
		if (--timeout == 0) {
			return SPI_ERROR_TIMEOUT;
		}
	}

	// clear status
	p_spi->SPI_RDR;

	return SPI_OK;
}

/**
 * \brief Send a sequence of bytes to an SPI device.
 *
 * Received bytes on the SPI bus are discarded.
 *
 * \param p_spi     Base address of the SPI instance.
 * \param data      Data buffer to write.
 * \param len       Length of data to be written.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t spi_write_packet(Spi *p_spi, const uint8_t *data, size_t len)
{
	if (len == 0)
		return SPI_OK;

	for (size_t i = 0; i < len-1; i++)
	{
		uint32_t timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}

		p_spi->SPI_TDR = (uint32_t)data[i];

		while (!spi_is_rx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}

		p_spi->SPI_RDR;
	}

	return spi_write_single(p_spi, data[len-1]);
}

/**
 * \brief Receive a sequence of bytes from an SPI device.
 *
 * All bytes sent out on SPI bus are sent as value 0.
 *
 * \param p_spi     Base address of the SPI instance.
 * \param data      Data buffer to read.
 * \param len       Length of data to be read.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t spi_read_packet(Spi *p_spi, uint8_t *data, size_t len)
{
	if (len-- == 0)
		return SPI_OK;

	for (uint16_t i = 0; i < len; i++)
	{
		uint32_t timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}

		p_spi->SPI_TDR = 0x000000FF;

		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}

		data[i] = p_spi->SPI_RDR;
	}

	return spi_read_single(p_spi, &data[len]);
}

/**
 * \brief Receive a sequence of 16-bit words from an SPI device.
 *
 * All bytes sent out on SPI bus are sent as value 0xff.
 *
 * \param p_spi     Base address of the SPI instance.
 * \param data      Data buffer to read.
 * \param len       Number of words of data to be read.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t spi_read_packet16(Spi *p_spi, uint16_t *data, size_t len)
{
	if (len-- == 0)
		return SPI_OK;

	for (uint16_t i = 0; i < len; i++)
	{
		uint32_t timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}

		p_spi->SPI_TDR = 0x0000FFFF;

		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}

		data[i] = (uint16_t)(p_spi->SPI_RDR);
	}

	return spi_read_single16(p_spi, &data[len]);
}

/**
 * \brief Send and receive a sequence of bytes from an SPI device.
 *
 * \param p_spi     Base address of the SPI instance.
 * \param tx_data   Data buffer to send.
 * \param rx_data   Data buffer to read.
 * \param len       Length of data to be read.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t spi_transceive_packet(Spi *p_spi, uint8_t *tx_data, uint8_t *rx_data, size_t len)
{
	uint32_t timeout = SPI_TIMEOUT;
	uint8_t val;
	uint32_t i = 0;

	while (len) {
		timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}
		spi_write_single(p_spi, tx_data[i]);

		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_ready(p_spi)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
			}
		}
		spi_read_single(p_spi, &val);

		rx_data[i] = val;
		i++;
		len--;
	}

	return SPI_OK;
}

#if defined(USE_SAM3X_DMAC)

void spi_start_transmit_dma(Dmac *p_dmac, Spi *p_spi, uint32_t ul_num,
							const void *src, uint32_t nb_bytes)
{
	static uint8_t ff = 0xFF;
	uint32_t cfg, src_incr = DMAC_CTRLB_SRC_INCR_INCREMENTING;
	dma_transfer_descriptor_t desc;

	// Send 0xFF repeatedly if src is NULL
	if (!src) {
		src = &ff;
		src_incr = DMAC_CTRLB_SRC_INCR_FIXED;
	}

	// Disable the DMA channel prior to configuring
	dmac_enable(p_dmac);
	dmac_channel_disable(p_dmac, ul_num);

	cfg = DMAC_CFG_SOD
		| DMAC_CFG_DST_H2SEL
		| DMAC_CFG_DST_PER(SPI_TX_IDX)
		| DMAC_CFG_FIFOCFG_ALAP_CFG;
	dmac_channel_set_configuration(p_dmac, ul_num, cfg);

	// Prepare DMA transfer
	desc.ul_source_addr = (uint32_t)src;
	desc.ul_destination_addr = (uint32_t)&(p_spi->SPI_TDR);
	desc.ul_ctrlA = DMAC_CTRLA_BTSIZE(nb_bytes)
		| DMAC_CTRLA_SRC_WIDTH_BYTE
		| DMAC_CTRLA_DST_WIDTH_BYTE;
	desc.ul_ctrlB = DMAC_CTRLB_SRC_DSCR
		| DMAC_CTRLB_DST_DSCR
		| DMAC_CTRLB_FC_MEM2PER_DMA_FC
		| src_incr
		| DMAC_CTRLB_DST_INCR_FIXED;

	// Next field is ignored, but set it anyway
	desc.ul_descriptor_addr = (uint32_t)NULL;

 	// Finish configuring the transfer
	dmac_channel_single_buf_transfer_init(p_dmac, ul_num, &desc);

	// And now start the DMA transfer
	dmac_channel_enable(p_dmac, ul_num);
}

void spi_start_receive_dma(Dmac *p_dmac, Spi *p_spi, uint32_t ul_num,
						   const void *dest, uint32_t nb_bytes)
{
	uint32_t cfg;
	dma_transfer_descriptor_t desc;

	// clear any potential overrun error
	cfg = p_spi->SPI_SR;

	// Turn the DMA channel off before configuring
	dmac_enable(p_dmac);
	dmac_channel_disable(p_dmac, ul_num);

	cfg = DMAC_CFG_SOD
		| DMAC_CFG_SRC_H2SEL
		| DMAC_CFG_SRC_PER(SPI_RX_IDX)
		| DMAC_CFG_FIFOCFG_ASAP_CFG;
	dmac_channel_set_configuration(p_dmac, ul_num, cfg);

	// Prepare DMA transfer
	desc.ul_source_addr = (uint32_t)&(p_spi->SPI_RDR);
	desc.ul_destination_addr = (uint32_t)dest;
	desc.ul_ctrlA = DMAC_CTRLA_BTSIZE(nb_bytes)
		| DMAC_CTRLA_SRC_WIDTH_BYTE
		| DMAC_CTRLA_DST_WIDTH_BYTE;
	desc.ul_ctrlB = DMAC_CTRLB_SRC_DSCR
		| DMAC_CTRLB_DST_DSCR
		| DMAC_CTRLB_FC_PER2MEM_DMA_FC
		| DMAC_CTRLB_SRC_INCR_FIXED
		| DMAC_CTRLB_DST_INCR_INCREMENTING;

	// This next field is ignored but set it anyway
	desc.ul_descriptor_addr = (uint32_t)NULL;

	// Finish configuring the DMA transfer
	dmac_channel_single_buf_transfer_init(p_dmac, ul_num, &desc);

	// And now allow the DMA transfer to begin
	dmac_channel_enable(p_dmac, ul_num);
}

#endif

//! @}
