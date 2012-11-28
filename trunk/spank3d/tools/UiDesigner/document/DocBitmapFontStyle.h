/*!
 * \file DocBitmapFontStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCBITMAPFONTSTYLE_H__
#define __DOCBITMAPFONTSTYLE_H__

#include "DocGraphicsStyle.h"
#include "DocPieceInfo.h"
#include <vector>
#include <map>

class DocBitmapFontStyle : public DocGraphicsStyle
{
public:
	typedef struct BITMAP_FONT_STATE_INFO_tag
	{
		unsigned int state;
		unsigned int color;
	} BITMAP_FONT_STATE_INFO;

	typedef std::vector<BITMAP_FONT_STATE_INFO> TV_BITMAP_FONT_STATE_INFO;

	typedef struct BITMAP_FONT_STYLE_tag : public STYLE_INFO
	{
		TV_BITMAP_FONT_STATE_INFO vBitmapFontState;
	} BITMAP_FONT_STYLE;

	typedef std::map<wxString, BITMAP_FONT_STYLE*> TM_BITMAP_FONT_STYLE;

public:
	DocBitmapFontStyle();
	virtual ~DocBitmapFontStyle();

	BITMAP_FONT_STYLE* FindBitmapFontStyle(const wxString& strId);
	TM_BITMAP_FONT_STYLE& GetBitmapFontStyleMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	BITMAP_FONT_STYLE* FromXml(wxXmlNode* pXmlBitmapFontStyle);
	wxXmlNode* ToXml(const BITMAP_FONT_STYLE* pBitmapFontStyle);

private:
	TM_BITMAP_FONT_STYLE m_BitmapFontStyleMap;

};
#endif // __DOCBITMAPFONTSTYLE_H__
