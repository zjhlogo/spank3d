/*!
 * \file FileUtil.h
 * \date 2-13-2012 13:21:24
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

#include <wx/string.h>

class FileUtil
{
public:
	static void FormatDir(wxString& strDirInOut);
	static wxString GetFileName(const wxString& strPath);
	static wxString RemoveRootDir(const wxString& strPath, const wxString& strRootDir);
	static void FormatId(wxString& strId);
};
#endif // __FILEUTIL_H__
