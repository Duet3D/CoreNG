/**
 * \file
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#undef __SOFTFP__				// should not be defined anyway, but Eclipse thinks it is

#include "same70.h"
#include "system_same70.h"
#include "wdt/wdt.h"
#include "Flash.h"
#include "Reset.h"

#if __FPU_USED /* CMSIS defined value to indicate usage of FPU */
#include "fpu/fpu.h"
#endif

extern void __libc_init_array(void);
extern void init(void);
extern void UrgentInit(void);
extern __attribute__((noreturn)) void AppMain();

/* Initialize segments */
extern uint32_t _sfixed;
//extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _szero_nocache;
extern uint32_t _ezero_nocache;
//extern uint32_t _sstack;
extern uint32_t _estack;
extern uint32_t _firmware_crc;

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M7 core handlers */
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void SUPC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RSTC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void RTT_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EFC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_PIOC_INSTANCE_
void PIOC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_PIOC_INSTANCE_ */
void USART0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USART2_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PIOD_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_PIOE_INSTANCE_
void PIOE_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_PIOE_INSTANCE_ */
#ifdef _SAME70_HSMCI_INSTANCE_
void HSMCI_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_HSMCI_INSTANCE_ */
void TWIHS0_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TWIHS1_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SPI0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SSC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_TC1_INSTANCE_
void TC3_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TC1_INSTANCE_ */
#ifdef _SAME70_TC1_INSTANCE_
void TC4_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TC1_INSTANCE_ */
#ifdef _SAME70_TC1_INSTANCE_
void TC5_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TC1_INSTANCE_ */
void AFEC0_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_DACC_INSTANCE_
void DACC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_DACC_INSTANCE_ */
void PWM0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ICM_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ACC_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USBHS_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MCAN0_INT0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MCAN0_INT1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_MCAN1_INSTANCE_
void MCAN1_INT0_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MCAN1_INT1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_MCAN1_INSTANCE_ */
void GMAC_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AFEC1_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_TWIHS2_INSTANCE_
void TWIHS2_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TWIHS2_INSTANCE_ */
void SPI1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void QSPI_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART2_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART3_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART4_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_TC2_INSTANCE_
void TC6_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TC2_INSTANCE_ */
#ifdef _SAME70_TC2_INSTANCE_
void TC7_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TC2_INSTANCE_ */
#ifdef _SAME70_TC2_INSTANCE_
void TC8_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_TC2_INSTANCE_ */
void TC9_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC10_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TC11_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void AES_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TRNG_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void XDMAC_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ISI_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PWM1_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FPU_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#ifdef _SAME70_SDRAMC_INSTANCE_
void SDRAMC_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* _SAME70_SDRAMC_INSTANCE_ */
void RSWDT_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

        /* Configure Initial Stack Pointer, using linker-generated symbols */
        .pvStack = (void*) (&_estack),

        .pfnReset_Handler      = (void*) Reset_Handler,
        .pfnNMI_Handler        = (void*) NMI_Handler,
        .pfnHardFault_Handler  = (void*) HardFault_Handler,
        .pfnMemManage_Handler  = (void*) MemManage_Handler,
        .pfnBusFault_Handler   = (void*) BusFault_Handler,
        .pfnUsageFault_Handler = (void*) UsageFault_Handler,
#if 1
		.pfnReserved1_Handler  = (void*) &_firmware_crc,
#else
        .pfnReserved1_Handler  = (void*) (0UL),          /* Reserved */
#endif
        .pfnReserved2_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved3_Handler  = (void*) (0UL),          /* Reserved */
        .pfnReserved4_Handler  = (void*) (0UL),          /* Reserved */
        .pfnSVC_Handler        = (void*) SVC_Handler,
        .pfnDebugMon_Handler   = (void*) DebugMon_Handler,
        .pfnReserved5_Handler  = (void*) (0UL),          /* Reserved */
        .pfnPendSV_Handler     = (void*) PendSV_Handler,
        .pfnSysTick_Handler    = (void*) SysTick_Handler,

        /* Configurable interrupts */
        .pfnSUPC_Handler   = (void*) SUPC_Handler,   /* 0  Supply Controller */
        .pfnRSTC_Handler   = (void*) RSTC_Handler,   /* 1  Reset Controller */
        .pfnRTC_Handler    = (void*) RTC_Handler,    /* 2  Real Time Clock */
        .pfnRTT_Handler    = (void*) RTT_Handler,    /* 3  Real Time Timer */
        .pfnWDT_Handler    = (void*) WDT_Handler,    /* 4  Watchdog Timer */
        .pfnPMC_Handler    = (void*) PMC_Handler,    /* 5  Power Management Controller */
        .pfnEFC_Handler    = (void*) EFC_Handler,    /* 6  Enhanced Embedded Flash Controller */
        .pfnUART0_Handler  = (void*) UART0_Handler,  /* 7  UART 0 */
        .pfnUART1_Handler  = (void*) UART1_Handler,  /* 8  UART 1 */
        .pvReserved9       = (void*) (0UL),          /* 9  Reserved */
        .pfnPIOA_Handler   = (void*) PIOA_Handler,   /* 10 Parallel I/O Controller A */
        .pfnPIOB_Handler   = (void*) PIOB_Handler,   /* 11 Parallel I/O Controller B */
#ifdef _SAME70_PIOC_INSTANCE_
        .pfnPIOC_Handler   = (void*) PIOC_Handler,   /* 12 Parallel I/O Controller C */
#else
        .pvReserved12      = (void*) (0UL),          /* 12 Reserved */
#endif /* _SAME70_PIOC_INSTANCE_ */
        .pfnUSART0_Handler = (void*) USART0_Handler, /* 13 USART 0 */
        .pfnUSART1_Handler = (void*) USART1_Handler, /* 14 USART 1 */
        .pfnUSART2_Handler = (void*) USART2_Handler, /* 15 USART 2 */
        .pfnPIOD_Handler   = (void*) PIOD_Handler,   /* 16 Parallel I/O Controller D */
#ifdef _SAME70_PIOE_INSTANCE_
        .pfnPIOE_Handler   = (void*) PIOE_Handler,   /* 17 Parallel I/O Controller E */
#else
        .pvReserved17      = (void*) (0UL),          /* 17 Reserved */
#endif /* _SAME70_PIOE_INSTANCE_ */
#ifdef _SAME70_HSMCI_INSTANCE_
        .pfnHSMCI_Handler  = (void*) HSMCI_Handler,  /* 18 Multimedia Card Interface */
#else
        .pvReserved18      = (void*) (0UL),          /* 18 Reserved */
#endif /* _SAME70_HSMCI_INSTANCE_ */
        .pfnTWIHS0_Handler = (void*) TWIHS0_Handler, /* 19 Two Wire Interface 0 HS */
        .pfnTWIHS1_Handler = (void*) TWIHS1_Handler, /* 20 Two Wire Interface 1 HS */
        .pfnSPI0_Handler   = (void*) SPI0_Handler,   /* 21 Serial Peripheral Interface 0 */
        .pfnSSC_Handler    = (void*) SSC_Handler,    /* 22 Synchronous Serial Controller */
        .pfnTC0_Handler    = (void*) TC0_Handler,    /* 23 Timer/Counter 0 */
        .pfnTC1_Handler    = (void*) TC1_Handler,    /* 24 Timer/Counter 1 */
        .pfnTC2_Handler    = (void*) TC2_Handler,    /* 25 Timer/Counter 2 */
#ifdef _SAME70_TC1_INSTANCE_
        .pfnTC3_Handler    = (void*) TC3_Handler,    /* 26 Timer/Counter 3 */
#else
        .pvReserved26      = (void*) (0UL),          /* 26 Reserved */
#endif /* _SAME70_TC1_INSTANCE_ */
#ifdef _SAME70_TC1_INSTANCE_
        .pfnTC4_Handler    = (void*) TC4_Handler,    /* 27 Timer/Counter 4 */
#else
        .pvReserved27      = (void*) (0UL),          /* 27 Reserved */
#endif /* _SAME70_TC1_INSTANCE_ */
#ifdef _SAME70_TC1_INSTANCE_
        .pfnTC5_Handler    = (void*) TC5_Handler,    /* 28 Timer/Counter 5 */
#else
        .pvReserved28      = (void*) (0UL),          /* 28 Reserved */
#endif /* _SAME70_TC1_INSTANCE_ */
        .pfnAFEC0_Handler  = (void*) AFEC0_Handler,  /* 29 Analog Front End 0 */
#ifdef _SAME70_DACC_INSTANCE_
        .pfnDACC_Handler   = (void*) DACC_Handler,   /* 30 Digital To Analog Converter */
#else
        .pvReserved30      = (void*) (0UL),          /* 30 Reserved */
#endif /* _SAME70_DACC_INSTANCE_ */
        .pfnPWM0_Handler   = (void*) PWM0_Handler,   /* 31 Pulse Width Modulation 0 */
        .pfnICM_Handler    = (void*) ICM_Handler,    /* 32 Integrity Check Monitor */
        .pfnACC_Handler    = (void*) ACC_Handler,    /* 33 Analog Comparator */
        .pfnUSBHS_Handler  = (void*) USBHS_Handler,  /* 34 USB Host / Device Controller */
        .pfnMCAN0_INT0_Handler = (void*) MCAN0_INT0_Handler, /* 35 Controller Area Network */
        .pfnMCAN0_INT1_Handler = (void*) MCAN0_INT1_Handler, /* 36 Controller Area Network */
#ifdef _SAME70_MCAN1_INSTANCE_
        .pfnMCAN1_INT0_Handler = (void*) MCAN1_INT0_Handler, /* 37 Controller Area Network */
        .pfnMCAN1_INT1_Handler = (void*) MCAN1_INT1_Handler, /* 38 Controller Area Network */
#else
        .pvReserved37      = (void*) (0UL),          /* 37 Reserved */
        .pvReserved38      = (void*) (0UL),          /* 38 Reserved */
#endif /* _SAME70_MCAN1_INSTANCE_ */
        .pfnGMAC_Handler   = (void*) GMAC_Handler,   /* 39 Ethernet MAC */
        .pfnAFEC1_Handler  = (void*) AFEC1_Handler,  /* 40 Analog Front End 1 */
#ifdef _SAME70_TWIHS2_INSTANCE_
        .pfnTWIHS2_Handler = (void*) TWIHS2_Handler, /* 41 Two Wire Interface 2 HS */
#else
        .pvReserved41      = (void*) (0UL),          /* 41 Reserved */
#endif /* _SAME70_TWIHS2_INSTANCE_ */
        .pfnSPI1_Handler   = (void*) SPI1_Handler,   /* 42 Serial Peripheral Interface 1 */
        .pfnQSPI_Handler   = (void*) QSPI_Handler,   /* 43 Quad I/O Serial Peripheral Interface */
        .pfnUART2_Handler  = (void*) UART2_Handler,  /* 44 UART 2 */
        .pfnUART3_Handler  = (void*) UART3_Handler,  /* 45 UART 3 */
        .pfnUART4_Handler  = (void*) UART4_Handler,  /* 46 UART 4 */
#ifdef _SAME70_TC2_INSTANCE_
        .pfnTC6_Handler    = (void*) TC6_Handler,    /* 47 Timer/Counter 6 */
#else
        .pvReserved47      = (void*) (0UL),          /* 47 Reserved */
#endif /* _SAME70_TC2_INSTANCE_ */
#ifdef _SAME70_TC2_INSTANCE_
        .pfnTC7_Handler    = (void*) TC7_Handler,    /* 48 Timer/Counter 7 */
#else
        .pvReserved48      = (void*) (0UL),          /* 48 Reserved */
#endif /* _SAME70_TC2_INSTANCE_ */
#ifdef _SAME70_TC2_INSTANCE_
        .pfnTC8_Handler    = (void*) TC8_Handler,    /* 49 Timer/Counter 8 */
#else
        .pvReserved49      = (void*) (0UL),          /* 49 Reserved */
#endif /* _SAME70_TC2_INSTANCE_ */
        .pfnTC9_Handler    = (void*) TC9_Handler,    /* 50 Timer/Counter 9 */
        .pfnTC10_Handler   = (void*) TC10_Handler,   /* 51 Timer/Counter 10 */
        .pfnTC11_Handler   = (void*) TC11_Handler,   /* 52 Timer/Counter 11 */
        .pvReserved53      = (void*) (0UL),          /* 53 Reserved */
        .pvReserved54      = (void*) (0UL),          /* 54 Reserved */
        .pvReserved55      = (void*) (0UL),          /* 55 Reserved */
        .pfnAES_Handler    = (void*) AES_Handler,    /* 56 AES */
        .pfnTRNG_Handler   = (void*) TRNG_Handler,   /* 57 True Random Generator */
        .pfnXDMAC_Handler  = (void*) XDMAC_Handler,  /* 58 DMA */
        .pfnISI_Handler    = (void*) ISI_Handler,    /* 59 Camera Interface */
        .pfnPWM1_Handler   = (void*) PWM1_Handler,   /* 60 Pulse Width Modulation 1 */
        .pfnFPU_Handler    = (void*) FPU_Handler,    /* 61 Floating Point Unit Registers */
#ifdef _SAME70_SDRAMC_INSTANCE_
        .pfnSDRAMC_Handler = (void*) SDRAMC_Handler, /* 62 SDRAM Controller */
#else
        .pvReserved62      = (void*) (0UL),          /* 62 Reserved */
#endif /* _SAME70_SDRAMC_INSTANCE_ */
        .pfnRSWDT_Handler  = (void*) RSWDT_Handler   /* 63 Reinforced Secure Watchdog Timer */
};

// This must be marked noinline so that R0 is loaded with the required value for SP. Return is via LR so it's OK to return with a different SP.
__attribute((noinline)) void SetStackPointer(uint32_t *topOfStack)
{
	__asm volatile("msr msp, r0");
}

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
__attribute__((noreturn, naked)) void Reset_Handler(void)
{
	// If TCM is allocated then SP may point beyond the end of RAM, so move it
	SetStackPointer(&_ezero_nocache);

	// Initialize the relocate segment
	{
		const uint32_t *pSrc = &_etext;
		uint32_t *pDest = &_srelocate;

		if (pSrc != pDest)
		{
			while (pDest < &_erelocate)
			{
				*pDest++ = *pSrc++;
			}
		}
	}

	/* Clear the zero segment */
	for (uint32_t *pDest = &_szero; pDest < &_ezero;)
	{
		*pDest++ = 0;
	}

	// Check that no TCM is allocated before we relocate the stack to the top of memory. This uses a RAMFUNC, so we must initialise the relocate segment before here.
	// The temporary stack we are on is in the non-cached memory segment, so don't clear that segment until after we have relocated the stack.
	if (flash_read_gpnvm_bits() & ((1ul << 7) | (1ul << 8)))
	{
		// TCM is allocated - we are probably downgrading from later firmware that uses TCM. Disable TCM and reboot.
		flash_clear_gpnvm(7);
		flash_clear_gpnvm(8);
		// On some microchip processors, we need a delay between writing to flash and resetting
		for (unsigned int i = 0; i < 10000; ++i)
		{
			__asm volatile("nop");
		}
		Reset();
	}

	// Now it's safe to reset the stack pointer to the top of memory
	SetStackPointer(&_estack);

	// Clear the nocache RAM segment
	for (uint32_t *pDest = &_szero_nocache; pDest < &_ezero_nocache;)
	{
		*pDest++ = 0;
	}

	// Set the vector table base address
	const uint32_t * const pSrc = (uint32_t *) & _sfixed;
	SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

#if __FPU_USED
	fpu_enable();
#else
# warning Compiling without FPU support
#endif

	SystemInit();			// set up the clock
	UrgentInit();			// initialise anything in the main application that can't wait
	__libc_init_array();	// initialize C library and call C++ constructors for static data
	init();					// initialise variant

	AppMain();				// note: app must set up the system tick interrupt, either within FreeRTOS or by calling SysTickInit
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
	while (1) { }
}

// End
