/*!
 * \file DocPieceInfo.cpp
 * \date 11-22-2012 9:30:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocPieceInfo.h"

DocPieceInfo::DocPieceInfo()
{
	// TODO: 
}

DocPieceInfo::~DocPieceInfo()
{
	Reset();
}

DocPieceInfo::PIECE_INFO* DocPieceInfo::FindPieceInfo(const wxString& strId)
{
	TM_PIECE_INFO::iterator it = m_PieceInfoMap.find(strId);
	if (it == m_PieceInfoMap.end()) return NULL;
	return it->second;
}

DocPieceInfo::TM_PIECE_INFO& DocPieceInfo::GetPieceInfoMap()
{
	return m_PieceInfoMap;
}

bool DocPieceInfo::DoOpen(const wxString& strFile)
{
	Reset();

	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlPieceInfoList = doc.GetRoot();
	if (!pXmlPieceInfoList || pXmlPieceInfoList->GetName() != wxT("PieceInfoList")) return false;

	m_strTexture = pXmlPieceInfoList->GetAttribute(wxT("texture"));
	wxXmlNode* pXmlPieceInfo = XmlUtil::FindChildNode(pXmlPieceInfoList, wxT("PieceInfo"));
	while (pXmlPieceInfo)
	{
		// parse piece info
		PIECE_INFO* pPieceInfo = FromXml(pXmlPieceInfo);
		if (pPieceInfo) m_PieceInfoMap.insert(std::make_pair(pPieceInfo->strId, pPieceInfo));
		pXmlPieceInfo = XmlUtil::FindNextSiblingNode(pXmlPieceInfo, wxT("PieceInfo"));
	}

	return true;
}

bool DocPieceInfo::DoSave(const wxString& strFile)
{
	wxXmlNode* pXmlPieceInfoList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("PieceInfoList"));
	pXmlPieceInfoList->AddAttribute(wxT("texture"), m_strTexture);

	for (TM_PIECE_INFO::const_iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		const PIECE_INFO* pPieceInfo = it->second;
		wxXmlNode* pXmlPieceInfo = ToXml(pPieceInfo);
		if (pXmlPieceInfo) pXmlPieceInfoList->AddChild(pXmlPieceInfo);
	}

	wxXmlDocument doc;
	doc.SetRoot(pXmlPieceInfoList);
	return doc.Save(strFile);
}

void DocPieceInfo::Reset()
{
	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		PIECE_INFO* pPieceInfo = it->second;
		wxDELETE(pPieceInfo);
	}
	m_PieceInfoMap.clear();
}

DocPieceInfo::PIECE_INFO* DocPieceInfo::FromXml(wxXmlNode* pXmlPieceInfo)
{
	PIECE_INFO* pPieceInfo = new PIECE_INFO();
	pPieceInfo->strId = pXmlPieceInfo->GetAttribute(wxT("id"));
	pPieceInfo->x = XmlUtil::GetIntAttribute(pXmlPieceInfo, wxT("x"));
	pPieceInfo->y = XmlUtil::GetIntAttribute(pXmlPieceInfo, wxT("y"));
	pPieceInfo->width = XmlUtil::GetIntAttribute(pXmlPieceInfo, wxT("width"));
	pPieceInfo->height = XmlUtil::GetIntAttribute(pXmlPieceInfo, wxT("height"));
	return pPieceInfo;
}

wxXmlNode* DocPieceInfo::ToXml(const PIECE_INFO* pPieceInfo)
{
	wxXmlNode* pXmlPieceInfo = new wxXmlNode(wxXML_ATTRIBUTE_NODE, wxT("PieceInfo"));
	pXmlPieceInfo->AddAttribute(wxT("id"), pPieceInfo->strId);
	XmlUtil::SetIntAttribute(pXmlPieceInfo, wxT("x"), pPieceInfo->x);
	XmlUtil::SetIntAttribute(pXmlPieceInfo, wxT("y"), pPieceInfo->y);
	XmlUtil::SetIntAttribute(pXmlPieceInfo, wxT("width"), pPieceInfo->width);
	XmlUtil::SetIntAttribute(pXmlPieceInfo, wxT("height"), pPieceInfo->height);
	return pXmlPieceInfo;
}
