/*!
 * \file BitmapFontStyle.h
 * \date 9-25-2012 21:20:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BITMAPFONTSTYLE_H__
#define __BITMAPFONTSTYLE_H__

#include "IFontStyle.h"
#include "IFontInfo.h"
#include <tinyxml-2.6.2/tinyxml.h>
#include <vector>

class BitmapFontStyle : public IFontStyle
{
public:
	typedef struct BITMAP_FONT_INFO_tag
	{
		uint nState;
		IFontInfo* pFontInfo;
		uint color;
	} BITMAP_FONT_INFO;

	typedef std::vector<BITMAP_FONT_INFO> TV_BITMAP_FONT_INFO;

public:
	RTTI_DEF(BitmapFontStyle, IFontStyle);

	BitmapFontStyle(const tstring& id);
	virtual ~BitmapFontStyle();

	virtual float GetLineHeight(uint state) const;
	virtual Vector2 CalcSize(const tstring& strText, uint state);
	virtual bool Render(const tstring& strText, const Vector2& pos, const Rect& clipRect, uint state);

	bool LoadFromXml(TiXmlElement* pXmlBitmapFontStyle);

private:
	const BITMAP_FONT_INFO* FindFontInfo(uint state) const;
	bool RenderText(const BITMAP_FONT_INFO& bitmapFontInfo, const tstring& strText, const Vector2& pos, const Rect& clipRect);

private:
	TV_BITMAP_FONT_INFO m_vBitmapFontInfo;

};
#endif // __BITMAPFONTSTYLE_H__
