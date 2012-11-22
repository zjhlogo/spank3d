/*!
 * \file StringUtil.cpp
 * \date 11-22-2012 13:33:39
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "StringUtil.h"

void StringUtil::FormatDir(wxString& strDirInOut)
{
	while (strDirInOut.rbegin() != strDirInOut.rend())
	{
		char ch = (*strDirInOut.rbegin());
		if (ch == wxT('\\') || ch == wxT('/'))
		{
			strDirInOut.erase(strDirInOut.length()-1, 1);
		}
		else
		{
			break;
		}
	}

	strDirInOut.Replace(wxT("\\"), wxT("/"));
}

wxString StringUtil::GetFileName(const wxString& strPath)
{
	int nStartPos = 0;
	for (int i = (int)strPath.length()-1; i >= 0; --i)
	{
		if (strPath[i] == wxT('\\') || strPath[i] == wxT('/'))
		{
			nStartPos = i;
			break;
		}
	}

	int nEndPos = nStartPos;
	for (; nEndPos < (int)strPath.length(); ++nEndPos)
	{
		if (strPath[nEndPos] == wxT('.')) break;
	}

	return strPath.SubString(nStartPos+1, nEndPos-1);
}

wxString StringUtil::GetFileDir(const wxString& strPath)
{
	int nEndPos = 0;
	for (int i = (int)strPath.length()-1; i >= 0; --i)
	{
		if (strPath[i] == wxT('\\') || strPath[i] == wxT('/'))
		{
			nEndPos = i;
			break;
		}
	}

	return strPath.SubString(0, nEndPos);
}

wxString StringUtil::RemoveRootDir(const wxString& strPath, const wxString& strRootDir)
{
	int nMinLen = strRootDir.length();
	if (nMinLen > (int)strPath.length()) nMinLen = strPath.length();

	wxString strPathFormated = strPath.Lower();
	strPathFormated.Replace(wxT("\\"), wxT("/"));

	wxString strRootDirFormated = strRootDir.Lower();
	strRootDirFormated.Replace(wxT("\\"), wxT("/"));

	int nEndPos = 0;
	for (; nEndPos < nMinLen; ++nEndPos)
	{
		if (strPathFormated[nEndPos] != strRootDirFormated[nEndPos]) break;
	}

	return strPath.SubString(nEndPos, strPath.length());
}
