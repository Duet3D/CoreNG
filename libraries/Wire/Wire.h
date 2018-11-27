/*
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

#ifndef TwoWire_h
#define TwoWire_h

#include "compiler.h"

#if SAME70
// TWI disabled for now
#else

#include "Stream.h"
#include "variant.h"

#define BUFFER_LENGTH 32

#if SAME70
# define Twi Twihs
#endif

class TwoWire
{
public:
	struct ErrorCounts
	{
		uint32_t naks;
		uint32_t sendTimeouts;
		uint32_t recvTimeouts;
		uint32_t finishTimeouts;

		void Clear();
	};

	typedef uint32_t (*WaitForStatusFunc)(Twi *twi, uint32_t bitsToWaitFor);

	TwoWire(Twi *twi, void(*begin_cb)(void));

	void BeginMaster(uint32_t clockFrequency);
	size_t Transfer(uint16_t address, uint8_t *buffer, size_t numToWrite, size_t numToRead, WaitForStatusFunc statusWaitFunc = DefaultWaitForStatusFunc);
	ErrorCounts GetErrorCounts(bool clear);

	static uint32_t DefaultWaitForStatusFunc(Twi *twi, uint32_t bitsToWaitFor);

private:
	bool WaitForStatus(uint32_t statusBit, uint32_t& timeoutErrorCounter, WaitForStatusFunc statusWaitFunc);
	bool WaitTransferComplete(WaitForStatusFunc statusWaitFunc);
	bool WaitByteSent(WaitForStatusFunc statusWaitFunc);
	bool WaitByteReceived(WaitForStatusFunc statusWaitFunc);

	Twi *twi;							// TWI instance
	void (*onBeginCallback)(void);		// called before initialization
	ErrorCounts errorCounts;			// error counts
};

#if WIRE_INTERFACES_COUNT > 0
extern TwoWire Wire;
#endif
#if WIRE_INTERFACES_COUNT > 1
extern TwoWire Wire1;
#endif

#endif

#endif

