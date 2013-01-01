/*!
 * \file ClipBitmapStyle.cpp
 * \date 2-14-2012 16:44:09
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ClipBitmapStyle.h"
#include "ImagePieceDocument.h"
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

ClipBitmapStyle::ClipBitmapStyle()
{
	memset(m_PieceInfo, 0, sizeof(m_PieceInfo));
}

ClipBitmapStyle::~ClipBitmapStyle()
{
	// TODO: 
}

bool ClipBitmapStyle::LoadFromXml(wxXmlNode* pNodeClipBitmapStyle)
{
	if (!pNodeClipBitmapStyle) return false;

	wxString strId = pNodeClipBitmapStyle->GetAttribute(wxT("id"));
	SetId(strId);

	if (pNodeClipBitmapStyle->GetAttribute(wxT("auto_bitmap")) == wxT("true"))
	{
		SetAutoGenBitmap(true);
	}

	if (pNodeClipBitmapStyle->GetAttribute(wxT("flip_x")) == wxT("true"))
	{
		SetFlipX(true);
	}

	if (pNodeClipBitmapStyle->GetAttribute(wxT("flip_y")) == wxT("true"))
	{
		SetFlipY(true);
	}

	m_PieceInfo[SS_NORMAL] = LoadStateInfo(pNodeClipBitmapStyle, wxT("normal"), NULL);
	if (!m_PieceInfo[SS_NORMAL]) return false;

	m_PieceInfo[SS_DOWN] = LoadStateInfo(pNodeClipBitmapStyle, wxT("down"), NULL);
	m_PieceInfo[SS_HOVER] = LoadStateInfo(pNodeClipBitmapStyle, wxT("hover"), NULL);
	m_PieceInfo[SS_DISABLED] = LoadStateInfo(pNodeClipBitmapStyle, wxT("disabled"), NULL);

	return true;
}

bool ClipBitmapStyle::SaveToXml(wxXmlNode* pNodeClipBitmapStyleList)
{
	if (!pNodeClipBitmapStyleList) return false;

	wxXmlNode* pNodeClipBitmapStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ClipBitmapStyle"));
	pNodeClipBitmapStyle->AddAttribute(wxT("id"), GetId());
	if (IsAutoGenBitmap()) pNodeClipBitmapStyle->AddAttribute(wxT("auto_bitmap"), wxT("true"));
	if (IsFlipX()) pNodeClipBitmapStyle->AddAttribute(wxT("flip_x"), wxT("true"));
	if (IsFlipY()) pNodeClipBitmapStyle->AddAttribute(wxT("flip_y"), wxT("true"));

	if (!SaveStateInfo(pNodeClipBitmapStyle, wxT("normal"), m_PieceInfo[SS_NORMAL]))
	{
		SAFE_DELETE(pNodeClipBitmapStyle);
		return false;
	}

	if (m_PieceInfo[SS_NORMAL] != m_PieceInfo[SS_DOWN]) SaveStateInfo(pNodeClipBitmapStyle, wxT("down"), m_PieceInfo[SS_DOWN]);
	if (m_PieceInfo[SS_NORMAL] != m_PieceInfo[SS_HOVER]) SaveStateInfo(pNodeClipBitmapStyle, wxT("hover"), m_PieceInfo[SS_HOVER]);
	if (m_PieceInfo[SS_NORMAL] != m_PieceInfo[SS_DISABLED]) SaveStateInfo(pNodeClipBitmapStyle, wxT("disabled"), m_PieceInfo[SS_DISABLED]);

	pNodeClipBitmapStyleList->AddChild(pNodeClipBitmapStyle);
	return true;
}

bool ClipBitmapStyle::SetStatePiece(const PieceInfo* pPieceInfo, STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_PieceInfo[eState] == pPieceInfo) return false;
	m_PieceInfo[eState] = pPieceInfo;
	return true;
}

const PieceInfo* ClipBitmapStyle::GetStatePiece(STYLE_STATE eState) const
{
	if (eState < 0 || eState >= SS_NUM) return NULL;

	return m_PieceInfo[eState];
}

const PieceInfo* ClipBitmapStyle::LoadStateInfo(wxXmlNode* pNodeClipBitmapStyle, const wxString& strState, const PieceInfo* pDefaultPieceInfo)
{
	wxXmlNode* pNodeState = this->FindXmlChild(pNodeClipBitmapStyle, strState);
	if (!pNodeState) return pDefaultPieceInfo;

	wxString strPieceId = pNodeState->GetAttribute(wxT("piece_id"));
	if (strPieceId.length() <= 0) return pDefaultPieceInfo;

	const PieceInfo* pPieceInfo = ImagePieceDocument::GetInstance().FindPieceInfo(strPieceId);
	if (!pPieceInfo)
	{
		wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("can not found piece id=%s, for clip bitmap style id=%s"), strPieceId, GetId()));
		msg.ShowModal();
		return pDefaultPieceInfo;
	}

	return pPieceInfo;
}

bool ClipBitmapStyle::SaveStateInfo(wxXmlNode* pNodeClipBitmapStyle, const wxString& strState, const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return false;

	wxXmlNode* pNodeState = new wxXmlNode(wxXML_ELEMENT_NODE, strState);
	pNodeState->AddAttribute(wxT("piece_id"), pPieceInfo->GetId());
	pNodeClipBitmapStyle->AddChild(pNodeState);

	return true;
}
