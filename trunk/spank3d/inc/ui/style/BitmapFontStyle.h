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
#include <tinyxml-2.6.2/tinyxml.h>
#include <vector>

class BitmapFontStyle : public IFontStyle
{
public:
	typedef std::vector<ITexture*> TV_TEXTURE;
	typedef std::map<uint, float> TM_UINT_FLOAT;

	typedef struct BITMAP_FONT_INFO_tag
	{
		uint nState;
		uint color;
	} BITMAP_FONT_INFO;

	typedef std::vector<BITMAP_FONT_INFO> TV_BITMAP_FONT_INFO;

public:
	RTTI_DEF(BitmapFontStyle, IFontStyle);

	BitmapFontStyle(const tstring& id);
	virtual ~BitmapFontStyle();

	virtual float GetLineHeight() const;
	virtual bool Render(const tstring& strText, const Vector2& pos, const Rect& clipRect, uint state);
	bool LoadFromXml(TiXmlElement* pXmlBitmapFontStyle);

private:
	bool RenderText(const BITMAP_FONT_INFO& bitmapFontInfo, const tstring& strText, const Vector2& pos, const Rect& clipRect);

	bool LoadFontFile(const tstring& strFile);
	bool ParseCommonInfo(TiXmlElement* pXmlCommon);
	bool CreateTextures(TiXmlElement* pXmlPages);
	void DestroyTextures();
	bool CreateCharsInfo(TiXmlElement* pXmlCharsInfo);
	bool CreateKerningsInfo(TiXmlElement* pXmlKerningsInfo);

private:
	float m_fLineHeight;
	int m_nNumPages;
	TV_TEXTURE m_vTextures;
	TM_CHAR_INFO m_CharInfoMap;
	TM_UINT_FLOAT m_KerningMap;
	TV_BITMAP_FONT_INFO m_vBitmapFontInfo;

};
#endif // __BITMAPFONTSTYLE_H__
