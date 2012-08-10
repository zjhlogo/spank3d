/*!
 * \file ColorStyle.h
 * \date 2-14-2012 16:21:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __COLORSTYLE_H__
#define __COLORSTYLE_H__

#include "IStyle.h"

class ColorStyle : public IStyle
{
public:
	ColorStyle();
	~ColorStyle();

	bool LoadFromXml(wxXmlNode* pNodeColorStyle);
	bool SaveToXml(wxXmlNode* pNodeColorStyleList);

	bool SetStateColor(unsigned int color, STYLE_STATE eState);
	unsigned int GetStateColor(STYLE_STATE eState) const;

private:
	unsigned int LoadStateInfo(wxXmlNode* pNodeColorStyle, const wxString& strState, unsigned int nDefaultColor);
	bool SaveStateInfo(wxXmlNode* pNodeColorStyle, const wxString& strState, unsigned int nColor);
	unsigned int GetReserveColor(unsigned int color);

private:
	unsigned int m_nColors[SS_NUM];

};
#endif // __COLORSTYLE_H__
