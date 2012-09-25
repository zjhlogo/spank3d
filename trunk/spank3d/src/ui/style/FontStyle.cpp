/*!
 * \file FontStyle.cpp
 * \date 9-25-2012 21:20:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/FontStyle.h>
#include <Spank3d.h>

FontStyle::FontStyle(const tstring& id)
:IStyle(id)
{
	// TODO: 
}

FontStyle::~FontStyle()
{
	// TODO: 
}

bool FontStyle::Render(const tstring& strText, const Vector2& pos, const Vector2& size, uint state)
{
	// TODO: 
	return true;
}

bool FontStyle::LoadFontFile(const tstring& strFile)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return false;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return false;

	// parse the xml files
	TiXmlElement* pXmlRoot = doc.RootElement();

	TiXmlElement* pXmlCommon = pXmlRoot->FirstChildElement(_("common"));
	if (!ParseCommonInfo(pXmlCommon)) return false;

	TiXmlElement* pXmlPages = pXmlRoot->FirstChildElement(_("pages"));
	if (!CreateTextures(pXmlPages)) return false;

	TiXmlElement* pXmlCharsInfo = pXmlRoot->FirstChildElement(_("chars"));
	if (!CreateCharsInfo(pXmlCharsInfo)) return false;

	TiXmlElement* pXmlKerningsInfo = pXmlRoot->FirstChildElement(_("kernings"));
	CreateKerningsInfo(pXmlKerningsInfo);

	return true;
}

bool FontStyle::ParseCommonInfo(TiXmlElement* pXmlCommon)
{
	if (!pXmlCommon) return false;

	if (!pXmlCommon->Attribute(_("lineHeight"), &m_nLineHeight)) return false;

	return true;
}

bool FontStyle::CreateTextures(TiXmlElement* pXmlPages)
{
	DestroyTextures();

	if (!pXmlPages) return false;

	for (TiXmlElement* pXmlPage = pXmlPages->FirstChildElement(_("page")); pXmlPage != NULL; pXmlPage = pXmlPage->NextSiblingElement(_("page")))
	{
		const tchar* pszTexture = pXmlPage->Attribute(_("file"));
		if (!pszTexture) return false;

		ITexture* pTexture = g_pResMgr->CreateTexture(pszTexture);
		if (!pTexture) return false;

		m_vTextures.push_back(pTexture);
	}

	return true;
}

void FontStyle::DestroyTextures()
{
	for (TV_TEXTURE::iterator it = m_vTextures.begin(); it != m_vTextures.end(); ++it)
	{
		ITexture* pTexture = (*it);
		SAFE_RELEASE(pTexture);
	}
	m_vTextures.clear();
}

bool FontStyle::CreateCharsInfo(TiXmlElement* pXmlCharsInfo)
{
	m_CharInfoMap.clear();
	if (!pXmlCharsInfo) return false;

	for (TiXmlElement* pXmlChar = pXmlCharsInfo->FirstChildElement(_("char")); pXmlChar != NULL; pXmlChar = pXmlChar->NextSiblingElement(_("char")))
	{
		CHAR_INFO charInfo;
		pXmlChar->Attribute(_("id"), &charInfo.id);
		pXmlChar->Attribute(_("x"), &charInfo.x);
		pXmlChar->Attribute(_("y"), &charInfo.y);
		pXmlChar->Attribute(_("width"), &charInfo.width);
		pXmlChar->Attribute(_("height"), &charInfo.height);

		int offsetX = 0;
		int offsetY = 0;
		pXmlChar->Attribute(_("xoffset"), &offsetX);
		pXmlChar->Attribute(_("yoffset"), &offsetY);
		charInfo.offset.x = float(offsetX);
		charInfo.offset.y = float(offsetY);

		int advance = 0;
		pXmlChar->Attribute(_("xadvance"), &advance);
		charInfo.advance = float(advance);

		int pageIndex = 0;
		pXmlChar->Attribute(_("page"), &pageIndex);
		charInfo.pTexture = m_vTextures[pageIndex];

		charInfo.u = charInfo.x / charInfo.pTexture->GetSize().x;
		charInfo.v = charInfo.y / charInfo.pTexture->GetSize().y;
		charInfo.du = charInfo.width / charInfo.pTexture->GetSize().x;
		charInfo.dv = charInfo.height / charInfo.pTexture->GetSize().y;

		m_CharInfoMap.insert(std::make_pair(charInfo.id, charInfo));
	}

	return true;
}

bool FontStyle::CreateKerningsInfo(TiXmlElement* pXmlKerningsInfo)
{
	m_KerningMap.clear();
	if (!pXmlKerningsInfo) return false;

	for (TiXmlElement* pXmlKerning = pXmlKerningsInfo->FirstChildElement(_("kerning")); pXmlKerning != NULL; pXmlKerning = pXmlKerning->NextSiblingElement(_("kerning")))
	{
		int firstId = 0;
		pXmlKerning->Attribute(_("first"), &firstId);

		int secondId = 0;
		pXmlKerning->Attribute(_("second"), &secondId);

		int offset = 0;
		pXmlKerning->Attribute(_("amount"), &offset);

		uint hashKey = ((firstId << 16) | (secondId & 0x0000FFFF));
		m_KerningMap.insert(std::make_pair(hashKey, float(offset)));
	}

	return true;
}
