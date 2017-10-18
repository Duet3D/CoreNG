/*
 * USBSerial.h
 *
 *  Created on: 18 Mar 2016
 *      Author: David
 */

#ifndef USBSERIAL_H_
#define USBSERIAL_H_

#include "Stream.h"

// Serial over CDC

class SerialCDC : public Stream
{
private:
	size_t txBufsize;
	bool isConnected;

public:
	SerialCDC();

	void begin(uint32_t baud_count);
	void begin(uint32_t baud_count, uint8_t config);
	void end(void);

	int available() override;
	int peek() override;
	int read() override;
	void flush() override;
	size_t write(uint8_t) override;
	size_t write(const uint8_t *buffer, size_t size) override;

	size_t write(const char *str) { return Print::write(str); }
    size_t write(const char *buffer, size_t size) { return write((const uint8_t *)buffer, size); }

	size_t canWrite() const override;	// Function added by DC42 so that we can tell how many characters we can write without blocking (for Duet)
	operator bool() const;

	// Callback functions called from the cdc layer - not for general use
	void cdcSetConnected(bool b);
	void cdcRxNotify();
	void cdcTxEmptyNotify();
};

extern SerialCDC SerialUSB;

#endif /* USBSERIAL_H_ */
