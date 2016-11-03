/* Small library to control the built-in RTC on the SAM3X and SAM4E
 *
 * written by Christian Hammacher, 2016
 */
#ifndef RTC_H
#define RTC_H

#include <ctime>

class RTCDue
{
public:
	static void Init();

	static bool IsDateTimeSet();
	static time_t GetDateTime();

	static bool SetDateTime(time_t datetime);
	static bool SetDate(time_t date);
	static bool SetTime(time_t time);

#ifdef __SAM4E__
	static void SetCorrection(uint32_t direction_ppm, uint32_t correction, uint32_t range_ppm);
#endif
};

#endif
