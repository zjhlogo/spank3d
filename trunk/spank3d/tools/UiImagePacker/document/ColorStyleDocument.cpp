/*!
 * \file ColorStyleDocument.cpp
 * \date 2-14-2012 16:55:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ColorStyleDocument.h"
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

ColorStyleDocument::ColorStyleDocument()
{
	// TODO: 
}

ColorStyleDocument::~ColorStyleDocument()
{
	Reset();
}

ColorStyleDocument& ColorStyleDocument::GetInstance()
{
	static ColorStyleDocument s_ColorStyleDocument;
	return s_ColorStyleDocument;
}

bool ColorStyleDocument::OpenFile(const wxString& strFile)
{
	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	Reset();
	SetFilePath(strFile);

	wxXmlNode* pNodeColorStyleList = doc.GetRoot();
	if (!pNodeColorStyleList || pNodeColorStyleList->GetName() != wxT("ColorStyleList")) return false;

	// parse image list
	wxXmlNode* pNodeColorStyle = this->FindXmlChild(pNodeColorStyleList, wxT("ColorStyle"));
	while (pNodeColorStyle)
	{
		ColorStyle* pColorStyle = new ColorStyle();
		if (!pColorStyle->LoadFromXml(pNodeColorStyle))
		{
			wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("load color style failed, id=%s"), pColorStyle->GetId()));
			msg.ShowModal();
			SAFE_DELETE(pColorStyle);
			SetModifiedFlag();
		}
		else
		{
			m_ColorStyleMap.insert(std::make_pair(pColorStyle->GetId(), pColorStyle));
		}
		pNodeColorStyle = this->GetNextXml(pNodeColorStyle, wxT("ColorStyle"));
	}

	return true;
}

bool ColorStyleDocument::SaveFile(const wxString& strFile)
{
	if (!IsModified()) return true;
	ClearModifiedFlag();

	wxXmlDocument doc;

	wxXmlNode* pNodeColorStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ColorStyleList"));
	doc.SetRoot(pNodeColorStyleList);

	for (TM_COLOR_STYLE::iterator it = m_ColorStyleMap.begin(); it != m_ColorStyleMap.end(); ++it)
	{
		ColorStyle* pColorStyle = it->second;
		if (!pColorStyle->SaveToXml(pNodeColorStyleList))
		{
			wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("save color style failed, id=%s"), pColorStyle->GetId()));
			msg.ShowModal();
		}
	}

	return doc.Save(strFile);
}

void ColorStyleDocument::Reset()
{
	SetFilePath(wxEmptyString);
	for (TM_COLOR_STYLE::iterator it = m_ColorStyleMap.begin(); it != m_ColorStyleMap.end(); ++it)
	{
		ColorStyle* pColorStyle = it->second;
		SAFE_DELETE(pColorStyle);
	}
	m_ColorStyleMap.clear();
	ClearModifiedFlag();
}

const ColorStyle* ColorStyleDocument::FindColorStyle(const wxString& strId)
{
	return InternalFindColorStyle(strId);
}

const ColorStyleDocument::TM_COLOR_STYLE& ColorStyleDocument::GetColorStyleMap()
{
	return m_ColorStyleMap;
}

bool ColorStyleDocument::RenameColorStyleId(const ColorStyle* pColorStyle, const wxString& strNewId)
{
	if (!pColorStyle) return false;

	wxString strOldId = pColorStyle->GetId();
	if (strOldId == strNewId) return true;
	if (FindColorStyle(strNewId)) return false;

	TM_COLOR_STYLE::iterator itfound = m_ColorStyleMap.find(strOldId);
	if (itfound == m_ColorStyleMap.end()) return false;
	SetModifiedFlag();

	ColorStyle* pFoundColorStyle = itfound->second;
	m_ColorStyleMap.erase(itfound);

	pFoundColorStyle->SetId(strNewId);
	m_ColorStyleMap.insert(std::make_pair(pFoundColorStyle->GetId(), pFoundColorStyle));

	return true;
}

bool ColorStyleDocument::SetStateColor(const ColorStyle* pColorStyle, unsigned int color, IStyle::STYLE_STATE eState)
{
	if (!pColorStyle) return false;

	ColorStyle* pFoundColorStyle = InternalFindColorStyle(pColorStyle->GetId());
	if (!pFoundColorStyle) return false;
	SetModifiedFlag();

	return pFoundColorStyle->SetStateColor(color, eState);
}

const ColorStyle* ColorStyleDocument::AddColorStyle(const wxString& strId)
{
	if (strId.empty()) return NULL;
	SetModifiedFlag();

	wxString strNewId = GenerateNewColorStyleId(strId);
	ColorStyle* pNewColorStyle = new ColorStyle();
	pNewColorStyle->SetId(strNewId);
	m_ColorStyleMap.insert(std::make_pair(pNewColorStyle->GetId(), pNewColorStyle));

	return pNewColorStyle;
}

bool ColorStyleDocument::RemoveColorStyle(const wxString& strId)
{
	TM_COLOR_STYLE::iterator itfound = m_ColorStyleMap.find(strId);
	if (itfound == m_ColorStyleMap.end()) return false;
	SetModifiedFlag();

	ColorStyle* pFoundColorStyle = (itfound->second);
	m_ColorStyleMap.erase(itfound);

	SAFE_DELETE(pFoundColorStyle);
	return true;
}

ColorStyle* ColorStyleDocument::InternalFindColorStyle(const wxString& strId)
{
	TM_COLOR_STYLE::iterator itfound = m_ColorStyleMap.find(strId);
	if (itfound == m_ColorStyleMap.end()) return NULL;
	return itfound->second;
}

wxString ColorStyleDocument::GenerateNewColorStyleId(const wxString& strId)
{
	wxString strNewId = strId;
	int index = 0;

	while (FindColorStyle(strNewId))
	{
		strNewId = wxString::Format(wxT("%s%d"), strId, index++);
	}

	return strNewId;
}
