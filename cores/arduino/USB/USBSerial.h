/*
 * USBSerial.h
 *
 *  Created on: 18 Mar 2016
 *      Author: David
 */

#ifndef USBSERIAL_H_
#define USBSERIAL_H_

#include "Core.h"
#include "Stream.h"

// Serial over CDC

class SerialCDC : public Stream
{
private:
	size_t txBufsize;
	bool isConnected;
	Pin vBusPin;

public:
	SerialCDC() noexcept;

	void Start(Pin p_vBusPin) noexcept;
	void end(void) noexcept;

	int available() noexcept override;
	int read() noexcept override;
	size_t readBytes(char *buffer, size_t length) noexcept override;
	void flush() noexcept override;
	size_t write(uint8_t) noexcept override;
	size_t write(const uint8_t *buffer, size_t size) noexcept override;

	size_t write(const char *str) noexcept { return Print::write(str); }
    size_t write(const char *buffer, size_t size) noexcept { return write((const uint8_t *)buffer, size); }

	size_t canWrite() const noexcept override;	// Function added by DC42 so that we can tell how many characters we can write without blocking (for Duet)
	bool IsConnected() const noexcept;

	// Callback functions called from the cdc layer - not for general use
	void cdcSetConnected(bool b) noexcept;
	void cdcRxNotify() noexcept;
	void cdcTxEmptyNotify() noexcept;
};

extern SerialCDC SerialUSB;

#endif /* USBSERIAL_H_ */
