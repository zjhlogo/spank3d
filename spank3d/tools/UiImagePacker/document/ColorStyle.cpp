/*!
 * \file ColorStyle.cpp
 * \date 2-14-2012 16:23:47
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ColorStyle.h"
#include "../Config.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

ColorStyle::ColorStyle()
{
	memset(m_nColors, 0, sizeof(m_nColors));
}

ColorStyle::~ColorStyle()
{
	// TODO: 
}

bool ColorStyle::LoadFromXml(wxXmlNode* pNodeColorStyle)
{
	if (!pNodeColorStyle) return false;

	wxString strId = pNodeColorStyle->GetAttribute(wxT("id"));
	SetId(strId);

	m_nColors[SS_NORMAL] = LoadStateInfo(pNodeColorStyle, wxT("normal"), Config::DEFAULT_COLOR);
	m_nColors[SS_DOWN] = LoadStateInfo(pNodeColorStyle, wxT("down"), m_nColors[SS_NORMAL]);
	m_nColors[SS_HOVER] = LoadStateInfo(pNodeColorStyle, wxT("hover"), m_nColors[SS_NORMAL]);
	m_nColors[SS_DISABLED] = LoadStateInfo(pNodeColorStyle, wxT("disabled"), m_nColors[SS_NORMAL]);

	return true;
}

bool ColorStyle::SaveToXml(wxXmlNode* pNodeColorStyleList)
{
	if (!pNodeColorStyleList) return false;

	wxXmlNode* pNodeColorStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ColorStyle"));
	pNodeColorStyle->AddAttribute(wxT("id"), GetId());

	SaveStateInfo(pNodeColorStyle, wxT("normal"), m_nColors[SS_NORMAL]);
	if (m_nColors[SS_NORMAL] != m_nColors[SS_DOWN]) SaveStateInfo(pNodeColorStyle, wxT("down"), m_nColors[SS_DOWN]);
	if (m_nColors[SS_NORMAL] != m_nColors[SS_HOVER]) SaveStateInfo(pNodeColorStyle, wxT("hover"), m_nColors[SS_HOVER]);
	if (m_nColors[SS_NORMAL] != m_nColors[SS_DISABLED]) SaveStateInfo(pNodeColorStyle, wxT("disabled"), m_nColors[SS_DISABLED]);

	pNodeColorStyleList->AddChild(pNodeColorStyle);
	return true;
}

bool ColorStyle::SaveStateInfo(wxXmlNode* pNodeColorStyle, const wxString& strState, unsigned int nColor)
{
	wxXmlNode* pNodeState = new wxXmlNode(wxXML_ELEMENT_NODE, strState);

	wxString value = wxString::Format(wxT("0x%x"), GetReserveColor(nColor));
	pNodeState->AddAttribute(wxT("value"), value);
	pNodeColorStyle->AddChild(pNodeState);

	return true;
}

bool ColorStyle::SetStateColor(unsigned int color, STYLE_STATE eState)
{
	if (eState < 0 || eState >= SS_NUM) return false;
	if (m_nColors[eState] == color) return false;
	m_nColors[eState] = color;
	return true;
}

unsigned int ColorStyle::GetStateColor(STYLE_STATE eState) const
{
	if (eState < 0 || eState >= SS_NUM) return Config::DEFAULT_COLOR;
	return m_nColors[eState];
}

unsigned int ColorStyle::LoadStateInfo(wxXmlNode* pNodeColorStyle, const wxString& strState, unsigned int nDefaultColor)
{
	wxXmlNode* pNodeState = this->FindXmlChild(pNodeColorStyle, strState);
	if (!pNodeState) return nDefaultColor;

	wxString strColor = pNodeState->GetAttribute(wxT("value"));
	if (strColor.length() <= 0) return nDefaultColor;

	unsigned int nColor = 0;
	_stscanf_s(strColor.c_str(), wxT("%x"), &nColor);

	return GetReserveColor(nColor);
}

unsigned int ColorStyle::GetReserveColor(unsigned int color)
{
	unsigned int r = (color & 0xFF0000) >> 16;
	unsigned int g = (color & 0xFF00) >> 8;
	unsigned int b = (color & 0xFF);

	return (b << 16) | (g << 8) | (r);
}
