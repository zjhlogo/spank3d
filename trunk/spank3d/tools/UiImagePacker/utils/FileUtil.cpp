/*!
 * \file FileUtil.cpp
 * \date 2-13-2012 13:21:39
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "FileUtil.h"
#include <Windows.h>

void FileUtil::FormatDir(wxString& strDirInOut)
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

wxString FileUtil::GetFileName(const wxString& strPath)
{
	int nStartPos = 0;
	for (int i = (int)strPath.length()-1; i >= 0; --i)
	{
		if (strPath[i] == wxT('\\') || strPath[i] == wxT('/'))
		{
			break;
		}

		nStartPos = i;
	}

	int nEndPos = nStartPos;
	for (; nEndPos < (int)strPath.length(); ++nEndPos)
	{
		if (strPath[nEndPos] == wxT('.')) break;
	}

	return strPath.SubString(nStartPos, nEndPos-1);
}

wxString FileUtil::RemoveRootDir(const wxString& strPath, const wxString& strRootDir)
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

void FileUtil::FormatId(wxString& strId)
{
	strId = strId.Lower();
	for (int i = 0; i < (int)strId.length(); ++i)
	{
		if ((strId[i] >= wxT('a') && strId[i] <= wxT('z'))
			|| (strId[i] >= wxT('0') && strId[i] <= wxT('9')))
		{
			// TODO: 
		}
		else
		{
			strId[i] = wxT('_');
		}
	}
}

void FileUtil::GetFileDir(wxString& strOut, const wxString& strIn)
{
	size_t nPosEnd = strIn.rfind(wxT('\\'));
	if (nPosEnd == wxString::npos)
	{
		nPosEnd = strIn.rfind(wxT('/'));
		if (nPosEnd == wxString::npos)
		{
			nPosEnd = -1;
			strOut.clear();
			return;
		}
	}

	strOut = strIn.substr(0, nPosEnd);
}

void FileUtil::GetCurrDir(wxString& strOut)
{
	wxChar currDir[1024];
	GetCurrentDirectory(1024, currDir);

	strOut = currDir;
	FormatDir(strOut);
}
