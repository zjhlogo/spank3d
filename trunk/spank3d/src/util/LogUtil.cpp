/*!
 * \file LogUtil.cpp
 * \date 4-18-2012 0:11:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <util/LogUtil.h>
#include <util/StringUtil.h>
#include <util/SystemUtil.h>
#include <util/FileUtil.h>

#if (ENABLE_LOG)
tstring g_strBuffer;
#endif

LogUtil::LogUtil()
{
	m_strFile = _("log.txt");
	m_pLogFile = NULL;
}

LogUtil::~LogUtil()
{
	SAFE_RELEASE(m_pLogFile);
}

LogUtil& LogUtil::GetInstance()
{
	static LogUtil s_LogUtil;
	return s_LogUtil;
}

void LogUtil::SetOutputFile(const tstring& strFile)
{
	SAFE_RELEASE(m_pLogFile);
	m_strFile = strFile;
}

void LogUtil::Log(const tstring& strMsg)
{
	if (!m_pLogFile && !CreateLogFile(m_strFile)) return;

	SystemUtil::LOCAL_TIME LocalTime;
	SystemUtil::TimeLocal(LocalTime, SystemUtil::TimeNow());

	tstring strText;
	StringUtil::strformat(strText, _("[%04d-%02d-%02d %02d:%02d:%02d]: %s\n"),
		LocalTime.nYear,
		LocalTime.nMonth,
		LocalTime.nDayInMonth,
		LocalTime.nHour,
		LocalTime.nMinute,
		LocalTime.nSecond,
		strMsg.c_str());

	std::string strAnsiText;
	StringUtil::tchar2char(strAnsiText, strText.c_str());

	m_pLogFile->Write(strAnsiText.c_str(), (uint)strAnsiText.length());
}

bool LogUtil::CreateLogFile(const tstring& strFile)
{
	static uchar s_BOM[] = {0xEF, 0xBB, 0xBF};

	SAFE_RELEASE(m_pLogFile);

	m_pLogFile = FileUtil::GetInstance().OpenFile(strFile, IFile::OFF_WRITE);
	if (!m_pLogFile) return false;

	m_pLogFile->Write(s_BOM, 3);

	return true;
}
