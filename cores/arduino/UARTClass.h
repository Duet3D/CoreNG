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

#ifndef _UART_CLASS_
#define _UART_CLASS_

#include "HardwareSerial.h"
#include "RingBuffer.h"

#if SAM4E || SAME70
#include "component/usart.h"
#else
#include "component/component_usart.h"
#endif

#define SERIAL_8N1 UARTClass::Mode_8N1
#define SERIAL_8E1 UARTClass::Mode_8E1
#define SERIAL_8O1 UARTClass::Mode_8O1
#define SERIAL_8M1 UARTClass::Mode_8M1
#define SERIAL_8S1 UARTClass::Mode_8S1


class UARTClass : public HardwareSerial
{
  public:
	typedef void (*InterruptCallbackFn)(UARTClass*) noexcept;
	typedef void (*OnBeginFunction)() noexcept;

	union Errors
	{
		uint32_t all;
		uint32_t uartOverrun : 11,
				 framing : 11,
				 bufferOverrun : 10;

		Errors() noexcept { all = 0; }
	};

    enum UARTModes {
      Mode_8N1 = US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO,
      Mode_8E1 = US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_EVEN,
      Mode_8O1 = US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_ODD,
      Mode_8M1 = US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_MARK,
      Mode_8S1 = US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_SPACE,
    };

    UARTClass(Uart* pUart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer* pRx_buffer, RingBuffer* pTx_buffer, OnBeginFunction onBegin) noexcept;

    void begin(const uint32_t dwBaudRate) noexcept;
    void begin(const uint32_t dwBaudRate, const UARTModes config) noexcept;
    void end(void) noexcept;
    int available(void) noexcept;
    int availableForWrite(void) noexcept;
    int peek(void) noexcept;
    int read(void) noexcept;
    void flush(void) noexcept;
    size_t write(const uint8_t c) noexcept override;
    size_t write(const uint8_t *buffer, size_t size) noexcept override;

    using Print::write; // pull in write(str) and write(buf, size) from Print
    size_t canWrite(void) const noexcept override;	//***** DC42 added for Duet

    void setInterruptPriority(uint32_t priority) noexcept;
    uint32_t getInterruptPriority() noexcept;

    void IrqHandler(void) noexcept;

    operator bool() noexcept { return true; }; // UART always active

    InterruptCallbackFn SetInterruptCallback(InterruptCallbackFn f) noexcept;

	// Get and clear the errors
	Errors GetAndClearErrors() noexcept;

  protected:
    void init(const uint32_t dwBaudRate, const uint32_t config) noexcept;

    OnBeginFunction onBeginFunc;

    RingBuffer * const _rx_buffer;
    RingBuffer * const _tx_buffer;

    Uart* const _pUart;
    const IRQn_Type _dwIrq;
    const uint32_t _dwId;
    size_t numInterruptBytesMatched;
    InterruptCallbackFn interruptCallback;
    Errors errors;

    static constexpr uint8_t interruptSeq[2] = { 0xF0, 0x0F };
};

#endif // _UART_CLASS_
