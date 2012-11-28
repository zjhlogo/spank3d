/*!
 * \file DocGraphicsStyle.h
 * \date 11-27-2012 11:36:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCGRAPHICSSTYLE_H__
#define __DOCGRAPHICSSTYLE_H__

#include "IDoc.h"

class DocGraphicsStyle : public IDoc
{
public:
	typedef struct STYLE_INFO_tag
	{
		wxString strId;
		int minWidth, minHeight;
		int maxWidth, maxHeight;
		int bestWidth, bestHeight;
		int paddingL, paddingT;
		int paddingR, paddingB;
	} STYLE_INFO;

public:
	DocGraphicsStyle();
	virtual ~DocGraphicsStyle();

protected:
	bool ParseStyleInfo(STYLE_INFO& styleInfo, wxXmlNode* pXmlGraphicsStyle);
	bool WriteStyleInfo(wxXmlNode* pXmlGraphicsStyle, const STYLE_INFO& styleInfo);

};
#endif // __DOCGRAPHICSSTYLE_H__
