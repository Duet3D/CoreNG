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

#ifndef _RING_BUFFER_
#define _RING_BUFFER_

#include <cstdint>
#include <cstddef>

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer, in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
const size_t SERIAL_BUFFER_SIZE = 512;

class RingBuffer
{
  public:
    volatile uint8_t _aucBuffer[SERIAL_BUFFER_SIZE];
    volatile size_t _iHead;
    volatile size_t _iTail;

  public:
    RingBuffer();
    void store_char(uint8_t c);
    size_t storeBlock(const uint8_t *data, size_t len);
    size_t roomLeft() const;
};

inline void RingBuffer::store_char(uint8_t c)
{
  size_t i = (_iHead + 1) % SERIAL_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != _iTail)
  {
    _aucBuffer[_iHead] = c;
    _iHead = i;
  }
  else
  {
	  _aucBuffer[(_iHead - 1) % SERIAL_BUFFER_SIZE] = 0x7F;		// replace the previous character by DEL to signal an overflow error
  }
}

inline size_t RingBuffer::roomLeft() const
{
	return (_iTail + (SERIAL_BUFFER_SIZE - 1) - _iHead) % SERIAL_BUFFER_SIZE;
}

#endif /* _RING_BUFFER_ */
