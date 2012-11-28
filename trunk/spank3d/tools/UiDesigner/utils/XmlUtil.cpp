/*!
 * \file XmlUtil.cpp
 * \date 11-27-2012 13:39:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "XmlUtil.h"


int XmlUtil::GetIntAttribute(wxXmlNode* pXml, const wxString& strName, int defaultValue /*= 0*/)
{
	wxString strValue;
	if (!pXml->GetAttribute(strName, &strValue)) return defaultValue;

	long lValue;
	if (!strValue.ToLong(&lValue)) return defaultValue;

	return int(lValue);
}

float XmlUtil::GetFloatAttribute(wxXmlNode* pXml, const wxString& strName, float defaultValue /* = 0.0f */)
{
	wxString strValue;
	if (!pXml->GetAttribute(strName, &strValue)) return defaultValue;

	double dValue;
	if (!strValue.ToDouble(&dValue)) return defaultValue;

	return float(dValue);
}

void XmlUtil::SetIntAttribute(wxXmlNode* pXml, const wxString& strName, int value)
{
	pXml->AddAttribute(strName, wxString::Format(wxT("%d"), value));
}

void XmlUtil::SetFloatAttribute(wxXmlNode* pXml, const wxString& strName, float value)
{
	pXml->AddAttribute(strName, wxString::Format(wxT("%f"), value));
}

wxXmlNode* XmlUtil::FindChildNode(wxXmlNode* pNode, const wxString& name)
{
	if (!pNode) return NULL;

	wxXmlNode* pChild = pNode->GetChildren();
	while (pChild)
	{
		if (pChild->GetName() == name) return pChild;
		pChild = pChild->GetNext();
	}

	return NULL;
}

wxXmlNode* XmlUtil::FindNextSiblingNode(wxXmlNode* pNode, const wxString& name)
{
	if (!pNode) return NULL;

	wxXmlNode* pCurrNode = pNode->GetNext();
	while (pCurrNode)
	{
		if (pCurrNode->GetName() == name) return pCurrNode;
		pCurrNode = pCurrNode->GetNext();
	}

	return NULL;
}
