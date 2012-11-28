/*!
 * \file DocGraphicsStyle.cpp
 * \date 11-27-2012 11:37:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocGraphicsStyle.h"

DocGraphicsStyle::DocGraphicsStyle()
{
	// TODO: 
}

DocGraphicsStyle::~DocGraphicsStyle()
{
	// TODO: 
}

bool DocGraphicsStyle::ParseStyleInfo(STYLE_INFO& styleInfo, wxXmlNode* pXmlGraphicsStyle)
{
	styleInfo.strId = pXmlGraphicsStyle->GetAttribute(wxT("id"));

	styleInfo.minWidth = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("minW"));
	styleInfo.minHeight = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("minH"));

	styleInfo.maxWidth = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("maxW"), SHRT_MAX);
	styleInfo.maxHeight = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("maxH"), SHRT_MAX);

	styleInfo.bestWidth = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("bestW"), styleInfo.minWidth);
	styleInfo.bestHeight = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("bestH"), styleInfo.minHeight);

	styleInfo.paddingL = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("paddingL"));
	styleInfo.paddingT = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("paddingT"));
	styleInfo.paddingR = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("paddingR"));
	styleInfo.paddingB = XmlUtil::GetIntAttribute(pXmlGraphicsStyle, wxT("paddingB"));

	return true;
}

bool DocGraphicsStyle::WriteStyleInfo(wxXmlNode* pXmlGraphicsStyle, const STYLE_INFO& styleInfo)
{
	pXmlGraphicsStyle->AddAttribute(wxT("id"), styleInfo.strId);

	XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("minW"), styleInfo.minWidth);
	XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("minH"), styleInfo.minHeight);

	if (SHRT_MAX != styleInfo.maxWidth) XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("maxW"), styleInfo.maxWidth);
	if (SHRT_MAX != styleInfo.maxHeight) XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("maxH"), styleInfo.maxHeight);

	if (styleInfo.minWidth != styleInfo.bestWidth) XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("bestW"), styleInfo.bestWidth);
	if (styleInfo.minHeight != styleInfo.bestHeight) XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("bestH"), styleInfo.bestHeight);

	XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("paddingL"), styleInfo.paddingL);
	XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("paddingT"), styleInfo.paddingT);
	XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("paddingR"), styleInfo.paddingR);
	XmlUtil::SetIntAttribute(pXmlGraphicsStyle, wxT("paddingB"), styleInfo.paddingB);

	return true;
}
