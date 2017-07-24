/*
 * USBSerial.cpp
 *
 *  Created on: 18 Mar 2016
 *      Author: David
 */

#include "USBSerial.h"
#include "conf_usb.h"		// include this to check that the signatures of the callback functions are correct
#include "udi_cdc.h"		// Atmel CDC module
#include "udc.h"

#ifdef __SAM4E8E__
#include "WInterrupts.h"

void core_vbus_off(void*);
#endif

// SerialCDC members

SerialCDC::SerialCDC() : /* _cdc_tx_buffer(), */ txBufsize(1), isConnected(false)
{
#ifdef __SAM4E8E__
	attachInterrupt(USB_VBUS_PIN, core_vbus_off, FALLING, nullptr);
#endif
}

void SerialCDC::begin(uint32_t baud_count)
{
	// suppress "unused parameter" warning
	(void)baud_count;
	udc_start();
}

void SerialCDC::begin(uint32_t baud_count, uint8_t config)
{
	// suppress "unused parameter" warning
	(void)baud_count;
	(void)config;
	udc_start();
}

void SerialCDC::end()
{
	isConnected = false;
	udc_stop();
}

int SerialCDC::available()
{
	return (isConnected) ? udi_cdc_get_nb_received_data() : 0;
}

int SerialCDC::peek()
{
	return -1;				// not supported
}

int SerialCDC::read()
{
	return (udi_cdc_is_rx_ready()) ? udi_cdc_getc() : -1;
}

void SerialCDC::flush(void)
{
	while (isConnected && udi_cdc_get_free_tx_buffer() < txBufsize) {}
}

size_t SerialCDC::write(uint8_t c)
{
	if (isConnected)
	{
		udi_cdc_putc(c);
	}
	return 1;
}

// Non-blocking write to USB. Returns number of bytes written.
size_t SerialCDC::write(const uint8_t *buffer, size_t size)
{
	if (isConnected && size != 0)
	{
		size_t remaining = udi_cdc_write_buf(buffer, size);
		return size = remaining;
	}
	return size;
}

size_t SerialCDC::canWrite() const
{
	return (isConnected) ? udi_cdc_get_free_tx_buffer() : 0;
}

SerialCDC::operator bool() const
{
	return isConnected;
}

void SerialCDC::cdcSetConnected(bool b)
{
	isConnected = b;
}

void SerialCDC::cdcRxNotify()
{
	// nothing here until we use a Rx buffer
}

void SerialCDC::cdcTxEmptyNotify()
{
	// If we haven't yet found out how large the transmit buffer is, find out now
	if (txBufsize == 1)
	{
		txBufsize = udi_cdc_get_free_tx_buffer();
	}
}

// Declare the Serial USB device
SerialCDC SerialUSB;

// Callback glue functions, all called from the USB ISR

// This is called when we are plugged in and connect to a host
extern "C" bool core_cdc_enable(uint8_t port)
{
	SerialUSB.cdcSetConnected(true);
	return true;
}

// This is called when we get disconnected from the host
extern "C" void core_cdc_disable(uint8_t port)
{
	SerialUSB.cdcSetConnected(false);
}

// This is called when data has been received
extern "C" void core_cdc_rx_notify(uint8_t port)
{
	SerialUSB.cdcRxNotify();
}

// This is called when the transmit buffer has been emptied
extern "C" void core_cdc_tx_empty_notify(uint8_t port)
{
	SerialUSB.cdcTxEmptyNotify();
}

#ifdef __SAM4E8E__
// On the SAM4E there is only a GPIO pin available to monitor the VBUS state
void core_vbus_off(void*)
{
	SerialUSB.cdcSetConnected(false);
}
#endif

// End
