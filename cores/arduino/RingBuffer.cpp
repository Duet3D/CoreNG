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

#include "RingBuffer.h"
#include <string.h>

RingBuffer::RingBuffer( void )
{
    memset((void *)_aucBuffer, 0, SERIAL_BUFFER_SIZE) ;
    _iHead=0 ;
    _iTail=0 ;
}

// Store a block of data. Returns the number of bytes actually stored, which may be less than the number requested.
size_t RingBuffer::storeBlock(const uint8_t *data, size_t len)
{
	const size_t room = roomLeft();
	if (room < len)
	{
		len = room;
	}
	if (len != 0)
	{
		const size_t roomToEnd = SERIAL_BUFFER_SIZE - _iHead;
		if (roomToEnd <= len)
		{
			memcpy((void*)(_aucBuffer + _iHead), data, roomToEnd);
			memcpy((void*)_aucBuffer, data + roomToEnd, len - roomToEnd);
			_iHead = len - roomToEnd;
		}
		else
		{
			memcpy((void*)(_aucBuffer + _iHead), data, len);
			_iHead += len;
		}
	}
	return len;
}

// End
