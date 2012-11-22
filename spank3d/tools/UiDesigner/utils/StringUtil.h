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

class StringUtil
{
public:
	static void FormatDir(wxString& strDirInOut);
	static wxString GetFileName(const wxString& strPath);
	static wxString GetFileDir(const wxString& strPath);
	static wxString RemoveRootDir(const wxString& strPath, const wxString& strRootDir);
};
#endif // __STRINGUTIL_H__
