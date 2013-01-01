/*!
 * \file BitmapStyle.cpp
 * \date 2-14-2012 13:52:46
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "BitmapStyle.h"
#include "ImagePieceDocument.h"
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

BitmapStyle::BitmapStyle()
{
	memset(m_PieceInfo, 0, sizeof(m_PieceInfo));
}

BitmapStyle::~BitmapStyle()
{
	// TODO: 
}

bool BitmapStyle::LoadFromXml(wxXmlNode* pNodeBitmapStyle)
{
	if (!pNodeBitmapStyle) return false;

	wxString strId = pNodeBitmapStyle->GetAttribute(wxT("id"));
	SetId(strId);

	if (pNodeBitmapStyle->GetAttribute(wxT("auto_bitmap")) == wxT("true"))
	{
		SetAutoGenBitmap(true);
	}

	if (pNodeBitmapStyle->GetAttribute(wxT("flip_x")) == wxT("true"))
	{
		SetFlipX(true);
	}

	if (pNodeBitmapStyle->GetAttribute(wxT("flip_y")) == wxT("true"))
	{
		SetFlipY(true);
	}

	m_PieceInfo[SS_NORMAL] = LoadStateInfo(pNodeBitmapStyle, wxT("normal"), NULL);
	if (!m_PieceInfo[SS_NORMAL]) return false;

	m_PieceInfo[SS_DOWN] = LoadStateInfo(pNodeBitmapStyle, wxT("down"), NULL);
	m_PieceInfo[SS_HOVER] = LoadStateInfo(pNodeBitmapStyle, wxT("hover"), NULL);
	m_PieceInfo[SS_DISABLED] = LoadStateInfo(pNodeBitmapStyle, wxT("disabled"), NULL);

	return true;
}

bool BitmapStyle::SaveToXml(wxXmlNode* pNodeBitmapStyleList)
{
	if (!pNodeBitmapStyleList) return false;

	wxXmlNode* pNodeBitmapStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("BitmapStyle"));
	pNodeBitmapStyle->AddAttribute(wxT("id"), GetId());
	if (IsAutoGenBitmap()) pNodeBitmapStyle->AddAttribute(wxT("auto_bitmap"), wxT("true"));
	if (IsFlipX()) pNodeBitmapStyle->AddAttribute(wxT("flip_x"), wxT("true"));
	if (IsFlipY()) pNodeBitmapStyle->AddAttribute(wxT("flip_y"), wxT("true"));

	if (!SaveStateInfo(pNodeBitmapStyle, wxT("normal"), m_PieceInfo[SS_NORMAL]))
	{
		SAFE_DELETE(pNodeBitmapStyle);
		return false;
	}

	if (m_PieceInfo[SS_NORMAL] != m_PieceInfo[SS_DOWN]) SaveStateInfo(pNodeBitmapStyle, wxT("down"), m_PieceInfo[SS_DOWN]);
	if (m_PieceInfo[SS_NORMAL] != m_PieceInfo[SS_HOVER]) SaveStateInfo(pNodeBitmapStyle, wxT("hover"), m_PieceInfo[SS_HOVER]);
	if (m_PieceInfo[SS_NORMAL] != m_PieceInfo[SS_DISABLED]) SaveStateInfo(pNodeBitmapStyle, wxT("disabled"), m_PieceInfo[SS_DISABLED]);

	pNodeBitmapStyleList->AddChild(pNodeBitmapStyle);
	return true;
}

bool BitmapStyle::SetStatePiece(const PieceInfo* pPieceInfo, STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_PieceInfo[eState] == pPieceInfo) return false;
	m_PieceInfo[eState] = pPieceInfo;
	return true;
}

const PieceInfo* BitmapStyle::GetStatePiece(STYLE_STATE eState) const
{
	if (eState < 0 || eState >= SS_NUM) return NULL;
	return m_PieceInfo[eState];
}

const PieceInfo* BitmapStyle::LoadStateInfo(wxXmlNode* pNodeBitmapStyle, const wxString& strState, const PieceInfo* pDefaultPieceInfo)
{
	wxXmlNode* pNodeState = FindXmlChild(pNodeBitmapStyle, strState);
	if (!pNodeState) return pDefaultPieceInfo;

	wxString wxPieceId = pNodeState->GetAttribute(wxT("piece_id"));
	if (wxPieceId.length() <= 0) return pDefaultPieceInfo;

	const PieceInfo* pPieceInfo = ImagePieceDocument::GetInstance().FindPieceInfo(wxPieceId);
	if (!pPieceInfo)
	{
		wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("can not found piece id=%s, for bitmap style id=%s"), wxPieceId, GetId()));
		msg.ShowModal();
		return pDefaultPieceInfo;
	}

	return pPieceInfo;
}

bool BitmapStyle::SaveStateInfo(wxXmlNode* pNodeBitmapStyle, const wxString& strState, const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return false;

	wxXmlNode* pNodeState = new wxXmlNode(wxXML_ELEMENT_NODE, strState);
	pNodeState->AddAttribute(wxT("piece_id"), pPieceInfo->GetId());
	pNodeBitmapStyle->AddChild(pNodeState);

	return true;
}
