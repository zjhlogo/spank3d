/*!
 * \file NineGridStyleDocument.cpp
 * \date 2-14-2012 15:40:37
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "NineGridStyleDocument.h"
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

NineGridStyleDocument::NineGridStyleDocument()
{
	// TODO: 
}

NineGridStyleDocument::~NineGridStyleDocument()
{
	Reset();
}

NineGridStyleDocument& NineGridStyleDocument::GetInstance()
{
	static NineGridStyleDocument s_NineGridStyleDocument;
	return s_NineGridStyleDocument;
}

bool NineGridStyleDocument::OpenFile(const wxString& strFile)
{
	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	Reset();
	SetFilePath(strFile);

	wxXmlNode* pNodeNineGridStyleList = doc.GetRoot();
	if (!pNodeNineGridStyleList || pNodeNineGridStyleList->GetName() != wxT("NineGridStyleList")) return false;

	// parse image list
	wxXmlNode* pNodeNineGridStyle = this->FindXmlChild(pNodeNineGridStyleList, wxT("NineGridStyle"));
	while (pNodeNineGridStyle)
	{
		NineGridStyle* pNineGridStyle = new NineGridStyle();
		if (!pNineGridStyle->LoadFromXml(pNodeNineGridStyle))
		{
			wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("load 9-grid style failed, id=%s"), pNineGridStyle->GetId()));
			msg.ShowModal();
			SAFE_DELETE(pNineGridStyle);
			SetModifiedFlag();
		}
		else
		{
			m_NineGridStyleMap.insert(std::make_pair(pNineGridStyle->GetId(), pNineGridStyle));
		}
		pNodeNineGridStyle = this->GetNextXml(pNodeNineGridStyle, wxT("NineGridStyle"));
	}

	return true;
}

bool NineGridStyleDocument::SaveFile(const wxString& strFile)
{
	if (!IsModified()) return true;
	ClearModifiedFlag();

	wxXmlDocument doc;
	wxXmlNode* pNodeNineGridStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NineGridStyleList"));
	doc.SetRoot(pNodeNineGridStyleList);

	for (TM_NINE_GRID_STYLE::iterator it = m_NineGridStyleMap.begin(); it != m_NineGridStyleMap.end(); ++it)
	{
		NineGridStyle* pNineGridStyle = it->second;
		if (!pNineGridStyle->SaveToXml(pNodeNineGridStyleList))
		{
			wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("save 9-grid style failed, id=%s"), pNineGridStyle->GetId()));
			msg.ShowModal();
		}
	}

	return doc.Save(strFile);
}

void NineGridStyleDocument::Reset()
{
	SetFilePath(wxEmptyString);
	for (TM_NINE_GRID_STYLE::iterator it = m_NineGridStyleMap.begin(); it != m_NineGridStyleMap.end(); ++it)
	{
		NineGridStyle* pNineGridStyle = it->second;
		SAFE_DELETE(pNineGridStyle);
	}
	m_NineGridStyleMap.clear();
	ClearModifiedFlag();
}

const NineGridStyle* NineGridStyleDocument::FindNineGridStyle(const wxString& strId)
{
	return InternalFindNineGridStyle(strId);
}

const NineGridStyleDocument::TM_NINE_GRID_STYLE& NineGridStyleDocument::GetNineGridStyleMap()
{
	return m_NineGridStyleMap;
}

int NineGridStyleDocument::EnumNineGridStyles(TV_NINE_GRID_STYLE& vNineGridStyleOut, const PieceInfo* pPieceInfo)
{
	int nFound = 0;
	for (TM_NINE_GRID_STYLE::const_iterator it = m_NineGridStyleMap.begin(); it != m_NineGridStyleMap.end(); ++it)
	{
		const NineGridStyle* pNineGridStyle = it->second;
		if (pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL)->pPieceInfo == pPieceInfo
			|| pNineGridStyle->GetStateGridInfo(IStyle::SS_DOWN)->pPieceInfo == pPieceInfo
			|| pNineGridStyle->GetStateGridInfo(IStyle::SS_HOVER)->pPieceInfo == pPieceInfo
			|| pNineGridStyle->GetStateGridInfo(IStyle::SS_DISABLED)->pPieceInfo == pPieceInfo)
		{
			vNineGridStyleOut.push_back(pNineGridStyle);
			nFound++;
		}
	}

	return nFound;
}

bool NineGridStyleDocument::RenameNineGridStyleId(const NineGridStyle* pNineGridStyle, const wxString& strNewId)
{
	if (!pNineGridStyle) return false;

	wxString strOldId = pNineGridStyle->GetId();
	if (strOldId == strNewId) return true;
	if (FindNineGridStyle(strNewId)) return false;

	TM_NINE_GRID_STYLE::iterator itfound = m_NineGridStyleMap.find(strOldId);
	if (itfound == m_NineGridStyleMap.end()) return false;
	SetModifiedFlag();

	NineGridStyle* pFoundNineGridStyle = itfound->second;
	m_NineGridStyleMap.erase(itfound);

	pFoundNineGridStyle->SetId(strNewId);
	m_NineGridStyleMap.insert(std::make_pair(pFoundNineGridStyle->GetId(), pFoundNineGridStyle));
	return true;
}

bool NineGridStyleDocument::SetAutoGenBitmap(const NineGridStyle* pNineGridStyle, bool bAutoGenBitmap)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	pFoundNineGridStyle->SetAutoGenBitmap(bAutoGenBitmap);
	return true;
}

bool NineGridStyleDocument::SetFlipX(const NineGridStyle* pNineGridStyle, bool bFlipX)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	pFoundNineGridStyle->SetFlipX(bFlipX);
	return true;
}

bool NineGridStyleDocument::SetFlipY(const NineGridStyle* pNineGridStyle, bool bFlipY)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	pFoundNineGridStyle->SetFlipY(bFlipY);
	return true;
}

bool NineGridStyleDocument::SetStatePiece(const NineGridStyle* pNineGridStyle, const PieceInfo* pPieceInfo, IStyle::STYLE_STATE eState)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	return pFoundNineGridStyle->SetStatePiece(pPieceInfo, eState);
}

bool NineGridStyleDocument::SetStateMinX(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	return pFoundNineGridStyle->SetStateMinX(value, eState);
}

bool NineGridStyleDocument::SetStateMinY(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	return pFoundNineGridStyle->SetStateMinY(value, eState);
}

bool NineGridStyleDocument::SetStateMaxX(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	return pFoundNineGridStyle->SetStateMaxX(value, eState);
}

bool NineGridStyleDocument::SetStateMaxY(const NineGridStyle* pNineGridStyle, int value, IStyle::STYLE_STATE eState)
{
	if (!pNineGridStyle) return false;

	NineGridStyle* pFoundNineGridStyle = InternalFindNineGridStyle(pNineGridStyle->GetId());
	if (!pFoundNineGridStyle) return false;
	SetModifiedFlag();

	return pFoundNineGridStyle->SetStateMaxY(value, eState);
}

const NineGridStyle* NineGridStyleDocument::AddNineGridStyle(const wxString& strId)
{
	if (strId.empty()) return NULL;
	SetModifiedFlag();

	wxString strNewId = GenerateNewNineGridStyleId(strId);
	NineGridStyle* pNewNineGridStyle = new NineGridStyle();
	pNewNineGridStyle->SetId(strNewId);
	m_NineGridStyleMap.insert(std::make_pair(pNewNineGridStyle->GetId(), pNewNineGridStyle));

	return pNewNineGridStyle;
}

bool NineGridStyleDocument::RemoveNineGridStyle(const wxString& strId)
{
	TM_NINE_GRID_STYLE::iterator itfound = m_NineGridStyleMap.find(strId);
	if (itfound == m_NineGridStyleMap.end()) return false;
	SetModifiedFlag();

	NineGridStyle* pFoundNineGridStyle = (itfound->second);
	m_NineGridStyleMap.erase(itfound);

	SAFE_DELETE(pFoundNineGridStyle);
	return true;
}

NineGridStyle* NineGridStyleDocument::InternalFindNineGridStyle(const wxString& strId)
{
	TM_NINE_GRID_STYLE::iterator itfound = m_NineGridStyleMap.find(strId);
	if (itfound == m_NineGridStyleMap.end()) return NULL;
	return itfound->second;
}

wxString NineGridStyleDocument::GenerateNewNineGridStyleId(const wxString& strId)
{
	wxString strNewId = strId;
	int index = 0;

	while (FindNineGridStyle(strNewId))
	{
		strNewId = wxString::Format(wxT("%s%d"), strId, index++);
	}

	return strNewId;
}
