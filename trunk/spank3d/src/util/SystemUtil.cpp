/*!
 * \file SystemUtil.cpp
 * \date 4-18-2012 0:33:02
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <util/SystemUtil.h>
#include <time.h>

uint64 SystemUtil::TimeNow()
{
	return time(NULL);
}

void SystemUtil::TimeLocal(LOCAL_TIME& LocalTime, uint64 nTimeStamp)
{
	tm tmLocal;
	localtime_s(&tmLocal, (time_t*)&nTimeStamp);

	LocalTime.nYear = tmLocal.tm_year + 1900;
	LocalTime.nMonth = tmLocal.tm_mon + 1;
	LocalTime.nDayInYear = tmLocal.tm_yday;
	LocalTime.nDayInMonth = tmLocal.tm_mday;
	LocalTime.nDayInWeek = tmLocal.tm_wday;
	LocalTime.nHour = tmLocal.tm_hour;
	LocalTime.nMinute = tmLocal.tm_min;
	LocalTime.nSecond = tmLocal.tm_sec;
}

void SystemUtil::ZeroMemory(void* pBuffer, uint nSize)
{
	memset(pBuffer, 0, nSize);
}

void SystemUtil::CopyMemory(void* pBuffer, const void* pSrcBuffer, uint nSize)
{
	memcpy(pBuffer, pSrcBuffer, nSize);
}