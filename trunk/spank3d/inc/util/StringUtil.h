/*!
 * \file StringUtil.h
 * \date 4-18-2012 0:39:24
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__

#include "../type/BaseType.h"

class StringUtil
{
public:
	static bool str2bool(bool& bValue, const tchar* pstrIn);
	static bool str2int(int& nValue, const tchar* pstrIn);
	static bool str2float(float& fValue, const tchar* pstrIn);

	static bool tchar2char(std::string& strOut, const tchar* pstrIn);
	static const char* tchar2char(const tchar* pszIn);

	static bool char2tchar(tstring& strOut, const char* pstrIn);
	static const tchar* char2tchar(const char* pszIn);

	static bool bool2str(tstring& strOut, bool bValue);
	static void int2str(tstring& strOut, int nValue);
	static void float2str(tstring& strOut, float fValue);
	static bool strformat(tstring& strOut, const tchar* strFormat, ...);
	static void toupper(tstring& strOut, const tstring& strIn);
	static void tolower(tstring& strOut, const tstring& strIn);

	static void GetFileNameWithoutExt(tstring& strOut, const tstring& strIn);
	static void GetFileNameWithExt(tstring& strOut, const tstring& strIn);
	static void GetFileExt(tstring& strOut, const tstring& strIn);
	static void GetFileDir(tstring& strOut, const tstring& strIn);
	static bool GetFileFullPath(tstring& strFilePathOut, const tstring& strDir, const tstring& strFile);

};
#endif // __STRINGUTIL_H__
