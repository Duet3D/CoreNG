/*
  Stream.h - base class for character-based streams.
  Copyright (c) 2010 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Stream_h
#define Stream_h

#include <inttypes.h>
#include "Print.h"

class Stream : public Print
{
public:
	Stream() noexcept { }

	virtual int available() noexcept = 0;
	virtual int read() noexcept = 0;
	virtual void flush() noexcept = 0;
	virtual size_t canWrite() const noexcept { return 1; }	// DC42 added for Duet

	virtual size_t readBytes( char *buffer, size_t length) noexcept; // read chars from stream into buffer
	size_t readBytes( uint8_t *buffer, size_t length) noexcept { return readBytes((char *)buffer, length); }
};

#endif
