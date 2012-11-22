/*!
 * \file StringUtil.cpp
 * \date 4-18-2012 0:39:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <tchar.h>
#include <util/StringUtil.h>
#include <algorithm>
#include <Windows.h>

bool StringUtil::tchar2char(std::string& strOut, const tchar* pstrIn)
{
#ifdef _UNICODE
	static std::vector<char> s_MultiByteBuf;

	int nBufSize = WideCharToMultiByte(CP_UTF8, NULL, pstrIn, -1, NULL, 0, NULL, NULL);
	if ((int)s_MultiByteBuf.size() < nBufSize) s_MultiByteBuf.resize(nBufSize);

	WideCharToMultiByte(CP_UTF8, NULL, pstrIn, -1, &s_MultiByteBuf[0], nBufSize, NULL, NULL);
	strOut = ((char*)(&s_MultiByteBuf[0]));
	return true;
#else
	strOut = pstrIn;
	return true;
#endif // _UNICODE
}

const char* StringUtil::tchar2char(const tchar* pszIn)
{
	static std::string s_strBuffer;
	tchar2char(s_strBuffer, pszIn);
	return s_strBuffer.c_str();
}

bool StringUtil::char2tchar(tstring& strOut, const char* pstrIn)
{
#ifdef _UNICODE
	static std::vector<tchar> s_WideCharBuf;

	int nBufSize = MultiByteToWideChar(CP_UTF8, NULL, pstrIn, -1, NULL, 0);
	if ((int)s_WideCharBuf.size() < nBufSize) s_WideCharBuf.resize(nBufSize);

	MultiByteToWideChar(CP_UTF8, NULL, pstrIn, -1, &s_WideCharBuf[0], nBufSize);
	strOut = ((tchar*)&s_WideCharBuf[0]);
	return true;
#else
	strOut = pstrIn;
	return true;
#endif // _UNICODE
}

const tchar* StringUtil::char2tchar(const char* pszIn)
{
	static tstring s_strBuffer;
	char2tchar(s_strBuffer, pszIn);
	return s_strBuffer.c_str();
}

bool StringUtil::str2bool(bool& bValue, const tchar* pstrIn)
{
	tstring strBool;
	tolower(strBool, pstrIn);
	if (strBool == _("true")) bValue = true;
	else if (strBool == _("false")) bValue = false;
	else return false;

	return true;
}

bool StringUtil::str2int(int& nValue, const tchar* pstrIn)
{
	nValue = _tstoi(pstrIn);
	return true;
}

bool StringUtil::str2float(float& fValue, const tchar* pstrIn)
{
	fValue = (float)_tstof(pstrIn);
	return true;
}

bool StringUtil::strHex2Uint(uint& nValue, const tchar* pstrIn)
{
	_stscanf_s(pstrIn, _("%x"), &nValue);
	return true;
}

void StringUtil::bool2str(tstring& strOut, bool bValue)
{
	if (bValue) strOut = _("true");
	else strOut = _("false");
}

void StringUtil::int2str(tstring& strOut, int nValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf_s(s_strBuffer, 1024, _("%d"), nValue);
	strOut = s_strBuffer;
}

void StringUtil::hex2str(tstring& strOut, uint nValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf_s(s_strBuffer, 1024, _("0x%08X"), nValue);
	strOut = s_strBuffer;
}

void StringUtil::float2str(tstring& strOut, float fValue)
{
	static tchar s_strBuffer[1024];
	_sntprintf_s(s_strBuffer, 1024, _("%f"), fValue);
	strOut = s_strBuffer;
}

bool StringUtil::strformat(tstring& strOut, const tchar* strFormat, ...)
{
	static const int MAX_BUFF_COUNT = 1024*10;
	static tchar s_strBuff[MAX_BUFF_COUNT];

	va_list marker;
	va_start(marker, strFormat);

	int nLength = _vsntprintf_s(s_strBuff, MAX_BUFF_COUNT, strFormat, marker);
	strOut = s_strBuff;
	va_end(marker);

	return true;
}

void StringUtil::toupper(tstring& strOut, const tstring& strIn)
{
	strOut = strIn;
	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);
}

void StringUtil::tolower(tstring& strOut, const tstring& strIn)
{
	strOut = strIn;
	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);
}

int StringUtil::splitString(std::vector<tstring>& arrOut, const tstring& strIn, const tstring& strSplit)
{
	int totalSplit = 0;
	size_t startIndex = 0;
	size_t endIndex = 0;

	endIndex = strIn.find(strSplit.c_str(), startIndex);
	while (endIndex != tstring::npos)
	{
		arrOut.push_back(strIn.substr(startIndex, endIndex-startIndex));
		totalSplit++;

		startIndex += strSplit.length();
		endIndex = strIn.find(strSplit.c_str(), startIndex);
	}

	if (startIndex >= strIn.length()-1) return totalSplit;

	arrOut.push_back(strIn.substr(startIndex));
	totalSplit++;

	return totalSplit;
}

tstring StringUtil::joinString(const std::vector<tstring>& arrIn, const tstring& strSplit)
{
	tstring strOut;

	int nLength = int(arrIn.size());
	if (nLength <= 0) return strOut;

	strOut += arrIn[0];

	for (int i = 1; i < nLength; ++i)
	{
		strOut += strSplit;
		strOut += arrIn[i];
	}

	return strOut;
}

void StringUtil::GetFileNameWithoutExt(tstring& strOut, const tstring& strIn)
{
	size_t nPosBegin = strIn.rfind(_('\\'));
	if (nPosBegin == tstring::npos)
	{
		nPosBegin = strIn.rfind(_('/'));
		if (nPosBegin == tstring::npos) nPosBegin = -1;
	}
	++nPosBegin;

	size_t nPosEnd = strIn.rfind(_('.'));
	if (nPosEnd == tstring::npos) nPosEnd = strIn.length();

	strOut = strIn.substr(nPosBegin, nPosEnd-nPosBegin);
}

void StringUtil::GetFileNameWithExt(tstring& strOut, const tstring& strIn)
{
	size_t nPosBegin = strIn.rfind(_('\\'));
	if (nPosBegin == tstring::npos)
	{
		nPosBegin = strIn.rfind(_('/'));
		if (nPosBegin == tstring::npos) nPosBegin = -1;
	}
	++nPosBegin;

	size_t nPosEnd = strIn.length();
	strOut = strIn.substr(nPosBegin, nPosEnd);
}

void StringUtil::GetFileExt(tstring& strOut, const tstring& strIn)
{
	size_t nPos = strIn.rfind(_('.'));
	if (nPos != tstring::npos)
	{
		strOut = strIn.substr(nPos+1, tstring::npos);
	}
	else
	{
		strOut.clear();
	}
}

void StringUtil::GetFileDir(tstring& strOut, const tstring& strIn)
{
	size_t nPos = strIn.rfind(_('\\'));
	if (nPos != tstring::npos)
	{
		strOut = strIn.substr(0, nPos);
	}
	else
	{
		strOut.clear();
	}
}

bool StringUtil::GetFileFullPath(tstring& strFilePathOut, const tstring& strDir, const tstring& strFile)
{
	strFilePathOut = strDir + _("\\") + strFile;
	return true;
}
