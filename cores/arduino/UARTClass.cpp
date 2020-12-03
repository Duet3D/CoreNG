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

#include "asf.h"
#include "UARTClass.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>		// for std::swap

#include "WMath.h"

// Constructors ////////////////////////////////////////////////////////////////

UARTClass::UARTClass(Uart *pUart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer *pRx_buffer, RingBuffer *pTx_buffer, OnBeginFunction onBegin) noexcept
	: onBeginFunc(onBegin),
	  _rx_buffer(pRx_buffer), _tx_buffer(pTx_buffer), _pUart(pUart), _dwIrq(dwIrq), _dwId(dwId),
	  numInterruptBytesMatched(0), interruptCallback(nullptr)
{
}

// Public Methods //////////////////////////////////////////////////////////////

void UARTClass::begin(const uint32_t dwBaudRate) noexcept
{
	begin(dwBaudRate, Mode_8N1);
}

void UARTClass::begin(const uint32_t dwBaudRate, const UARTModes config) noexcept
{
	uint32_t modeReg = static_cast<uint32_t>(config) & 0x00000E00;
	init(dwBaudRate, modeReg | UART_MR_CHMODE_NORMAL);
}

void UARTClass::init(const uint32_t dwBaudRate, const uint32_t modeReg) noexcept
{
	// Configure PMC
	pmc_enable_periph_clk( _dwId );
	onBeginFunc();

#if !SAME70
	// Disable PDC channel
	_pUart->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
#endif

	// Reset and disable receiver and transmitter
	_pUart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	// Configure mode
	_pUart->UART_MR = modeReg;

	// Configure baudrate (asynchronous, no oversampling)
	const uint32_t br16 = dwBaudRate * 16;
	_pUart->UART_BRGR = (SystemPeripheralClock() + (br16/2) - 1) / br16;

	// Make sure both ring buffers are initialized back to empty.
	_rx_buffer->_iHead = _rx_buffer->_iTail = 0;
	_tx_buffer->_iHead = _tx_buffer->_iTail = 0;

	// Configure interrupts
	_pUart->UART_IDR = 0xFFFFFFFF;
	_pUart->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

	// Enable UART interrupt in NVIC
	NVIC_EnableIRQ(_dwIrq);

	// Enable receiver and transmitter
	errors.all = 0;
	_pUart->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

void UARTClass::end( void ) noexcept
{
	// Clear any received data
	_rx_buffer->_iHead = _rx_buffer->_iTail;

	// Wait for any outstanding data to be sent
	flush();

	// Disable UART interrupt in NVIC
	NVIC_DisableIRQ( _dwIrq );

	pmc_disable_periph_clk( _dwId );
}

void UARTClass::setInterruptPriority(uint32_t priority) noexcept
{
	NVIC_SetPriority(_dwIrq, priority & 0x0F);
}

uint32_t UARTClass::getInterruptPriority() noexcept
{
	return NVIC_GetPriority(_dwIrq);
}

int UARTClass::available( void ) noexcept
{
	return (SERIAL_BUFFER_SIZE + _rx_buffer->_iHead - _rx_buffer->_iTail) % SERIAL_BUFFER_SIZE;
}

int UARTClass::availableForWrite(void) noexcept
{
	size_t head = _tx_buffer->_iHead;
	size_t tail = _tx_buffer->_iTail;
	if (head >= tail)
	{
		return SERIAL_BUFFER_SIZE - 1 - head + tail;
	}
	return tail - head - 1;
}

int UARTClass::peek( void ) noexcept
{
	if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
	{
		return -1;
	}

	return _rx_buffer->_aucBuffer[_rx_buffer->_iTail];
}

int UARTClass::read( void ) noexcept
{
	// if the head isn't ahead of the tail, we don't have any characters
	if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
	{
		return -1;
	}

	uint8_t uc = _rx_buffer->_aucBuffer[_rx_buffer->_iTail];
	_rx_buffer->_iTail = (_rx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE;
	return uc;
}

void UARTClass::flush( void ) noexcept
{
	while (_tx_buffer->_iHead != _tx_buffer->_iTail) { }			// wait for transmit data to be sent
	// Wait for transmission to complete
	while ((_pUart->UART_SR & UART_SR_TXRDY) != UART_SR_TXRDY) { }
}

size_t UARTClass::write( const uint8_t uc_data ) noexcept
{
	// Is the hardware currently busy?
	if ((_pUart->UART_SR & UART_SR_TXRDY) != UART_SR_TXRDY || _tx_buffer->_iTail != _tx_buffer->_iHead)
	{
		// If busy we buffer
		const size_t hn = (_tx_buffer->_iHead + 1) % SERIAL_BUFFER_SIZE;
		while (_tx_buffer->_iTail == hn) { }						// spin locks if we're about to overwrite the buffer. This continues once the data is sent

		_tx_buffer->_aucBuffer[_tx_buffer->_iHead] = uc_data;
		_tx_buffer->_iHead = hn;
		// Make sure TX interrupt is enabled
		_pUart->UART_IER = UART_IER_TXRDY;
	}
	else
	{
		// Bypass buffering and send character directly
		_pUart->UART_THR = uc_data;
	}
	return 1;
}

size_t UARTClass::write(const uint8_t *buffer, size_t size) noexcept
{
	size_t ret = size;
	while (size != 0)
	{
		size_t written = _tx_buffer->storeBlock(buffer, size);
		buffer += written;
		size -= written;
	    _pUart->UART_IER = UART_IER_TXRDY;
	}
	return ret;
}

size_t UARTClass::canWrite() const noexcept
{
	return _tx_buffer->roomLeft();		// we may also be able to write 1 more byte direct to the UART, but this is close enough
}

void UARTClass::IrqHandler() noexcept
{
	const uint32_t status = _pUart->UART_SR;

	// Did we receive data?
	if ((status & UART_SR_RXRDY) != 0)
	{
		const uint8_t c = _pUart->UART_RHR;
		if (c == interruptSeq[numInterruptBytesMatched])
		{
			++numInterruptBytesMatched;
			if (numInterruptBytesMatched == ARRAY_SIZE(interruptSeq))
			{
				numInterruptBytesMatched = 0;
				if (interruptCallback != nullptr)
				{
				  interruptCallback(this);
				}
			}
		}
		else
		{
			numInterruptBytesMatched = 0;
		}
		if (!_rx_buffer->store_char(c))
		{
			++errors.bufferOverrun;
		}
	}

	// Do we need to keep sending data?
	if ((status & UART_SR_TXRDY) != 0)
	{
		if (_tx_buffer->_iTail != _tx_buffer->_iHead)
		{
			_pUart->UART_THR = _tx_buffer->_aucBuffer[_tx_buffer->_iTail];
			_tx_buffer->_iTail = (_tx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE;
		}
		else
		{
			// Mask off transmit interrupt so we don't get it anymore
			_pUart->UART_IDR = UART_IDR_TXRDY;
		}
	}

	// Acknowledge errors
	if ((status & (UART_SR_OVRE | UART_SR_FRAME)) != 0)
	{
		if (status & UART_SR_OVRE)
		{
			++errors.uartOverrun;
		}
		if (status & UART_SR_FRAME)
		{
			++errors.framing;
		}
		_pUart->UART_CR = UART_CR_RSTSTA;
		_rx_buffer->store_char(0x7F);				// store a DEL character so that the receiving process knows there has been an error
	}
}

UARTClass::InterruptCallbackFn UARTClass::SetInterruptCallback(InterruptCallbackFn f) noexcept
{
	InterruptCallbackFn ret = interruptCallback;
	interruptCallback = f;
	return ret;
}

// Get and clear the errors
UARTClass::Errors UARTClass::GetAndClearErrors() noexcept
{
	Errors errs;
	std::swap(errs, errors);
	return errs;
}

// End
