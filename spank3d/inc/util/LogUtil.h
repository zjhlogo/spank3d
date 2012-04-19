/*!
 * \file LogUtil.h
 * \date 4-18-2012 0:10:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __LOGUTIL_H__
#define __LOGUTIL_H__

#include "../Config.h"
#include "StringUtil.h"
#include "IFile.h"

class LogUtil
{
public:
	LogUtil();
	virtual ~LogUtil();

	static LogUtil& GetInstance();

	void SetOutputFile(const tstring& strFile);
	void Log(const tstring& strMsg);

private:
	bool CreateLogFile(const tstring& strFile);

private:
	tstring m_strFile;
	IFile* m_pLogFile;

};

#if (ENABLE_LOG)
	extern tstring g_strBuffer;
	#define LOG(pstrFormat, ...)																\
		StringUtil::strformat(g_strBuffer, pstrFormat, __VA_ARGS__);							\
		LogUtil::GetInstance().Log(g_strBuffer)
#else
	#define LOG(pstrFormat, ...) 0
#endif

#endif // __LOGUTIL_H__
