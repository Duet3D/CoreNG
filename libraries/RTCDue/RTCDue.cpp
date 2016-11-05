/* Small library to control the built-in RTC on the SAM3X and SAM4E
 *
 * written by Christian Hammacher, 2016
 */

#include "RTCDue.h"
#include "sam/drivers/rtc/rtc.h"
#include <cstring>

void RTCDue::Init()
{
	rtc_set_hour_mode(RTC, 0);		// set RTC to 24-hour mode
	RTC->RTC_CALR = 0xFFFFFFFF;		// invalidate the current date

	// disable RTC alarm interrupts
	NVIC_DisableIRQ(RTC_IRQn);
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_SetPriority(RTC_IRQn, 8);
}

bool RTCDue::IsDateTimeSet()
{
	return rtc_get_valid_entry(RTC) == 0;
}

time_t RTCDue::GetDateTime()
{
	uint32_t hour, minute, second;
	rtc_get_time(RTC, &hour, &minute, &second);

	uint32_t year, month, day, week;
	rtc_get_date(RTC, &year, &month, &day, &week);

	struct tm timeInfo;
	memset(&timeInfo, 0, sizeof(tm));
	timeInfo.tm_sec = second;
	timeInfo.tm_min = minute;
	timeInfo.tm_hour = hour;
	timeInfo.tm_mday = day;
	timeInfo.tm_mon = month - 1;
	timeInfo.tm_year = year - 1900;
	timeInfo.tm_isdst = 0;
	return mktime(&timeInfo);
}

bool RTCDue::SetDateTime(time_t datetime)
{
	return SetDate(datetime) && SetTime(datetime);
}

// This function has been copied from the ASF RTC example
static uint32_t calculate_week(uint32_t ul_year, uint32_t ul_month, uint32_t ul_day)
{
	if (ul_month == 1 || ul_month == 2)
	{
		ul_month += 12;
		--ul_year;
	}

	const uint32_t ul_week = (ul_day + 2 * ul_month + 3 * (ul_month + 1) / 5 + ul_year + ul_year / 4 - ul_year / 100 + ul_year / 400) % 7;

	return ul_week + 1;
}

bool RTCDue::SetDate(time_t date)
{
	const struct tm * const timeInfo = gmtime(&date);
	const uint32_t week = calculate_week(timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday);
	return rtc_set_date(RTC, timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, week) == 0;
}

bool RTCDue::SetTime(time_t time)
{
	const struct tm * const timeInfo = gmtime(&time);
	return rtc_set_time(RTC, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec) == 0;
}

#ifdef __SAM4E__
void RTCDue::SetCorrection(uint32_t direction_ppm, uint32_t correction, uint32_t range_ppm)
{
	// This feature isn't available on the SAM3X
	rtc_set_calibration(RTC, direction_ppm, correction, range_ppm);
}
#endif

