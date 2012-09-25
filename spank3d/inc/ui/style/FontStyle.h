/*!
 * \file FontStyle.h
 * \date 9-25-2012 21:20:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FONTSTYLE_H__
#define __FONTSTYLE_H__

#include "IStyle.h"
#include "../../render/ITexture.h"
#include <tinyxml-2.6.2/tinyxml.h>
#include <vector>

class FontStyle : public IStyle
{
public:
	typedef std::vector<ITexture*> TV_TEXTURE;

	typedef struct CHAR_INFO_tag
	{
		int id;
		int x;
		int y;
		int width;
		int height;
		float u;
		float v;
		float du;
		float dv;
		ITexture* pTexture;
		Vector2 offset;
		float advance;
	} CHAR_INFO;
	typedef std::map<int, CHAR_INFO> TM_CHAR_INFO;

	typedef std::map<uint, float> TM_UINT_FLOAT;

public:
	RTTI_DEF(FontStyle, IStyle);

	FontStyle(const tstring& id);
	virtual ~FontStyle();

	virtual bool Render(const tstring& strText, const Vector2& pos, const Vector2& size, uint state);
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
#endif // __FONTSTYLE_H__
