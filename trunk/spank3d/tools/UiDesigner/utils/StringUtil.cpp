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

int StringUtil::SplitString(std::vector<wxString>& arrOut, const wxString& strIn, const wxString& strSplit)
{
	int totalSplit = 0;
	size_t startIndex = 0;
	size_t endIndex = 0;

	endIndex = strIn.find(strSplit.c_str(), startIndex);
	while (endIndex != wxString::npos)
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

wxString StringUtil::JoinString(const std::vector<wxString>& arrIn, const wxString& strSplit)
{
	wxString strOut;

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

wxString StringUtil::Hex2Str(unsigned int nValue)
{
	wxString strOut;
	strOut.Printf(wxT("0x%08X"), nValue);
	return strOut;
}

unsigned int StringUtil::Str2Hex(const wxString& strIn)
{
	unsigned int value = 0;
	_stscanf_s(strIn.c_str(), wxT("%x"), &value);
	return value;
}
