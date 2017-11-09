/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Wire.h"

#if SAME70
// TWI disabled for now
#else

#if SAME70
#include "twihs/twihs.h"
#else
#include "twi/twi.h"
#endif

#include <cstring>

/**
 * \brief Configures a TWI peripheral to operate in master mode, at the given
 * frequency (in Hz). The duty cycle of the TWI clock is set to 50%.
 * \param pTwi  Pointer to an Twi instance.
 * \param twck  Desired TWI clock frequency.
 * \param mck  Master clock frequency.
 */
static void TWI_ConfigureMaster(Twi* pTwi, uint32_t dwTwCk, uint32_t dwMCk)
{
	twi_options_t opt;

	opt.speed = dwTwCk;
	opt.master_clk = dwMCk;
	opt.chip = opt.smbus = 0;
	twi_master_init(pTwi, &opt);
}

/**
 * \brief Check if a byte have been received from TWI.
 * \param pTwi  Pointer to an Twi instance.
 * \return 1 if a byte has been received and can be read on the given TWI
 * peripheral; otherwise, returns 0. This function resets the status register.
 */
inline uint8_t TWI_ByteReceived(Twi *pTwi)
{
    return ((pTwi->TWI_SR & TWI_SR_RXRDY) == TWI_SR_RXRDY);
}

/**
 * \brief Check if a byte have been sent to TWI.
 * \param pTwi  Pointer to an Twi instance.
 * \return 1 if a byte has been sent  so another one can be stored for
 * transmission; otherwise returns 0. This function clears the status register.
 */
inline uint8_t TWI_ByteSent(Twi *pTwi)
{
    return ((pTwi->TWI_SR & TWI_SR_TXRDY) == TWI_SR_TXRDY);
}

/**
 * \brief Check if current transmission is complete.
 * \param pTwi  Pointer to an Twi instance.
 * \return  1 if the current transmission is complete (the STOP has been sent);
 * otherwise returns 0.
 */
inline uint8_t TWI_TransferComplete(Twi *pTwi)
{
    return ((pTwi->TWI_SR & TWI_SR_TXCOMP) == TWI_SR_TXCOMP);
}

/**
 * \brief Starts a read operation on the TWI bus with the specified slave, it returns
 * immediately. Data must then be read using TWI_ReadByte() whenever a byte is
 * available (poll using TWI_ByteReceived()).
 * \param pTwi  Pointer to an Twi instance.
 * \param address  Slave address on the bus.
 * \param iaddress  Optional internal address bytes.
 * \param isize  Number of internal address bytes.
 */
static void TWI_StartRead(
    Twi *pTwi,
    uint8_t address,
    uint32_t iaddress,
    uint8_t isize)
{
//	assert( pTwi != NULL ) ;
//	assert( (address & 0x80) == 0 ) ;
//	assert( (iaddress & 0xFF000000) == 0 ) ;
//	assert( isize < 4 ) ;

    /* Set slave address and number of internal address bytes. */
    pTwi->TWI_MMR = 0;
    pTwi->TWI_MMR = (isize << 8) | TWI_MMR_MREAD | (address << 16);

    /* Set internal address bytes */
    pTwi->TWI_IADR = 0;
    pTwi->TWI_IADR = iaddress;

    /* Send START condition */
    pTwi->TWI_CR = TWI_CR_START;
}

/**
 * \brief Starts a write operation on the TWI to access the selected slave, then
 *  returns immediately. A byte of data must be provided to start the write;
 * other bytes are written next.
 * after that to send the remaining bytes.
 * \param pTwi  Pointer to an Twi instance.
 * \param address  Address of slave to acccess on the bus.
 * \param iaddress  Optional slave internal address.
 * \param isize  Number of internal address bytes.
 * \param byte  First byte to send.
 */
static void TWI_StartWrite(
    Twi *pTwi,
    uint8_t address,
    uint32_t iaddress,
    uint8_t isize,
    uint8_t byte)
{
//	assert( pTwi != NULL ) ;
//	assert( (address & 0x80) == 0 ) ;
//	assert( (iaddress & 0xFF000000) == 0 ) ;
//	assert( isize < 4 ) ;

    /* Set slave address and number of internal address bytes. */
    pTwi->TWI_MMR = 0;
    pTwi->TWI_MMR = (isize << 8) | (address << 16);

    /* Set internal address bytes. */
    pTwi->TWI_IADR = 0;
    pTwi->TWI_IADR = iaddress;

    /* Write first byte to send.*/
    twi_write_byte(pTwi, byte);
}

/**
 * \brief  Sends a STOP condition. STOP Condition is sent just after completing
 *  the current byte transmission in master read mode.
 * \param pTwi  Pointer to an Twi instance.
 */
inline void TWI_SendSTOPCondition(Twi *pTwi)
{
    pTwi->TWI_CR |= TWI_CR_STOP;
}

/**
 * \brief Sends a STOP condition on the TWI.
 * \param pTwi  Pointer to an Twi instance.
 */
void TWI_Stop( Twi *pTwi )
{
    pTwi->TWI_CR = TWI_CR_STOP;
}

// Returns true if the TXRDY bit (ready to transmit data) is set in the given status register value
inline bool TWI_STATUS_TXRDY(uint32_t status)
{
	return (status & TWI_SR_TXRDY) == TWI_SR_TXRDY;
}

// Returns true if the RXRDY bit (ready to receive data) is set in the given status register value
inline bool TWI_STATUS_RXRDY(uint32_t status)
{
	return (status & TWI_SR_RXRDY) == TWI_SR_RXRDY;
}

// Returns true if the TXCOMP bit (transfer complete) is set in the given status register value
inline bool TWI_STATUS_TXCOMP(uint32_t status)
{
	return (status & TWI_SR_TXCOMP) == TWI_SR_TXCOMP;
}

static inline bool TWI_FailedAcknowledge(Twi *pTwi) {
	return pTwi->TWI_SR & TWI_SR_NACK;
}

static inline bool TWI_WaitTransferComplete(Twi *_twi, uint32_t _timeout) {
	while (!TWI_TransferComplete(_twi)) {
		if (TWI_FailedAcknowledge(_twi))
		{
			return false;
		}
		if (--_timeout == 0)
		{
			return false;
		}
	}
	return true;
}

static inline bool TWI_WaitByteSent(Twi *_twi, uint32_t _timeout)
{
	while (!TWI_ByteSent(_twi))
	{
		if (TWI_FailedAcknowledge(_twi))
		{
			return false;
		}
		if (--_timeout == 0)
		{
			return false;
		}
	}
	return true;
}

static inline bool TWI_WaitByteReceived(Twi *_twi, uint32_t _timeout)
{
	while (!TWI_ByteReceived(_twi))
	{
		if (TWI_FailedAcknowledge(_twi))
		{
			return false;
		}
		if (--_timeout == 0)
		{
			return false;
		}
	}
	return true;
}

static inline bool TWI_STATUS_SVREAD(uint32_t status) {
	return (status & TWI_SR_SVREAD) == TWI_SR_SVREAD;
}

static inline bool TWI_STATUS_SVACC(uint32_t status) {
	return (status & TWI_SR_SVACC) == TWI_SR_SVACC;
}

#if 0	// unused
static inline bool TWI_STATUS_GACC(uint32_t status) {
	return (status & TWI_SR_GACC) == TWI_SR_GACC;
}
#endif

static inline bool TWI_STATUS_EOSACC(uint32_t status) {
	return (status & TWI_SR_EOSACC) == TWI_SR_EOSACC;
}

static inline bool TWI_STATUS_NACK(uint32_t status) {
	return (status & TWI_SR_NACK) == TWI_SR_NACK;
}

TwoWire::TwoWire(Twi *_twi, void(*_beginCb)(void)) :
	rxBufferIndex(0), rxBufferLength(0), txAddress(0), txBufferLength(0),
	srvBufferIndex(0), srvBufferLength(0), onRequestCallback(0), onReceiveCallback(0), onBeginCallback(_beginCb), twi(_twi),
	status(UNINITIALIZED)
{
}

void TwoWire::begin(void) {
	if (onBeginCallback)
		onBeginCallback();

	// Disable PDC channel
	twi->TWI_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	TWI_ConfigureMaster(twi, TWI_CLOCK, VARIANT_MCK);
	status = MASTER_IDLE;
}

void TwoWire::begin(uint8_t address) {
	if (onBeginCallback != nullptr)
	{
		onBeginCallback();
	}

	// Disable PDC channel
	twi->TWI_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	twi_slave_init(twi, address);
	status = SLAVE_IDLE;
	twi_enable_interrupt(twi, TWI_IER_SVACC);
	//| TWI_IER_RXRDY | TWI_IER_TXRDY	| TWI_IER_TXCOMP);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	if (quantity > BUFFER_LENGTH)
	{
		quantity = BUFFER_LENGTH;
	}

	// perform blocking read into buffer
	int readed = 0;
	TWI_StartRead(twi, address, 0, 0);
	do {
		// Stop condition must be set during the reception of last byte
		if (readed + 1 == quantity)
		{
			TWI_SendSTOPCondition( twi);
		}

		TWI_WaitByteReceived(twi, RECV_TIMEOUT);
		rxBuffer[readed++] = twi_read_byte(twi);
	} while (readed < quantity);
	TWI_WaitTransferComplete(twi, RECV_TIMEOUT);

	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = readed;

	return readed;
}

void TwoWire::beginTransmission(uint8_t address) {
	status = MASTER_SEND;

	// save address of target and empty buffer
	txAddress = address;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
	beginTransmission((uint8_t) address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop) {
	// transmit buffer (blocking)
	TWI_StartWrite(twi, txAddress, 0, 0, txBuffer[0]);
	TWI_WaitByteSent(twi, XMIT_TIMEOUT);
	int sent = 1;
	while (sent < txBufferLength) {
		twi_write_byte(twi, txBuffer[sent++]);
		TWI_WaitByteSent(twi, XMIT_TIMEOUT);
	}
	TWI_Stop( twi);
	TWI_WaitTransferComplete(twi, XMIT_TIMEOUT);

	// empty buffer
	txBufferLength = 0;

	status = MASTER_IDLE;
	return sent;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

size_t TwoWire::write(uint8_t data) {
	if (status == MASTER_SEND) {
		if (txBufferLength >= BUFFER_LENGTH)
		{
			return 0;
		}
		txBuffer[txBufferLength++] = data;
		return 1;
	} else {
		if (srvBufferLength >= BUFFER_LENGTH)
		{
			return 0;
		}
		srvBuffer[srvBufferLength++] = data;
		return 1;
	}
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
	if (status == MASTER_SEND) {
		for (size_t i = 0; i < quantity; ++i) {
			if (txBufferLength >= BUFFER_LENGTH)
			{
				return i;
			}
			txBuffer[txBufferLength++] = data[i];
		}
	} else {
		for (size_t i = 0; i < quantity; ++i) {
			if (srvBufferLength >= BUFFER_LENGTH)
			{
				return i;
			}
			srvBuffer[srvBufferLength++] = data[i];
		}
	}
	return quantity;
}

int TwoWire::available(void) {
	return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void) {
	if (rxBufferIndex < rxBufferLength)
	{
		return rxBuffer[rxBufferIndex++];
	}
	return -1;
}

int TwoWire::peek(void) {
	if (rxBufferIndex < rxBufferLength)
	{
		return rxBuffer[rxBufferIndex];
	}
	return -1;
}

void TwoWire::flush(void) {
	// Do nothing, use endTransmission(..) to force
	// data transfer.
}

void TwoWire::onReceive(void(*function)(int)) {
	onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void)) {
	onRequestCallback = function;
}

void TwoWire::onService(void) {
	// Retrieve interrupt status
	uint32_t sr = twi_get_interrupt_status(twi);

	if (status == SLAVE_IDLE && TWI_STATUS_SVACC(sr)) {
		twi_disable_interrupt(twi, TWI_IDR_SVACC);
		twi_enable_interrupt(twi, TWI_IER_RXRDY | TWI_IER_GACC | TWI_IER_NACK
				| TWI_IER_EOSACC | TWI_IER_SCL_WS | TWI_IER_TXCOMP);

		srvBufferLength = 0;
		srvBufferIndex = 0;

		// Detect if we should go into RECV or SEND status
		// SVREAD==1 means *master* reading -> SLAVE_SEND
		if (!TWI_STATUS_SVREAD(sr)) {
			status = SLAVE_RECV;
		} else {
			status = SLAVE_SEND;

			// Alert calling program to generate a response ASAP
			if (onRequestCallback)
				onRequestCallback();
			else
				// create a default 1-byte response
				write((uint8_t) 0);
		}
	}

	if (status != SLAVE_IDLE) {
		if (TWI_STATUS_TXCOMP(sr) && TWI_STATUS_EOSACC(sr)) {
			if (status == SLAVE_RECV && onReceiveCallback) {
				// Copy data into rxBuffer
				// (allows to receive another packet while the
				// user program reads actual data)
				for (uint8_t i = 0; i < srvBufferLength; ++i)
					rxBuffer[i] = srvBuffer[i];
				rxBufferIndex = 0;
				rxBufferLength = srvBufferLength;

				// Alert calling program
				onReceiveCallback( rxBufferLength);
			}

			// Transfer completed
			twi_enable_interrupt(twi, TWI_SR_SVACC);
			twi_disable_interrupt(twi, TWI_IDR_RXRDY | TWI_IDR_GACC | TWI_IDR_NACK
					| TWI_IDR_EOSACC | TWI_IDR_SCL_WS | TWI_IER_TXCOMP);
			status = SLAVE_IDLE;
		}
	}

	if (status == SLAVE_RECV) {
		if (TWI_STATUS_RXRDY(sr)) {
			if (srvBufferLength < BUFFER_LENGTH)
			{
				srvBuffer[srvBufferLength++] = twi_read_byte(twi);
			}
		}
	}

	if (status == SLAVE_SEND) {
		if (TWI_STATUS_TXRDY(sr) && !TWI_STATUS_NACK(sr)) {
			uint8_t c = 'x';
			if (srvBufferIndex < srvBufferLength)
			{
				c = srvBuffer[srvBufferIndex++];
			}
			twi_write_byte(twi, c);
		}
	}
}

#if WIRE_INTERFACES_COUNT > 0
static void Wire_Init(void) {
	pmc_enable_periph_clk(WIRE_INTERFACE_ID);
	ConfigurePin(g_APinDescription[APIN_WIRE_SDA]);
	ConfigurePin(g_APinDescription[APIN_WIRE_SCL]);

	NVIC_DisableIRQ(TWI1_IRQn);
	NVIC_ClearPendingIRQ(TWI1_IRQn);
	NVIC_EnableIRQ(TWI1_IRQn);
}

TwoWire Wire = TwoWire(WIRE_INTERFACE, Wire_Init);

void WIRE_ISR_HANDLER(void) {
	Wire.onService();
}
#endif

#if WIRE_INTERFACES_COUNT > 1
static void Wire1_Init(void) {
	pmc_enable_periph_clk(WIRE1_INTERFACE_ID);
	ConfigurePin(g_APinDescription[APIN_WIRE1_SDA]);
	ConfigurePin(g_APinDescription[APIN_WIRE1_SCL]);

	NVIC_DisableIRQ(TWI0_IRQn);
	NVIC_ClearPendingIRQ(TWI0_IRQn);
	NVIC_EnableIRQ(TWI0_IRQn);
}

TwoWire Wire1 = TwoWire(WIRE1_INTERFACE, Wire1_Init);

void WIRE1_ISR_HANDLER(void) {
	Wire1.onService();
}
#endif

#endif

// End
