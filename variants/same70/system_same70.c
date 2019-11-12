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

#include "same70.h"
#include "variant.h"	// for VARIANT_MCK

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

// Definitions for the clock system
// Max processor clock is 300MHz
// Max PLLA output frequency is 500MHz
#if VARIANT_MCK == 150000000
const uint32_t PllaMul = 25;			// the multiplier we need from 12MHz to get the required PLLA clock
const uint32_t FlashWaitStates = 6;		// the FWS value needed, one less than the actual number of flash wait states. See table 59-51 of the datasheet.
#elif VARIANT_MCK == 144000000
const uint32_t PllaMul = 24;
const uint32_t FlashWaitStates = 6;
#elif VARIANT_MCK == 120000000
// For this one we could use the 480MHz USB PLL output instead (PMC_MCKR_CSS_UPLL_CLK) and divide it by 2 (PMC_MCKR_PRES_CLK_2)
const uint32_t PllaMul = 20;
const uint32_t FlashWaitStates = 5;
#endif

#define SYS_BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8U))									// crystal oscillator startup time is 8 * 8 slow clocks
#define SYS_BOARD_PLLAR     (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(PllaMul - 1) | \
                            CKGR_PLLAR_PLLACOUNT(0x3fU) | CKGR_PLLAR_DIVA_BYPASS)
#define SYS_BOARD_MCKR      (PMC_MCKR_PRES_CLK_1 | PMC_MCKR_CSS_PLLA_CLK | PMC_MCKR_MDIV_PCK_DIV2)

uint32_t SystemCoreClock = CHIP_FREQ_XTAL_12M;

/**
 * \brief Setup the microcontroller system.
 * Initialize the System and update the SystemFrequency variable.
 */
 void SystemInit( void )
{
  /* Set FWS according to SYS_BOARD_MCKR configuration */
  EFC->EEFC_FMR = EEFC_FMR_FWS(FlashWaitStates);

  /* Initialize main oscillator */
  if ( !(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) )			// if the crystal oscillator is not selected
  {
    PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | SYS_BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;		// enable the crystal oscillator, keep the RC oscillator enabled

    while ( !(PMC->PMC_SR & PMC_SR_MOSCXTS) )			// wait until crystal oscillator has stablised
    {
    }
  }

  /* Switch to 3-20MHz Xtal oscillator */
  PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | SYS_BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;	// switch to crystal oscillator

  while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) )			// wait until selection is complete
  {
  }

  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;					// switch master clock to main clock

  while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
  {
  }

  /* Initialize PLLA */
  PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
  while ( !(PMC->PMC_SR & PMC_SR_LOCKA) )
  {
  }

  /* Switch to main clock */
  PMC->PMC_MCKR = (SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
  while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
  {
  }

  /* Switch to PLLA */
  PMC->PMC_MCKR = SYS_BOARD_MCKR;
  while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) )
  {
  }

  SystemCoreClock = (CHIP_FREQ_XTAL_12M * PllaMul)/2;		// see definitions of SYS_BOARD_PLLAR near the start of this file
}

// SysTick init, called by non-RTOS builds
void SysTickInit(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
