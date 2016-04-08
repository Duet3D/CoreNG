/*
 * spi_master.h
 *
 *  Created on: 28 Mar 2016
 *      Author: David
 */

#if defined(__SAM3X8E__)
# include "duet/spi_master.h"
#elif defined(__SAM4E8E__)
# include "duetNG/spi_master.h"
#else
# error Unknown electronics variant
#endif
