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

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>

#include "Stream.h"

class HardwareSerial : public Stream
{
  public:
    virtual void begin(unsigned long) noexcept;
    virtual void end() noexcept;
    virtual int available(void) noexcept = 0;
    virtual int peek(void) noexcept = 0;
    virtual int read(void) noexcept = 0;
    virtual void flush(void) noexcept = 0;
    size_t write(uint8_t) noexcept override = 0;
    using Print::write; // pull in write(str) and write(buf, size) from Print
    virtual operator bool() noexcept = 0;
};

extern void serialEventRun(void) noexcept __attribute__((weak));

#endif
