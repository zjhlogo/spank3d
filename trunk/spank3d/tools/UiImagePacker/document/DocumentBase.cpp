/*!
 * \file DocumentBase.cpp
 * \date 2-14-2012 1:22:58
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "DocumentBase.h"
#include "../ImagePackerFrame.h"

DocumentBase::DocumentBase()
{
	m_bModifiedFlag = false;
}

DocumentBase::~DocumentBase()
{
}

void DocumentBase::SetFilePath(const wxString& strFilePath)
{
	m_strFilePath = strFilePath;
}

const wxString& DocumentBase::GetFilePath() const
{
	return m_strFilePath;
}

void DocumentBase::SetModifiedFlag()
{
	m_bModifiedFlag = true;
	ImagePackerFrame::GetInstance().UpdateUiModified(m_bModifiedFlag);
}

void DocumentBase::ClearModifiedFlag()
{
	m_bModifiedFlag = false;
}

bool DocumentBase::IsModified() const
{
	return m_bModifiedFlag;
}

wxXmlNode* DocumentBase::FindXmlChild(wxXmlNode* pParent, const wxString& name)
{
	if (!pParent) return NULL;

	wxXmlNode* pChild = pParent->GetChildren();
	while (pChild)
	{
		if (pChild->GetName() == name) return pChild;
		pChild = pChild->GetNext();
	}

	return NULL;
}

wxXmlNode* DocumentBase::GetNextXml(wxXmlNode* pChild, const wxString& name)
{
	if (!pChild) return NULL;

	wxXmlNode* pCurrChild = pChild->GetNext();
	while (pCurrChild)
	{
		if (pCurrChild->GetName() == name) return pCurrChild;
		pCurrChild = pCurrChild->GetNext();
	}

	return NULL;
}
