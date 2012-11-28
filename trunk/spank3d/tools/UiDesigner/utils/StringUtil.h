/*!
 * \file StringUtil.h
 * \date 11-22-2012 13:33:24
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__

#include <wx/string.h>
#include <vector>

class StringUtil
{
public:
	static void FormatDir(wxString& strDirInOut);
	static wxString GetFileName(const wxString& strPath);
	static wxString GetFileDir(const wxString& strPath);
	static wxString RemoveRootDir(const wxString& strPath, const wxString& strRootDir);

	static int SplitString(std::vector<wxString>& arrOut, const wxString& strIn, const wxString& strSplit);
	static wxString JoinString(const std::vector<wxString>& arrIn, const wxString& strSplit);

	static wxString Hex2Str(unsigned int nValue);
	static unsigned int Str2Hex(const wxString& strIn);

};
#endif // __STRINGUTIL_H__
