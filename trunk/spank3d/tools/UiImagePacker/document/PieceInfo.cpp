/*!
 * \file PieceInfo.cpp
 * \date 2-13-2012 19:33:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "PieceInfo.h"
#include "ImagePieceDocument.h"

PieceInfo::PieceInfo()
{
	m_pImageInfo = NULL;
}

PieceInfo::~PieceInfo()
{
	// TODO: 
}

bool PieceInfo::LoadFromXml(wxXmlNode* pNodePiece)
{
	if (!pNodePiece) return false;

	m_strId = pNodePiece->GetAttribute(wxT("id"));
	if (m_strId.length() <= 0) return false;

	wxString strImageId = pNodePiece->GetAttribute(wxT("image_id"));
	m_pImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	if (!m_pImageInfo) return false;

	wxString value;
	if (!pNodePiece->GetAttribute(wxT("x"), &value)) return false;
	value.ToLong((long*)&m_Rect.x);
	if (!pNodePiece->GetAttribute(wxT("y"), &value)) return false;
	value.ToLong((long*)&m_Rect.y);
	if (!pNodePiece->GetAttribute(wxT("width"), &value)) return false;
	value.ToLong((long*)&m_Rect.width);
	if (!pNodePiece->GetAttribute(wxT("height"), &value)) return false;
	value.ToLong((long*)&m_Rect.height);

	return true;
}

bool PieceInfo::SaveToXml(wxXmlNode* pNodePieceList)
{
	wxXmlNode* pNodePiece = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Piece"));

	pNodePiece->AddAttribute(wxT("id"), m_strId);
	pNodePiece->AddAttribute(wxT("image_id"), m_pImageInfo->GetId());

	pNodePiece->AddAttribute(wxT("x"), wxString::Format(wxT("%d"), m_Rect.x));
	pNodePiece->AddAttribute(wxT("y"), wxString::Format(wxT("%d"), m_Rect.y));
	pNodePiece->AddAttribute(wxT("width"), wxString::Format(wxT("%d"), m_Rect.width));
	pNodePiece->AddAttribute(wxT("height"), wxString::Format(wxT("%d"), m_Rect.height));

	pNodePieceList->AddChild(pNodePiece);

	return true;
}

void PieceInfo::SetId(const wxString& strId)
{
	m_strId = strId;
}

const wxString& PieceInfo::GetId() const
{
	return m_strId;
}

void PieceInfo::SetImageInfo(const ImageInfo* pImageInfo)
{
	m_pImageInfo = pImageInfo;
}

const ImageInfo* PieceInfo::GetImageInfo() const
{
	return m_pImageInfo;
}

void PieceInfo::SetRect(const wxRect& rect)
{
	m_Rect = rect;
}

const wxRect& PieceInfo::GetRect() const
{
	return m_Rect;
}

void PieceInfo::SetTreeItemId(const wxTreeItemId& itemId)
{
	m_TreeItemId = itemId;
}

const wxTreeItemId& PieceInfo::GetTreeItemId() const
{
	return m_TreeItemId;
}
