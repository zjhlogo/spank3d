/*!
 * \file IDoc.cpp
 * \date 11-22-2012 9:12:38
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "IDoc.h"
#include "../utils/StringUtil.h"

IDoc::IDoc()
{
	// TODO: 
}

IDoc::~IDoc()
{
	// TODO: 
}

bool IDoc::Open(const wxString& strDir, const wxString& strFile)
{
	m_strBaseDir = strDir;
	StringUtil::FormatDir(m_strBaseDir);
	m_strFileName = strFile;
	m_strPath = m_strBaseDir + wxT("/") + m_strFileName;

	Reset();
	return DoOpen(m_strPath);
}

bool IDoc::Save(const wxString& strFile /* = wxEmptyString */)
{
	wxString path = strFile;
	if (path.empty()) path = m_strPath;
	if (path.empty()) return false;

	return DoSave(path);
}

void IDoc::Close()
{
	Reset();
}

const wxString& IDoc::GetFileName() const
{
	return m_strFileName;
}
