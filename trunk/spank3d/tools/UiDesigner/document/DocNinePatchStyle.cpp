/*!
 * \file DocNinePatchStyle.cpp
 * \date 11-22-2012 9:30:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocNinePatchStyle.h"
#include "State.h"
#include "DocProject.h"
#include <wx/xml/xml.h>

DocNinePatchStyle::DocNinePatchStyle()
{
	// TODO: 
}

DocNinePatchStyle::~DocNinePatchStyle()
{
	Reset();
}

DocNinePatchStyle::NINE_PATCH_STYLE* DocNinePatchStyle::FindNinePatchStyle(const wxString& strId)
{
	TM_NINE_PATCH_STYLE::iterator it = m_NinePatchStyleMap.find(strId);
	if (it == m_NinePatchStyleMap.end()) return NULL;
	return it->second;
}

DocNinePatchStyle::TM_NINE_PATCH_STYLE& DocNinePatchStyle::GetNinePatchStyleMap()
{
	return m_NinePatchStyleMap;
}

bool DocNinePatchStyle::DoOpen(const wxString& strFile)
{
	Reset();

	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlNinePatchStyleList = doc.GetRoot();
	if (!pXmlNinePatchStyleList || pXmlNinePatchStyleList->GetName() != wxT("NinePatchStyleList")) return false;

	wxXmlNode* pXmlNinePatchStyle = XmlUtil::FindChildNode(pXmlNinePatchStyleList, wxT("NinePatchStyle"));
	while (pXmlNinePatchStyle)
	{
		// parse piece info
		NINE_PATCH_STYLE* pNinePatchStyle = FromXml(pXmlNinePatchStyle);
		if (pNinePatchStyle) m_NinePatchStyleMap.insert(std::make_pair(pNinePatchStyle->strId, pNinePatchStyle));
		pXmlNinePatchStyle = XmlUtil::FindNextSiblingNode(pXmlNinePatchStyle, wxT("NinePatchStyle"));
	}

	return true;
}

bool DocNinePatchStyle::DoSave(const wxString& strFile)
{
	wxXmlNode* pXmlNinePatchStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NinePatchStyleList"));

	for (TM_NINE_PATCH_STYLE::const_iterator it = m_NinePatchStyleMap.begin(); it != m_NinePatchStyleMap.end(); ++it)
	{
		const NINE_PATCH_STYLE* pNinePatchStyle = it->second;
		wxXmlNode* pXmlNinePatchStyle = ToXml(pNinePatchStyle);
		if (pXmlNinePatchStyle) pXmlNinePatchStyleList->AddChild(pXmlNinePatchStyle);
	}

	wxXmlDocument doc;
	doc.SetRoot(pXmlNinePatchStyleList);
	return doc.Save(strFile);
}

void DocNinePatchStyle::Reset()
{
	for (TM_NINE_PATCH_STYLE::iterator it = m_NinePatchStyleMap.begin(); it != m_NinePatchStyleMap.end(); ++it)
	{
		NINE_PATCH_STYLE* pNinePatchStyle = it->second;
		wxDELETE(pNinePatchStyle);
	}
	m_NinePatchStyleMap.clear();
}

DocNinePatchStyle::NINE_PATCH_STYLE* DocNinePatchStyle::FromXml(wxXmlNode* pXmlNinePatchStyle)
{
	NINE_PATCH_STYLE* pNinePatchStyle = new NINE_PATCH_STYLE();
	if (!DocGraphicsStyle::ParseStyleInfo(*pNinePatchStyle, pXmlNinePatchStyle))
	{
		wxDELETE(pNinePatchStyle);
		return NULL;
	}

	// parse state info
	wxXmlNode* pXmlState = XmlUtil::FindChildNode(pXmlNinePatchStyle, wxT("State"));
	while (pXmlState)
	{
		// parse state
		NINE_PATCH_STATE_INFO stateInfo;
		stateInfo.state = State::GetStateValue(pXmlState->GetAttribute(wxT("id")));
		stateInfo.pPieceInfo = DocProject::GetInstance().FindPieceInfo(pXmlState->GetAttribute(wxT("piece")));
		stateInfo.minX = XmlUtil::GetIntAttribute(pXmlState, wxT("minX"));
		stateInfo.minY = XmlUtil::GetIntAttribute(pXmlState, wxT("minY"));
		stateInfo.maxX = XmlUtil::GetIntAttribute(pXmlState, wxT("maxX"));
		stateInfo.maxY = XmlUtil::GetIntAttribute(pXmlState, wxT("maxY"));
		pNinePatchStyle->vNinePatchState.push_back(stateInfo);

		pXmlState = XmlUtil::FindNextSiblingNode(pXmlState, wxT("State"));
	}

	return pNinePatchStyle;
}

wxXmlNode* DocNinePatchStyle::ToXml(const NINE_PATCH_STYLE* pNinePatchStyle)
{
	wxXmlNode* pXmlNinePatchStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NinePatchStyle"));
	if (!DocGraphicsStyle::WriteStyleInfo(pXmlNinePatchStyle, *pNinePatchStyle))
	{
		wxDELETE(pXmlNinePatchStyle);
		return NULL;
	}

	// write state info
	for (TV_NINE_PATCH_STATE_INFO::const_iterator it = pNinePatchStyle->vNinePatchState.begin(); it != pNinePatchStyle->vNinePatchState.end(); ++it)
	{
		const NINE_PATCH_STATE_INFO& stateInfo = (*it);

		// write state
		wxXmlNode* pXmlState = new wxXmlNode(wxXML_ATTRIBUTE_NODE, wxT("State"));
		pXmlState->AddAttribute(wxT("id"), State::GetStateString(stateInfo.state));
		pXmlState->AddAttribute(wxT("piece"), stateInfo.pPieceInfo->strId);
		XmlUtil::SetIntAttribute(pXmlState, wxT("minX"), stateInfo.minX);
		XmlUtil::SetIntAttribute(pXmlState, wxT("minY"), stateInfo.minY);
		XmlUtil::SetIntAttribute(pXmlState, wxT("maxX"), stateInfo.maxX);
		XmlUtil::SetIntAttribute(pXmlState, wxT("maxY"), stateInfo.maxY);

		pXmlNinePatchStyle->AddChild(pXmlState);
	}

	return pXmlNinePatchStyle;
}
