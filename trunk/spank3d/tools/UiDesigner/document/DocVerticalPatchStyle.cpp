/*!
 * \file DocVerticalPatchStyle.cpp
 * \date 11-22-2012 9:30:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocVerticalPatchStyle.h"
#include "State.h"
#include "DocProject.h"
#include <wx/xml/xml.h>

DocVerticalPatchStyle::DocVerticalPatchStyle()
{
	// TODO: 
}

DocVerticalPatchStyle::~DocVerticalPatchStyle()
{
	Reset();
}

DocVerticalPatchStyle::VERTICAL_PATCH_STYLE* DocVerticalPatchStyle::FindVerticalPatchStyle(const wxString& strId)
{
	TM_VERTICAL_PATCH_STYLE::iterator it = m_VerticalPatchStyleMap.find(strId);
	if (it == m_VerticalPatchStyleMap.end()) return NULL;
	return it->second;
}

DocVerticalPatchStyle::TM_VERTICAL_PATCH_STYLE& DocVerticalPatchStyle::GetVerticalPatchStyleMap()
{
	return m_VerticalPatchStyleMap;
}

bool DocVerticalPatchStyle::DoOpen(const wxString& strFile)
{
	Reset();

	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlVerticalPatchStyleList = doc.GetRoot();
	if (!pXmlVerticalPatchStyleList || pXmlVerticalPatchStyleList->GetName() != wxT("VerticalPatchStyleList")) return false;

	wxXmlNode* pXmlVerticalPatchStyle = XmlUtil::FindChildNode(pXmlVerticalPatchStyleList, wxT("VerticalPatchStyle"));
	while (pXmlVerticalPatchStyle)
	{
		// parse piece info
		VERTICAL_PATCH_STYLE* pVerticalPatchStyle = FromXml(pXmlVerticalPatchStyle);
		if (pVerticalPatchStyle) m_VerticalPatchStyleMap.insert(std::make_pair(pVerticalPatchStyle->strId, pVerticalPatchStyle));
		pXmlVerticalPatchStyle = XmlUtil::FindNextSiblingNode(pXmlVerticalPatchStyle, wxT("VerticalPatchStyle"));
	}

	return true;
}

bool DocVerticalPatchStyle::DoSave(const wxString& strFile)
{
	wxXmlNode* pXmlVerticalPatchStyleList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("VerticalPatchStyleList"));

	for (TM_VERTICAL_PATCH_STYLE::const_iterator it = m_VerticalPatchStyleMap.begin(); it != m_VerticalPatchStyleMap.end(); ++it)
	{
		const VERTICAL_PATCH_STYLE* pVerticalPatchStyle = it->second;
		wxXmlNode* pXmlVerticalPatchStyle = ToXml(pVerticalPatchStyle);
		if (pXmlVerticalPatchStyle) pXmlVerticalPatchStyleList->AddChild(pXmlVerticalPatchStyle);
	}

	wxXmlDocument doc;
	doc.SetRoot(pXmlVerticalPatchStyleList);
	return doc.Save(strFile);
}

void DocVerticalPatchStyle::Reset()
{
	for (TM_VERTICAL_PATCH_STYLE::iterator it = m_VerticalPatchStyleMap.begin(); it != m_VerticalPatchStyleMap.end(); ++it)
	{
		VERTICAL_PATCH_STYLE* pVerticalPatchStyle = it->second;
		wxDELETE(pVerticalPatchStyle);
	}
	m_VerticalPatchStyleMap.clear();
}

DocVerticalPatchStyle::VERTICAL_PATCH_STYLE* DocVerticalPatchStyle::FromXml(wxXmlNode* pXmlVerticalPatchStyle)
{
	VERTICAL_PATCH_STYLE* pVerticalPatchStyle = new VERTICAL_PATCH_STYLE();
	if (!DocGraphicsStyle::ParseStyleInfo(*pVerticalPatchStyle, pXmlVerticalPatchStyle))
	{
		wxDELETE(pVerticalPatchStyle);
		return NULL;
	}

	// parse state info
	wxXmlNode* pXmlState = XmlUtil::FindChildNode(pXmlVerticalPatchStyle, wxT("State"));
	while (pXmlState)
	{
		// parse state
		VERTICAL_PATCH_STATE_INFO stateInfo;
		stateInfo.state = State::GetStateValue(pXmlState->GetAttribute(wxT("id")));
		stateInfo.pPieceInfo = DocProject::GetInstance().FindPieceInfo(pXmlState->GetAttribute(wxT("piece")));
		stateInfo.minY = XmlUtil::GetIntAttribute(pXmlState, wxT("minY"));
		stateInfo.maxY = XmlUtil::GetIntAttribute(pXmlState, wxT("maxY"));
		pVerticalPatchStyle->vVerticalPatchState.push_back(stateInfo);

		pXmlState = XmlUtil::FindNextSiblingNode(pXmlState, wxT("State"));
	}

	return pVerticalPatchStyle;
}

wxXmlNode* DocVerticalPatchStyle::ToXml(const VERTICAL_PATCH_STYLE* pVerticalPatchStyle)
{
	wxXmlNode* pXmlVerticalPatchStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("VerticalPatchStyle"));
	if (!DocGraphicsStyle::WriteStyleInfo(pXmlVerticalPatchStyle, *pVerticalPatchStyle))
	{
		wxDELETE(pXmlVerticalPatchStyle);
		return NULL;
	}

	// write state info
	for (TV_VERTICAL_PATCH_STATE_INFO::const_iterator it = pVerticalPatchStyle->vVerticalPatchState.begin(); it != pVerticalPatchStyle->vVerticalPatchState.end(); ++it)
	{
		const VERTICAL_PATCH_STATE_INFO& stateInfo = (*it);

		// write state
		wxXmlNode* pXmlState = new wxXmlNode(wxXML_ATTRIBUTE_NODE, wxT("State"));
		pXmlState->AddAttribute(wxT("id"), State::GetStateString(stateInfo.state));
		pXmlState->AddAttribute(wxT("piece"), stateInfo.pPieceInfo->strId);
		XmlUtil::SetIntAttribute(pXmlState, wxT("minY"), stateInfo.minY);
		XmlUtil::SetIntAttribute(pXmlState, wxT("maxY"), stateInfo.maxY);

		pXmlVerticalPatchStyle->AddChild(pXmlState);
	}

	return pXmlVerticalPatchStyle;
}
