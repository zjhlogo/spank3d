/*!
 * \file DocBitmapFontStyle.cpp
 * \date 11-22-2012 9:30:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocBitmapFontStyle.h"
#include "State.h"
#include "DocProject.h"
#include "../utils/StringUtil.h"
#include <wx/xml/xml.h>

DocBitmapFontStyle::DocBitmapFontStyle()
{
	// TODO: 
}

DocBitmapFontStyle::~DocBitmapFontStyle()
{
	Reset();
}

DocBitmapFontStyle::BITMAP_FONT_STYLE* DocBitmapFontStyle::FindBitmapFontStyle(const wxString& strId)
{
	TM_BITMAP_FONT_STYLE::iterator it = m_BitmapFontStyleMap.find(strId);
	if (it == m_BitmapFontStyleMap.end()) return NULL;
	return it->second;
}

DocBitmapFontStyle::TM_BITMAP_FONT_STYLE& DocBitmapFontStyle::GetBitmapFontStyleMap()
{
	return m_BitmapFontStyleMap;
}

bool DocBitmapFontStyle::DoOpen(const wxString& strFile)
{
	Reset();

	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlBitmapFontStyleList = doc.GetRoot();
	if (!pXmlBitmapFontStyleList || pXmlBitmapFontStyleList->GetName() != wxT("BitmapFontStyleList")) return false;

	wxXmlNode* pXmlBitmapFontStyle = XmlUtil::FindChildNode(pXmlBitmapFontStyleList, wxT("BitmapFontStyle"));
	while (pXmlBitmapFontStyle)
	{
		// parse piece info
		BITMAP_FONT_STYLE* pBitmapFontStyle = FromXml(pXmlBitmapFontStyle);
		if (pBitmapFontStyle) m_BitmapFontStyleMap.insert(std::make_pair(pBitmapFontStyle->strId, pBitmapFontStyle));
		pXmlBitmapFontStyle = XmlUtil::FindNextSiblingNode(pXmlBitmapFontStyle, wxT("BitmapFontStyle"));
	}

	return true;
}

bool DocBitmapFontStyle::DoSave(const wxString& strFile)
{
	wxXmlNode* pXmlBitmapFontStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("BitmapFontStyleList"));

	for (TM_BITMAP_FONT_STYLE::const_iterator it = m_BitmapFontStyleMap.begin(); it != m_BitmapFontStyleMap.end(); ++it)
	{
		const BITMAP_FONT_STYLE* pBitmapFontStyle = it->second;
		wxXmlNode* pXmlBitmapFontStyle = ToXml(pBitmapFontStyle);
		if (pXmlBitmapFontStyle) pXmlBitmapFontStyleList->AddChild(pXmlBitmapFontStyle);
	}

	wxXmlDocument doc;
	doc.SetRoot(pXmlBitmapFontStyleList);
	return doc.Save(strFile);
}

void DocBitmapFontStyle::Reset()
{
	for (TM_BITMAP_FONT_STYLE::iterator it = m_BitmapFontStyleMap.begin(); it != m_BitmapFontStyleMap.end(); ++it)
	{
		BITMAP_FONT_STYLE* pBitmapFontStyle = it->second;
		wxDELETE(pBitmapFontStyle);
	}
	m_BitmapFontStyleMap.clear();
}

DocBitmapFontStyle::BITMAP_FONT_STYLE* DocBitmapFontStyle::FromXml(wxXmlNode* pXmlBitmapFontStyle)
{
	BITMAP_FONT_STYLE* pBitmapFontStyle = new BITMAP_FONT_STYLE();
	if (!DocGraphicsStyle::ParseStyleInfo(*pBitmapFontStyle, pXmlBitmapFontStyle))
	{
		wxDELETE(pBitmapFontStyle);
		return NULL;
	}

	// parse state info
	wxXmlNode* pXmlState = XmlUtil::FindChildNode(pXmlBitmapFontStyle, wxT("State"));
	while (pXmlState)
	{
		// parse state
		BITMAP_FONT_STATE_INFO stateInfo;
		stateInfo.state = State::GetStateValue(pXmlState->GetAttribute(wxT("id")));
		stateInfo.color = StringUtil::Str2Hex(pXmlState->GetAttribute(wxT("color")));
		pBitmapFontStyle->vBitmapFontState.push_back(stateInfo);

		pXmlState = XmlUtil::FindNextSiblingNode(pXmlState, wxT("State"));
	}

	return pBitmapFontStyle;
}

wxXmlNode* DocBitmapFontStyle::ToXml(const BITMAP_FONT_STYLE* pBitmapFontStyle)
{
	wxXmlNode* pXmlBitmapFontStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("BitmapFontStyle"));
	if (!DocGraphicsStyle::WriteStyleInfo(pXmlBitmapFontStyle, *pBitmapFontStyle))
	{
		wxDELETE(pXmlBitmapFontStyle);
		return NULL;
	}

	// write state info
	for (TV_BITMAP_FONT_STATE_INFO::const_iterator it = pBitmapFontStyle->vBitmapFontState.begin(); it != pBitmapFontStyle->vBitmapFontState.end(); ++it)
	{
		const BITMAP_FONT_STATE_INFO& stateInfo = (*it);

		// write state
		wxXmlNode* pXmlState = new wxXmlNode(wxXML_ATTRIBUTE_NODE, wxT("State"));
		pXmlState->AddAttribute(wxT("id"), State::GetStateString(stateInfo.state));
		pXmlState->AddAttribute(wxT("color"), StringUtil::Hex2Str(stateInfo.color));

		pXmlBitmapFontStyle->AddChild(pXmlState);
	}

	return pXmlBitmapFontStyle;
}
