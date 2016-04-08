/*
 * variant.h
 *
 *  Created on: 26 Mar 2016
 *      Author: David
 */

#if defined(__SAM3X8E__)
# include "duet/variant.h"
#elif defined(__SAM4E8E__)
# include "duetNG/variant.h"
#else
# error Unknown electronics variant
#endif
