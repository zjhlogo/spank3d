/*!
 * \file BitmapFontInfo.cpp
 * \date 10-20-2012 9:10:59
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/BitmapFontInfo.h>
#include <Spank3d.h>

BitmapFontInfo::BitmapFontInfo(const tstring& strId)
:IFontInfo(strId)
{
	m_fLineHeight = 0.0f;
	m_nNumPages = 0;
}

BitmapFontInfo::~BitmapFontInfo()
{
	DestroyTextures();
	m_CharInfoMap.clear();
	m_KerningMap.clear();
}

float BitmapFontInfo::GetLineHeight() const
{
	return m_fLineHeight;
}

const IFontInfo::CHAR_INFO* BitmapFontInfo::GetCharInfo(int ch) const
{
	TM_CHAR_INFO::const_iterator itChar = m_CharInfoMap.find(ch);
	if (itChar == m_CharInfoMap.end()) return NULL;
	const CHAR_INFO& charInfo = itChar->second;
	return &charInfo;
}

float BitmapFontInfo::GetKerning(int char1, int char2) const
{
	float kerning = 0.0f;
	uint hashKey = ((uint(char1) << 16) | (uint(char2) & 0x0000FFFF));
	TM_UINT_FLOAT::const_iterator itKerning = m_KerningMap.find(hashKey);
	if (itKerning != m_KerningMap.end()) kerning = itKerning->second;
	return kerning;
}

bool BitmapFontInfo::LoadFromFile(const tstring& strFile)
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

	SetOk(true);
	return true;
}

bool BitmapFontInfo::ParseCommonInfo(TiXmlElement* pXmlCommon)
{
	if (!pXmlCommon) return false;
	if (!pXmlCommon->Attribute(_("lineHeight"), &m_fLineHeight)) return false;
	return true;
}

bool BitmapFontInfo::CreateTextures(TiXmlElement* pXmlPages)
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

void BitmapFontInfo::DestroyTextures()
{
	for (TV_TEXTURE::iterator it = m_vTextures.begin(); it != m_vTextures.end(); ++it)
	{
		ITexture* pTexture = (*it);
		SAFE_RELEASE(pTexture);
	}
	m_vTextures.clear();
}

bool BitmapFontInfo::CreateCharsInfo(TiXmlElement* pXmlCharsInfo)
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

		pXmlChar->Attribute(_("xoffset"), &charInfo.offset.x);
		pXmlChar->Attribute(_("yoffset"), &charInfo.offset.y);

		pXmlChar->Attribute(_("xadvance"), &charInfo.advance);

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

bool BitmapFontInfo::CreateKerningsInfo(TiXmlElement* pXmlKerningsInfo)
{
	m_KerningMap.clear();
	if (!pXmlKerningsInfo) return false;

	for (TiXmlElement* pXmlKerning = pXmlKerningsInfo->FirstChildElement(_("kerning")); pXmlKerning != NULL; pXmlKerning = pXmlKerning->NextSiblingElement(_("kerning")))
	{
		int firstId = 0;
		pXmlKerning->Attribute(_("first"), &firstId);

		int secondId = 0;
		pXmlKerning->Attribute(_("second"), &secondId);

		float offset = 0.0f;
		pXmlKerning->Attribute(_("amount"), &offset);

		uint hashKey = ((firstId << 16) | (secondId & 0x0000FFFF));
		m_KerningMap.insert(std::make_pair(hashKey, offset));
	}

	return true;
}
