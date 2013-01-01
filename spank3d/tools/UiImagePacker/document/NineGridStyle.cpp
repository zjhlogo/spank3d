/*!
 * \file NineGridStyle.cpp
 * \date 2-14-2012 15:16:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "NineGridStyle.h"
#include "ImagePieceDocument.h"
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

NineGridStyle::NineGridStyle()
{
	memset(m_NineGridInfo, 0, sizeof(m_NineGridInfo));
}

NineGridStyle::~NineGridStyle()
{
	// TODO: 
}

bool NineGridStyle::LoadFromXml(wxXmlNode* pNodeNineGridStyle)
{
	if (!pNodeNineGridStyle) return false;

	wxString strId = pNodeNineGridStyle->GetAttribute(wxT("id"));
	SetId(strId);

	if (pNodeNineGridStyle->GetAttribute(wxT("auto_bitmap")) == wxT("true"))
	{
		SetAutoGenBitmap(true);
	}

	if (pNodeNineGridStyle->GetAttribute(wxT("flip_x")) == wxT("true"))
	{
		SetFlipX(true);
	}

	if (pNodeNineGridStyle->GetAttribute(wxT("flip_y")) == wxT("true"))
	{
		SetFlipY(true);
	}

	if (!LoadStateInfo(m_NineGridInfo[SS_NORMAL], pNodeNineGridStyle, wxT("normal"))) return false;
	LoadStateInfo(m_NineGridInfo[SS_DOWN], pNodeNineGridStyle, wxT("down"));
	LoadStateInfo(m_NineGridInfo[SS_HOVER], pNodeNineGridStyle, wxT("hover"));
	LoadStateInfo(m_NineGridInfo[SS_DISABLED], pNodeNineGridStyle, wxT("disabled"));

	return true;
}

bool NineGridStyle::SaveToXml(wxXmlNode* pNodeNineGridStyleList)
{
	if (!pNodeNineGridStyleList) return false;

	wxXmlNode* pNodeNineGridStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NineGridStyle"));
	pNodeNineGridStyle->AddAttribute(wxT("id"), GetId());
	if (IsAutoGenBitmap()) pNodeNineGridStyle->AddAttribute(wxT("auto_bitmap"), wxT("true"));
	if (IsFlipX()) pNodeNineGridStyle->AddAttribute(wxT("flip_x"), wxT("true"));
	if (IsFlipY()) pNodeNineGridStyle->AddAttribute(wxT("flip_y"), wxT("true"));

	if (!SaveStateInfo(pNodeNineGridStyle, wxT("normal"), m_NineGridInfo[SS_NORMAL]))
	{
		SAFE_DELETE(pNodeNineGridStyle);
		return false;
	}

	if (!IsNormalGrid(m_NineGridInfo[SS_DOWN])) SaveStateInfo(pNodeNineGridStyle, wxT("down"), m_NineGridInfo[SS_DOWN]);
	if (!IsNormalGrid(m_NineGridInfo[SS_HOVER])) SaveStateInfo(pNodeNineGridStyle, wxT("hover"), m_NineGridInfo[SS_HOVER]);
	if (!IsNormalGrid(m_NineGridInfo[SS_DISABLED])) SaveStateInfo(pNodeNineGridStyle, wxT("disabled"), m_NineGridInfo[SS_DISABLED]);

	pNodeNineGridStyleList->AddChild(pNodeNineGridStyle);
	return true;
}

bool NineGridStyle::SetStatePiece(const PieceInfo* pPieceInfo, STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_NineGridInfo[eState].pPieceInfo == pPieceInfo) return false;

	m_NineGridInfo[eState].pPieceInfo = pPieceInfo;
	BoundingRect(m_NineGridInfo[eState]);

	return true;
}

bool NineGridStyle::SetStateMinX(int value, IStyle::STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_NineGridInfo[eState].min_x == value) return false;

	m_NineGridInfo[eState].min_x = value;
	BoundingRect(m_NineGridInfo[eState]);

	return true;
}

bool NineGridStyle::SetStateMinY(int value, IStyle::STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_NineGridInfo[eState].min_y == value) return false;

	m_NineGridInfo[eState].min_y = value;
	BoundingRect(m_NineGridInfo[eState]);

	return true;
}

bool NineGridStyle::SetStateMaxX(int value, IStyle::STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_NineGridInfo[eState].max_x == value) return false;

	m_NineGridInfo[eState].max_x = value;
	BoundingRect(m_NineGridInfo[eState]);

	return true;
}

bool NineGridStyle::SetStateMaxY(int value, IStyle::STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_NineGridInfo[eState].max_y == value) return false;

	m_NineGridInfo[eState].max_y = value;
	BoundingRect(m_NineGridInfo[eState]);

	return true;
}

const NineGridStyle::NINE_GRID_INFO* NineGridStyle::GetStateGridInfo(STYLE_STATE eState) const
{
	return &m_NineGridInfo[eState];
}

bool NineGridStyle::LoadStateInfo(NINE_GRID_INFO& NineGridInfoOut, wxXmlNode* pNodeNineGridStyle, const wxString& strState)
{
	wxXmlNode* pNodeState = this->FindXmlChild(pNodeNineGridStyle, strState);
	if (!pNodeState) return false;

	wxString strPieceId = pNodeState->GetAttribute(wxT("piece_id"));
	if (strPieceId.length() <= 0) return false;

	const PieceInfo* pPieceInfo = ImagePieceDocument::GetInstance().FindPieceInfo(strPieceId);
	if (!pPieceInfo)
	{
		wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("can not found piece id=%s, for 9-grid style id=%s"), strPieceId, GetId()));
		msg.ShowModal();
		return false;
	}

	NineGridInfoOut.pPieceInfo = pPieceInfo;
	wxString value = pNodeState->GetAttribute(wxT("min_x"));
	value.ToLong((long*)&NineGridInfoOut.min_x);
	value = pNodeState->GetAttribute(wxT("min_y"));
	value.ToLong((long*)&NineGridInfoOut.min_y);
	value = pNodeState->GetAttribute(wxT("max_x"));
	value.ToLong((long*)&NineGridInfoOut.max_x);
	value = pNodeState->GetAttribute(wxT("max_y"));
	value.ToLong((long*)&NineGridInfoOut.max_y);

	return true;
}

bool NineGridStyle::SaveStateInfo(wxXmlNode* pNodeNineGridStyle, const wxString& strState, const NINE_GRID_INFO& NineGridInfo)
{
	if (!NineGridInfo.pPieceInfo) return false;
	if (NineGridInfo.min_x == NineGridInfo.max_x && NineGridInfo.min_y == NineGridInfo.max_y) return false;
	if (NineGridInfo.min_x > NineGridInfo.max_x || NineGridInfo.min_y > NineGridInfo.max_y) return false;

	wxXmlNode* pNodeState = new wxXmlNode(wxXML_ELEMENT_NODE, strState);
	pNodeState->AddAttribute(wxT("piece_id"), NineGridInfo.pPieceInfo->GetId());
	pNodeState->AddAttribute(wxT("min_x"), wxString::Format(wxT("%d"), NineGridInfo.min_x));
	pNodeState->AddAttribute(wxT("min_y"), wxString::Format(wxT("%d"), NineGridInfo.min_y));
	pNodeState->AddAttribute(wxT("max_x"), wxString::Format(wxT("%d"), NineGridInfo.max_x));
	pNodeState->AddAttribute(wxT("max_y"), wxString::Format(wxT("%d"), NineGridInfo.max_y));
	pNodeNineGridStyle->AddChild(pNodeState);

	return true;
}

bool NineGridStyle::IsNormalGrid(const NINE_GRID_INFO& NineGridInfo)
{
	if (NineGridInfo.min_x != m_NineGridInfo[SS_NORMAL].min_x) return false;
	if (NineGridInfo.min_y != m_NineGridInfo[SS_NORMAL].min_y) return false;
	if (NineGridInfo.max_x != m_NineGridInfo[SS_NORMAL].max_x) return false;
	if (NineGridInfo.max_y != m_NineGridInfo[SS_NORMAL].max_y) return false;
	if (NineGridInfo.pPieceInfo != m_NineGridInfo[SS_NORMAL].pPieceInfo) return false;
	return true;
}

void NineGridStyle::BoundingRect(NINE_GRID_INFO& gridInfo)
{
	if (gridInfo.min_x < 0) gridInfo.min_x = 0;
	if (gridInfo.min_y < 0) gridInfo.min_y = 0;

	int maxWidth = 0;
	if (gridInfo.pPieceInfo) maxWidth = gridInfo.pPieceInfo->GetRect().width;

	int maxHeight = 0;
	if (gridInfo.pPieceInfo) maxHeight = gridInfo.pPieceInfo->GetRect().height;

	if (gridInfo.max_x > maxWidth) gridInfo.max_x = maxWidth;
	if (gridInfo.max_y > maxHeight) gridInfo.max_y = maxHeight;

	if (gridInfo.min_x > gridInfo.max_x) gridInfo.min_x = gridInfo.max_x;
	if (gridInfo.min_y > gridInfo.max_y) gridInfo.min_y = gridInfo.max_y;

	if (gridInfo.max_x < gridInfo.min_x) gridInfo.max_x = gridInfo.min_x;
	if (gridInfo.max_y < gridInfo.min_y) gridInfo.max_y = gridInfo.min_y;
}
