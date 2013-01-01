/*!
 * \file IStyle.cpp
 * \date 2-14-2012 13:59:16
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "IStyle.h"

IStyle::IStyle()
{
	m_AutoGenBitmap = false;
	m_FlipX = false;
	m_FlipY = false;
}

IStyle::~IStyle()
{
	// TODO: 
}

void IStyle::SetId(const wxString& strId)
{
	m_strId = strId;
}

const wxString& IStyle::GetId() const
{
	return m_strId;
}

void IStyle::SetTreeItemId(wxTreeItemId itemId)
{
	m_TreeItemId = itemId;
}

const wxTreeItemId& IStyle::GetTreeItemId() const
{
	return m_TreeItemId;
}

wxXmlNode* IStyle::FindXmlChild(wxXmlNode* pParent, const wxString& name)
{
	wxXmlNode* pChild = pParent->GetChildren();
	while (pChild)
	{
		if (pChild->GetName() == name) return pChild;
		pChild = pChild->GetNext();
	}

	return NULL;
}

void IStyle::SetAutoGenBitmap(bool bAutoGenBitmap)
{
	m_AutoGenBitmap = bAutoGenBitmap;
}

bool IStyle::IsAutoGenBitmap() const
{
	return m_AutoGenBitmap;
}

void IStyle::SetFlipX(bool bFlipX)
{
	m_FlipX = bFlipX;
}

bool IStyle::IsFlipX() const
{
	return m_FlipX;
}

void IStyle::SetFlipY(bool bFlipY)
{
	m_FlipY = bFlipY;
}

bool IStyle::IsFlipY() const
{
	return m_FlipY;
}