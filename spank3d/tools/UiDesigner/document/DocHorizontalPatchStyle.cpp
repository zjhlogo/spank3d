/*!
 * \file DocHorizontalPatchStyle.cpp
 * \date 11-22-2012 9:30:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocHorizontalPatchStyle.h"
#include "State.h"
#include "DocProject.h"
#include <wx/xml/xml.h>

DocHorizontalPatchStyle::DocHorizontalPatchStyle()
{
	// TODO: 
}

DocHorizontalPatchStyle::~DocHorizontalPatchStyle()
{
	Reset();
}

DocHorizontalPatchStyle::HORIZONTAL_PATCH_STYLE* DocHorizontalPatchStyle::FindHorizontalPatchStyle(const wxString& strId)
{
	TM_HORIZONTAL_PATCH_STYLE::iterator it = m_HorizontalPatchStyleMap.find(strId);
	if (it == m_HorizontalPatchStyleMap.end()) return NULL;
	return it->second;
}

DocHorizontalPatchStyle::TM_HORIZONTAL_PATCH_STYLE& DocHorizontalPatchStyle::GetHorizontalPatchStyleMap()
{
	return m_HorizontalPatchStyleMap;
}

bool DocHorizontalPatchStyle::DoOpen(const wxString& strFile)
{
	Reset();

	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlHorizontalPatchStyleList = doc.GetRoot();
	if (!pXmlHorizontalPatchStyleList || pXmlHorizontalPatchStyleList->GetName() != wxT("HorizontalPatchStyleList")) return false;

	wxXmlNode* pXmlHorizontalPatchStyle = XmlUtil::FindChildNode(pXmlHorizontalPatchStyleList, wxT("HorizontalPatchStyle"));
	while (pXmlHorizontalPatchStyle)
	{
		// parse piece info
		HORIZONTAL_PATCH_STYLE* pHorizontalPatchStyle = FromXml(pXmlHorizontalPatchStyle);
		if (pHorizontalPatchStyle) m_HorizontalPatchStyleMap.insert(std::make_pair(pHorizontalPatchStyle->strId, pHorizontalPatchStyle));
		pXmlHorizontalPatchStyle = XmlUtil::FindNextSiblingNode(pXmlHorizontalPatchStyle, wxT("HorizontalPatchStyle"));
	}

	return true;
}

bool DocHorizontalPatchStyle::DoSave(const wxString& strFile)
{
	wxXmlNode* pXmlHorizontalPatchStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("HorizontalPatchStyleList"));

	for (TM_HORIZONTAL_PATCH_STYLE::const_iterator it = m_HorizontalPatchStyleMap.begin(); it != m_HorizontalPatchStyleMap.end(); ++it)
	{
		const HORIZONTAL_PATCH_STYLE* pHorizontalPatchStyle = it->second;
		wxXmlNode* pXmlHorizontalPatchStyle = ToXml(pHorizontalPatchStyle);
		if (pXmlHorizontalPatchStyle) pXmlHorizontalPatchStyleList->AddChild(pXmlHorizontalPatchStyle);
	}

	wxXmlDocument doc;
	doc.SetRoot(pXmlHorizontalPatchStyleList);
	return doc.Save(strFile);
}

void DocHorizontalPatchStyle::Reset()
{
	for (TM_HORIZONTAL_PATCH_STYLE::iterator it = m_HorizontalPatchStyleMap.begin(); it != m_HorizontalPatchStyleMap.end(); ++it)
	{
		HORIZONTAL_PATCH_STYLE* pHorizontalPatchStyle = it->second;
		wxDELETE(pHorizontalPatchStyle);
	}
	m_HorizontalPatchStyleMap.clear();
}

DocHorizontalPatchStyle::HORIZONTAL_PATCH_STYLE* DocHorizontalPatchStyle::FromXml(wxXmlNode* pXmlHorizontalPatchStyle)
{
	HORIZONTAL_PATCH_STYLE* pHorizontalPatchStyle = new HORIZONTAL_PATCH_STYLE();
	if (!DocGraphicsStyle::ParseStyleInfo(*pHorizontalPatchStyle, pXmlHorizontalPatchStyle))
	{
		wxDELETE(pHorizontalPatchStyle);
		return NULL;
	}

	// parse state info
	wxXmlNode* pXmlState = XmlUtil::FindChildNode(pXmlHorizontalPatchStyle, wxT("State"));
	while (pXmlState)
	{
		// parse state
		HORIZONTAL_PATCH_STATE_INFO stateInfo;
		stateInfo.state = State::GetStateValue(pXmlState->GetAttribute(wxT("id")));
		stateInfo.pPieceInfo = DocProject::GetInstance().FindPieceInfo(pXmlState->GetAttribute(wxT("piece")));
		stateInfo.minX = XmlUtil::GetIntAttribute(pXmlState, wxT("minX"));
		stateInfo.maxX = XmlUtil::GetIntAttribute(pXmlState, wxT("maxX"));
		pHorizontalPatchStyle->vHorizontalPatchState.push_back(stateInfo);

		pXmlState = XmlUtil::FindNextSiblingNode(pXmlState, wxT("State"));
	}

	return pHorizontalPatchStyle;
}

wxXmlNode* DocHorizontalPatchStyle::ToXml(const HORIZONTAL_PATCH_STYLE* pHorizontalPatchStyle)
{
	wxXmlNode* pXmlHorizontalPatchStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("HorizontalPatchStyle"));
	if (!DocGraphicsStyle::WriteStyleInfo(pXmlHorizontalPatchStyle, *pHorizontalPatchStyle))
	{
		wxDELETE(pXmlHorizontalPatchStyle);
		return NULL;
	}

	// write state info
	for (TV_HORIZONTAL_PATCH_STATE_INFO::const_iterator it = pHorizontalPatchStyle->vHorizontalPatchState.begin(); it != pHorizontalPatchStyle->vHorizontalPatchState.end(); ++it)
	{
		const HORIZONTAL_PATCH_STATE_INFO& stateInfo = (*it);

		// write state
		wxXmlNode* pXmlState = new wxXmlNode(wxXML_ATTRIBUTE_NODE, wxT("State"));
		pXmlState->AddAttribute(wxT("id"), State::GetStateString(stateInfo.state));
		pXmlState->AddAttribute(wxT("piece"), stateInfo.pPieceInfo->strId);
		XmlUtil::SetIntAttribute(pXmlState, wxT("minX"), stateInfo.minX);
		XmlUtil::SetIntAttribute(pXmlState, wxT("maxX"), stateInfo.maxX);

		pXmlHorizontalPatchStyle->AddChild(pXmlState);
	}

	return pXmlHorizontalPatchStyle;
}
