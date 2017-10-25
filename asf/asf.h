/*
 * asf.h
 *
 *  Created on: 13 Mar 2016
 *      Author: David
 */

#ifndef ASF_H_
#define ASF_H_

#include "sam/utils/compiler.h"

#if SAM4E
#include "sam/drivers/afec/afec.h"
#else
#include "sam/drivers/adc/adc.h"
#endif

#include "sam/drivers/dacc/dacc.h"

#if SAM4E || SAM3XA
#include "sam/drivers/dmac/dmac.h"
#endif

#include "sam/drivers/pio/pio.h"
#include "sam/drivers/pmc/pmc.h"
#include "sam/drivers/pwm/pwm.h"
#include "sam/drivers/spi/spi.h"
#include "sam/drivers/wdt/wdt.h"

#if SAM4E
#include "sam/drivers/rswdt/rswdt.h"
#endif

#if SAM4E || SAM4S
#include "sam/services/flash_efc/flash_efc.h"
#endif

#endif /* ASF_H_ */
