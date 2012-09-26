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

public:
	RTTI_DEF(BitmapFontStyle, IFontStyle);

	BitmapFontStyle(const tstring& id);
	virtual ~BitmapFontStyle();

	virtual bool Render(const tstring& strText, const Vector2& pos, const Rect& clipRect, uint state);
	bool LoadFontFile(const tstring& strFile);

private:
	bool ParseCommonInfo(TiXmlElement* pXmlCommon);
	bool CreateTextures(TiXmlElement* pXmlPages);
	void DestroyTextures();
	bool CreateCharsInfo(TiXmlElement* pXmlCharsInfo);
	bool CreateKerningsInfo(TiXmlElement* pXmlKerningsInfo);

private:
	int m_nLineHeight;
	int m_nNumPages;
	TV_TEXTURE m_vTextures;
	TM_CHAR_INFO m_CharInfoMap;
	TM_UINT_FLOAT m_KerningMap;

};
#endif // __BITMAPFONTSTYLE_H__
