/*!
 * \file BitmapFontInfo.h
 * \date 10-20-2012 9:10:54
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BITMAPFONTINFO_H__
#define __BITMAPFONTINFO_H__

#include "IFontInfo.h"
#include <tinyxml-2.6.2/tinyxml.h>

class BitmapFontInfo : public IFontInfo
{
public:
	RTTI_DEF(BitmapFontInfo, IFontInfo);

	BitmapFontInfo(const tstring& strId);
	virtual ~BitmapFontInfo();

	virtual float GetLineHeight() const;
	virtual const CHAR_INFO* GetCharInfo(int ch) const;
	virtual float GetKerning(int char1, int char2) const;

	bool LoadFromFile(const tstring& strFile);

private:
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

};
#endif // __BITMAPFONTINFO_H__
