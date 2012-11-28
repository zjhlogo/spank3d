/*!
 * \file DocBitmapStyle.cpp
 * \date 11-22-2012 9:30:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocBitmapStyle.h"
#include "State.h"
#include "DocProject.h"
#include <wx/xml/xml.h>

DocBitmapStyle::DocBitmapStyle()
{
	// TODO: 
}

DocBitmapStyle::~DocBitmapStyle()
{
	Reset();
}

DocBitmapStyle::BITMAP_STYLE* DocBitmapStyle::FindBitmapStyle(const wxString& strId)
{
	TM_BITMAP_STYLE::iterator it = m_BitmapStyleMap.find(strId);
	if (it == m_BitmapStyleMap.end()) return NULL;
	return it->second;
}

DocBitmapStyle::TM_BITMAP_STYLE& DocBitmapStyle::GetBitmapStyleMap()
{
	return m_BitmapStyleMap;
}

bool DocBitmapStyle::DoOpen(const wxString& strFile)
{
	Reset();

	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlBitmapStyleList = doc.GetRoot();
	if (!pXmlBitmapStyleList || pXmlBitmapStyleList->GetName() != wxT("BitmapStyleList")) return false;

	wxXmlNode* pXmlBitmapStyle = XmlUtil::FindChildNode(pXmlBitmapStyleList, wxT("BitmapStyle"));
	while (pXmlBitmapStyle)
	{
		// parse piece info
		BITMAP_STYLE* pBitmapStyle = FromXml(pXmlBitmapStyle);
		if (pBitmapStyle) m_BitmapStyleMap.insert(std::make_pair(pBitmapStyle->strId, pBitmapStyle));
		pXmlBitmapStyle = XmlUtil::FindNextSiblingNode(pXmlBitmapStyle, wxT("BitmapStyle"));
	}

	return true;
}

bool DocBitmapStyle::DoSave(const wxString& strFile)
{
	wxXmlNode* pXmlBitmapStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("BitmapStyleList"));

	for (TM_BITMAP_STYLE::const_iterator it = m_BitmapStyleMap.begin(); it != m_BitmapStyleMap.end(); ++it)
	{
		const BITMAP_STYLE* pBitmapStyle = it->second;
		wxXmlNode* pXmlBitmapStyle = ToXml(pBitmapStyle);
		if (pXmlBitmapStyle) pXmlBitmapStyleList->AddChild(pXmlBitmapStyle);
	}

	wxXmlDocument doc;
	doc.SetRoot(pXmlBitmapStyleList);
	return doc.Save(strFile);
}

void DocBitmapStyle::Reset()
{
	for (TM_BITMAP_STYLE::iterator it = m_BitmapStyleMap.begin(); it != m_BitmapStyleMap.end(); ++it)
	{
		BITMAP_STYLE* pBitmapStyle = it->second;
		wxDELETE(pBitmapStyle);
	}
	m_BitmapStyleMap.clear();
}

DocBitmapStyle::BITMAP_STYLE* DocBitmapStyle::FromXml(wxXmlNode* pXmlBitmapStyle)
{
	BITMAP_STYLE* pBitmapStyle = new BITMAP_STYLE();
	if (!DocGraphicsStyle::ParseStyleInfo(*pBitmapStyle, pXmlBitmapStyle))
	{
		wxDELETE(pBitmapStyle);
		return NULL;
	}

	// parse state info
	wxXmlNode* pXmlState = XmlUtil::FindChildNode(pXmlBitmapStyle, wxT("State"));
	while (pXmlState)
	{
		// parse state
		BITMAP_STATE_INFO stateInfo;
		stateInfo.state = State::GetStateValue(pXmlState->GetAttribute(wxT("id")));
		stateInfo.pPieceInfo = DocProject::GetInstance().FindPieceInfo(pXmlState->GetAttribute(wxT("piece")));
		pBitmapStyle->vBitmapState.push_back(stateInfo);

		pXmlState = XmlUtil::FindNextSiblingNode(pXmlState, wxT("State"));
	}

	return pBitmapStyle;
}

wxXmlNode* DocBitmapStyle::ToXml(const BITMAP_STYLE* pBitmapStyle)
{
	wxXmlNode* pXmlBitmapStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("BitmapStyle"));
	if (!DocGraphicsStyle::WriteStyleInfo(pXmlBitmapStyle, *pBitmapStyle))
	{
		wxDELETE(pXmlBitmapStyle);
		return NULL;
	}

	// write state info
	for (TV_BITMAP_STATE_INFO::const_iterator it = pBitmapStyle->vBitmapState.begin(); it != pBitmapStyle->vBitmapState.end(); ++it)
	{
		const BITMAP_STATE_INFO& stateInfo = (*it);

		// write state
		wxXmlNode* pXmlState = new wxXmlNode(wxXML_ATTRIBUTE_NODE, wxT("State"));
		pXmlState->AddAttribute(wxT("id"), State::GetStateString(stateInfo.state));
		pXmlState->AddAttribute(wxT("piece"), stateInfo.pPieceInfo->strId);

		pXmlBitmapStyle->AddChild(pXmlState);
	}

	return pXmlBitmapStyle;
}
