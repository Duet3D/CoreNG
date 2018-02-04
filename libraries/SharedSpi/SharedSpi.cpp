// ASF 3.27.0

/**
 * \file
 *
 * \brief Shared SPI bus services for Duet and other Atmel SAM-based controller electronics
 *
 * This module provides access to the SPI bus used to access peripheral devices in RepRapFirmware, in particular thermocouple and RTD readers.
 * Depending on the board, we may use either the main SPI channel or one of the USARTs in SPI mode.
 *
 */

#include "Core.h"
#include "SharedSpi.h"
#include "variant.h"

#if SAM4E

#define USART_SPI		1

# include "usart/usart.h"		// On Duet NG the general SPI channel is on USART 0

#  define USART_SSPI	USART0
#  define ID_SSPI		ID_USART0

#elif SAM4S

#define USART_SPI		1

# include "usart/usart.h"		// On Duet Maestro the general SPI channel is on USART 0

#  define USART_SSPI	USART0
#  define ID_SSPI		ID_USART0

#else

#define USART_SPI		0

// We have to tell the processor which NPCS output we are using, even though we use other pins for CS
#if SAME70
// We choose NPCS2 because on the SAME70, it is not physically connected
# define PERIPHERAL_CHANNEL_ID		2
# define PERIPHERAL_CHANNEL_CS_PIN	APIN_SPI_SS2
#elif SAM3XA
// We choose NPCS3 because on the SAM3X, it is not physically connected
# define PERIPHERAL_CHANNEL_ID		3
# define PERIPHERAL_CHANNEL_CS_PIN	APIN_SPI_SS3
#endif


/** Time-out value (number of attempts). */
#define SPI_TIMEOUT       15000

// Which SPI channel we use
# define SSPI		SPI0
# define ID_SSPI	ID_SPI0

#endif

// Lock for the SPI subsystem
static bool sspiLocked = false;

// Gain exclusive use of the GSPI bus
// Returning true if successful, false if GSPI bus is busy
bool sspi_acquire()
{
	irqflags_t flags = cpu_irq_save();
	bool rslt;
	if (sspiLocked)
	{
		rslt = false;
	}
	else
	{
		sspiLocked = true;
		rslt = true;
	}
	cpu_irq_restore(flags);
	return rslt;
}

// Release the GSPI bus
void sspi_release()
{
	sspiLocked = false;
}

// Wait for transmitter ready returning true if timed out
static inline bool waitForTxReady()
{
	uint32_t timeout = SPI_TIMEOUT;
#if USART_SPI
	while (!usart_is_tx_ready(USART_SSPI))
#else
	while (!spi_is_tx_ready(SSPI))
#endif
	{
		if (--timeout == 0)
		{
			return true;
		}
	}
	return false;
}

// Wait for transmitter empty returning true if timed out
static inline bool waitForTxEmpty()
{
	uint32_t timeout = SPI_TIMEOUT;
#if USART_SPI
	while (!usart_is_tx_empty(USART_SSPI))
#else
		while (!spi_is_tx_empty(SSPI))
#endif
	{
		if (!timeout--)
		{
			return true;
		}
	}
	return false;
}

// Wait for receive data available returning true if timed out
static inline bool waitForRxReady()
{
	uint32_t timeout = SPI_TIMEOUT;
#if USART_SPI
	while (!usart_is_rx_ready(USART_SSPI))
#else
	while (!spi_is_rx_ready(SSPI))
#endif
	{
		if (--timeout == 0)
		{
			return true;
		}
	}
	return false;
}

// Set up the Shared SPI subsystem
void sspi_master_init(struct sspi_device *device, uint32_t bits)
{
	static bool commsInitDone = false;

	pinMode(device->csPin, (device->csPolarity) ? OUTPUT_LOW : OUTPUT_HIGH);

	if (!commsInitDone)
	{
#if USART_SPI
		ConfigurePin(g_APinDescription[APIN_USART0_SCK]);
		ConfigurePin(g_APinDescription[APIN_USART0_MOSI]);
		ConfigurePin(g_APinDescription[APIN_USART0_MISO]);
		pmc_enable_periph_clk(ID_SSPI);

		// Set USART0 in SPI master mode
		USART_SSPI->US_IDR = ~0u;
		USART_SSPI->US_CR = US_CR_RSTRX | US_CR_RSTTX | US_CR_RXDIS | US_CR_TXDIS;
		USART_SSPI->US_MR = US_MR_USART_MODE_SPI_MASTER
						| US_MR_USCLKS_MCK
						| US_MR_CHRL_8_BIT
						| US_MR_CHMODE_NORMAL;
		USART_SSPI->US_BRGR = SystemCoreClock/1000000;			// 1MHz SPI clock for now
		USART_SSPI->US_CR = US_CR_RSTRX | US_CR_RSTTX | US_CR_RXDIS | US_CR_TXDIS | US_CR_RSTSTA;
#else
		ConfigurePin(g_APinDescription[APIN_SPI_SCK]);
		ConfigurePin(g_APinDescription[APIN_SPI_MOSI]);
		ConfigurePin(g_APinDescription[APIN_SPI_MISO]);

		pmc_enable_periph_clk(ID_SSPI);

		SSPI->SPI_CR = SPI_CR_SPIDIS;
	    SSPI->SPI_CR = SPI_CR_SWRST;
	    SSPI->SPI_MR = SPI_MR_MSTR | SPI_MR_MODFDIS;

# if defined(USE_SAM3X_DMAC)
		pmc_enable_periph_clk(ID_DMAC);
		dmac_disable(DMAC);
		dmac_set_priority_mode(DMAC, DMAC_GCFG_ARB_CFG_FIXED);
		dmac_enable(DMAC);
# endif

#endif
		commsInitDone = true;
	}

#if USART_SPI
	// On USARTs we only support 8-bit transfers. 5, 6, 7 and 9 are also available.
	device->bitsPerTransferControl = US_MR_CHRL_8_BIT;
#else
	// On SPI we only support 8 and 16 bit modes. 11-15 bit modes are also available.
	switch (bits)
	{
	case 8:
	default:
		device->bitsPerTransferControl = SPI_CSR_BITS_8_BIT;
		break;
	case 16:
		device->bitsPerTransferControl = SPI_CSR_BITS_16_BIT;
		break;
	}
#endif
}

/**
 * \brief Set up an SPI device.
 *
 * The returned device descriptor structure must be passed to the driver
 * whenever that device should be used as current slave device.
 *
 * \param device    Pointer to SPI device struct that should be initialized.
 */
void sspi_master_setup_device(const struct sspi_device *device)
{
#if USART_SPI
	USART_SSPI->US_CR = US_CR_RXDIS | US_CR_TXDIS;			// disable transmitter and receiver
	USART_SSPI->US_BRGR = SystemCoreClock/device->clockFrequency;
	uint32_t mr = US_MR_USART_MODE_SPI_MASTER
					| US_MR_USCLKS_MCK
					| US_MR_CHRL_8_BIT
					| US_MR_CHMODE_NORMAL
					| US_MR_CLKO;
	if (device->spiMode & 2)
	{
		mr |= US_MR_CPOL;
	}
	if ((device->spiMode & 1) == 0)							// the bit is called CPHA but is actually NPCHA
	{
		mr |= US_MR_CPHA;
	}
	USART_SSPI->US_MR = mr;
	USART_SSPI->US_CR = US_CR_RSTRX | US_CR_RSTTX;			// reset transmitter and receiver (required - see datasheet)
	USART_SSPI->US_CR = US_CR_RXEN | US_CR_TXEN;			// enable transmitter and receiver
#else
	spi_reset(SSPI);
    SSPI->SPI_MR = SPI_MR_MSTR | SPI_MR_MODFDIS;

	// Set SPI mode, clock frequency, CS not active after transfer, delay between transfers
	uint16_t baud_div = (uint16_t)spi_calc_baudrate_div(device->clockFrequency, SystemCoreClock);
	uint32_t csr = SPI_CSR_SCBR(baud_div)				// Baud rate
					| device->bitsPerTransferControl	// Transfer bit width
					| SPI_CSR_DLYBCT(0);      			// Transfer delay
	if ((device->spiMode & 0x01) == 0)
	{
		csr |= SPI_CSR_NCPHA;
	}
	if (device->spiMode & 0x02)
	{
		csr |= SPI_CSR_CPOL;
	}
	SSPI->SPI_CSR[PERIPHERAL_CHANNEL_ID] = csr;
	spi_enable(SSPI);
#endif
}

/**
 * \brief Select the given device on the SPI bus.
 *
 * Set device specific setting and call board chip select.
 *
 * \param device  SPI device.
 *
 */
void sspi_select_device(const struct sspi_device *device)
{
#if SAM3XA
	spi_set_peripheral_chip_select_value(SSPI, spi_get_pcs(PERIPHERAL_CHANNEL_ID));
#endif

	// Enable the CS line
	digitalWrite(device->csPin, device->csPolarity);
}

/**
 * \brief Deselect the given device on the SPI bus.
 *
 * Call board chip deselect.
 *
 * \param device  SPI device.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
void sspi_deselect_device(const struct sspi_device *device)
{
	waitForTxEmpty();

	// Disable the CS line
	digitalWrite(device->csPin, !device->csPolarity);
}

/**
 * \brief Send and receive a sequence of bytes from an SPI device.
 *
 * \param tx_data   Data buffer to send.
 * \param rx_data   Data buffer to read.
 * \param len       Length of data to be read.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t sspi_transceive_packet(const uint8_t *tx_data, uint8_t *rx_data, size_t len)
{
	for (uint32_t i = 0; i < len; ++i)
	{
		uint32_t dOut = (tx_data == nullptr) ? 0x000000FF : (uint32_t)*tx_data++;
		if (waitForTxReady())
		{
			return SPI_ERROR_TIMEOUT;
		}

		// Write to transmit register
#if USART_SPI
		USART_SSPI->US_THR = dOut;
#else
		if (i + 1 == len)
		{
			dOut |= SPI_TDR_LASTXFER;
		}
		SSPI->SPI_TDR = dOut;
#endif

		// Some devices are transmit-only e.g. 12864 display, so don't wait for received data if we don't need to
		if (rx_data != nullptr)
		{
			// Wait for receive register
			if (waitForRxReady())
			{
				return SPI_ERROR_TIMEOUT;
			}

			// Get data from receive register
			const uint8_t dIn =
#if USART_SPI
					(uint8_t)USART_SSPI->US_RHR;
#else
					(uint8_t)SSPI->SPI_RDR;
#endif
			*rx_data++ = dIn;
		}
	}

	// If we didn't wait to receive, then we need to wait for transmit to finish and clear the receive buffer and
	if (rx_data == nullptr)
	{
		waitForTxEmpty();
#if USART_SPI
		(void)USART_SSPI->US_RHR;
#else
		(void)SSPI->SPI_RDR;
#endif

	}

	return SPI_OK;
}

#if SAM3XA
/**
 * \brief Send and receive a sequence of 16-bit words from an SPI device.
 *
 * \param tx_data   Data buffer to send.
 * \param rx_data   Data buffer to read.
 * \param len       Length of data to send and receive in 16-bit words.
 *
 * \pre SPI device must be selected with spi_select_device() first.
 */
spi_status_t sspi_transceive_packet16(const uint16_t *tx_data, uint16_t *rx_data, size_t len)
{
	for (uint32_t i = 0; i < len; ++i)
	{
		// Wait for transmit register empty
		if (waitForTxReady())
		{
			return SPI_ERROR_TIMEOUT;
		}

		// Write to transmit register
		uint32_t dOut = (tx_data == nullptr) ? 0x000000FF : (uint32_t)tx_data[i];
		if (i + 1 == len)
		{
			dOut |= SPI_TDR_LASTXFER;
		}
		SSPI->SPI_TDR = dOut;

		// Wait for receive register
		if (waitForRxReady())
		{
			return SPI_ERROR_TIMEOUT;
		}

		// Get data from receive register
		uint16_t dIn = (uint16_t)SSPI->SPI_RDR;
		if (rx_data != nullptr)
		{
			rx_data[i] = dIn;
		}
	}

	return SPI_OK;
}
#endif

#if defined(USE_SAM3X_DMAC)

void sspi_start_transmit_dma(Dmac *p_dmac, uint32_t ul_num, const void *src, uint32_t nb_bytes)
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
	desc.ul_destination_addr = (uint32_t)&(SSPI->SPI_TDR);
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

void sspi_start_receive_dma(Dmac *p_dmac, uint32_t ul_num, const void *dest, uint32_t nb_bytes)
{
	uint32_t cfg;
	dma_transfer_descriptor_t desc;

	// clear any potential overrun error
	cfg = SSPI->SPI_SR;

	// Turn the DMA channel off before configuring
	dmac_enable(p_dmac);
	dmac_channel_disable(p_dmac, ul_num);

	cfg = DMAC_CFG_SOD
		| DMAC_CFG_SRC_H2SEL
		| DMAC_CFG_SRC_PER(SPI_RX_IDX)
		| DMAC_CFG_FIFOCFG_ASAP_CFG;
	dmac_channel_set_configuration(p_dmac, ul_num, cfg);

	// Prepare DMA transfer
	desc.ul_source_addr = (uint32_t)&(SSPI->SPI_RDR);
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
