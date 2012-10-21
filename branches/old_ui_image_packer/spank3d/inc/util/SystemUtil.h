/*!
 * \file SystemUtil.h
 * \date 4-18-2012 0:32:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __SYSTEMUTIL_H__
#define __SYSTEMUTIL_H__

#include "../type/BaseType.h"

class SystemUtil
{
public:
	typedef struct LOCAL_TIME_tag
	{
		int nYear;			// years [1900, 9999]
		int nMonth;			// month [1, 12]
		int nDayInYear;		// day in year [0, 365]
		int nDayInMonth;	// day in month [1, 31]
		int nDayInWeek;		// day in week [0, 6]
		int nHour;			// hour [0, 23]
		int nMinute;		// minute [0, 59]
		int nSecond;		// second [0, 59]
	} LOCAL_TIME;

public:
	static uint64 TimeNow();
	static void TimeLocal(LOCAL_TIME& LocalTime, uint64 nTimeStamp);

	static void ZeroMemory(void* pBuffer, uint nSize);
	static void CopyMemory(void* pBuffer, const void* pSrcBuffer, uint nSize);

};
#endif // __SYSTEMUTIL_H__
