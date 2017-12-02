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

#ifndef _WIRING_MATH_
#define _WIRING_MATH_

#include <stdint.h>
#include <compiler.h>

#if SAM3XA || SAME70

#ifdef __cplusplus
extern "C" uint32_t trueRandom();
#else
extern uint32_t trueRandom();
#endif

#endif	// SAM3XA || SAME70

#ifdef __cplusplus

#include <cmath>

extern int32_t random(int32_t);
extern int32_t random(int32_t, int32_t);
extern int32_t map(int32_t, int32_t, int32_t, int32_t, int32_t);

// std::min and std::max don't seem to work with this variant of gcc, so define our own ones here
// We use these only with primitive types, so pass them directly instead of by const reference
#ifdef min
# undef min
#endif

#ifdef max
# undef max
#endif

template<class X> inline X min(X _a, X _b)
{
	return (_a < _b) ? _a : _b;
}

template<class X> inline X max(X _a, X _b)
{
	return (_a > _b) ? _a : _b;
}

// Specialisations for float and double to handle NaNs properly
template<> inline float min(float _a, float _b)
{
	return (std::isnan(_a) || _a < _b) ? _a : _b;
}

template<> inline float max(float _a, float _b)
{
	return (std::isnan(_a) || _a > _b) ? _a : _b;
}

template<> inline double min(double _a, double _b)
{
	return (std::isnan(_a) || _a < _b) ? _a : _b;
}

template<> inline double max(double _a, double _b)
{
	return (std::isnan(_a) || _a > _b) ? _a : _b;
}

inline float fsquare(float arg)
{
	return arg * arg;
}

inline double dsquare(double arg)
{
	return arg * arg;
}

inline uint64_t isquare64(int32_t arg)
{
	return (uint64_t)((int64_t)arg * arg);
}

inline uint64_t isquare64(uint32_t arg)
{
	return (uint64_t)arg * arg;
}

inline void swap(float& a, float& b)
{
	float temp = a;
	a = b;
	b = temp;
}

// Note that constrain<float> will return NaN for a NaN input because of the way we define min<float> and max<float>
template<class T> inline T constrain(T val, T vmin, T vmax)
{
	return max<T>(min<T>(val, vmax), vmin);
}

#endif

// Macro to give us the number of elements in an array
#define ARRAY_SIZE(_x)	(sizeof(_x)/sizeof(_x[0]))

// Macro to give us the highest valid index into an array i.e. one less than the size
#define ARRAY_UPB(_x)	(ARRAY_SIZE(_x) - 1)

#endif /* _WIRING_MATH_ */
