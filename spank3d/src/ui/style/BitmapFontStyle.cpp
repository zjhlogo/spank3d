/*!
 * \file BitmapFontStyle.cpp
 * \date 9-25-2012 21:20:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/BitmapFontStyle.h>
#include <ui/UiState.h>
#include <util/StringUtil.h>
#include <Spank3d.h>

BitmapFontStyle::BitmapFontStyle(const tstring& id)
:IFontStyle(id)
{

}

BitmapFontStyle::~BitmapFontStyle()
{

}

float BitmapFontStyle::GetLineHeight(uint state) const
{
	const BITMAP_FONT_INFO* pBitmapFontInfo = FindFontInfo(state);
	if (!pBitmapFontInfo) return 0.0f;
	return pBitmapFontInfo->pFontInfo->GetLineHeight();
}

Vector2 BitmapFontStyle::CalcSize(const tstring& strText, uint state)
{
	Vector2 size(0.0f, 0.0f);

	const BITMAP_FONT_INFO* pBitmapFontInfo = FindFontInfo(state);
	if (!pBitmapFontInfo) return size;

	size.y = pBitmapFontInfo->pFontInfo->GetLineHeight();

	tchar lastChar = 0;
	for (tstring::const_iterator it = strText.begin(); it != strText.end(); ++it)
	{
		const tchar& ch = (*it);

		const IFontInfo::CHAR_INFO* pCharInfo = pBitmapFontInfo->pFontInfo->GetCharInfo(ch);
		if (!pCharInfo) continue;

		float kerning = pBitmapFontInfo->pFontInfo->GetKerning(lastChar, ch);

		size.x += (pCharInfo->advance+kerning);
		lastChar = ch;
	}

	return size;
}

bool BitmapFontStyle::Render(const tstring& strText, const Vector2& pos, const Rect& clipRect, uint state)
{
	const BITMAP_FONT_INFO* pBitmapFontInfo = FindFontInfo(state);
	if (!pBitmapFontInfo) return true;
	return RenderText(*pBitmapFontInfo, strText, pos, clipRect);
}

bool BitmapFontStyle::FromXml(TiXmlElement* pXmlBitmapFontStyle)
{
	const tchar* pszFontInfoId = pXmlBitmapFontStyle->Attribute(_("fontInfoId"));
	if (!pszFontInfoId) return false;

	IFontInfo* pFontInfo = g_pUiResMgr->FindFontInfo(pszFontInfoId);
	if (!pFontInfo) return false;

	for (TiXmlElement* pXmlState = pXmlBitmapFontStyle->FirstChildElement(_("State")); pXmlState != NULL; pXmlState = pXmlState->NextSiblingElement(_("State")))
	{
		const tchar* pszId = pXmlState->Attribute(_("id"));
		if (!pszId) continue;

		uint nState = UiState::GetStateValue(pszId);
		if (nState == 0) continue;

		const tchar* pszColor = pXmlState->Attribute(_("color"));
		if (!pszColor) continue;

		BITMAP_FONT_INFO bitmapFontInfo;
		bitmapFontInfo.nState = nState;
		bitmapFontInfo.pFontInfo = pFontInfo;
		StringUtil::strHex2Uint(bitmapFontInfo.color, pszColor);
		m_vBitmapFontInfo.push_back(bitmapFontInfo);
	}

	return true;
}

TiXmlElement* BitmapFontStyle::ToXml()
{
	TiXmlElement* pXmlBitmapFontStyle = new TiXmlElement(_("BitmapFontStyle"));

	pXmlBitmapFontStyle->SetAttribute(_("id"), GetId().c_str());
	pXmlBitmapFontStyle->SetAttribute(_("fontInfoId"), m_vBitmapFontInfo[0].pFontInfo->GetId().c_str());

	for (TV_BITMAP_FONT_INFO::const_iterator it = m_vBitmapFontInfo.begin(); it != m_vBitmapFontInfo.end(); ++it)
	{
		const BITMAP_FONT_INFO& bitmapFontInfo = (*it);

		tstring strState = UiState::GetStateString(bitmapFontInfo.nState);
		tstring strColor;
		StringUtil::hex2str(strColor, bitmapFontInfo.color);

		TiXmlElement* pXmlState = new TiXmlElement(_("State"));
		pXmlState->SetAttribute(_("id"), strState.c_str());
		pXmlState->SetAttribute(_("color"), strColor.c_str());

		pXmlBitmapFontStyle->LinkEndChild(pXmlState);
	}

	return pXmlBitmapFontStyle;
}

const BitmapFontStyle::BITMAP_FONT_INFO* BitmapFontStyle::FindFontInfo(uint state) const
{
	for (TV_BITMAP_FONT_INFO::const_iterator it = m_vBitmapFontInfo.begin(); it != m_vBitmapFontInfo.end(); ++it)
	{
		const BITMAP_FONT_INFO& bitmapFontInfo = (*it);
		if ((bitmapFontInfo.nState & state) != 0)
		{
			return &bitmapFontInfo;
		}
	}

	return NULL;
}

bool BitmapFontStyle::RenderText(const BITMAP_FONT_INFO& bitmapFontInfo, const tstring& strText, const Vector2& pos, const Rect& clipRect)
{
	Vector2 currPos(pos.x, pos.y);

	tchar lastChar = 0;
	for (tstring::const_iterator it = strText.begin(); it != strText.end(); ++it)
	{
		const tchar& ch = (*it);

		const IFontInfo::CHAR_INFO* pCharInfo = bitmapFontInfo.pFontInfo->GetCharInfo(ch);
		if (!pCharInfo) continue;

		float kerning = bitmapFontInfo.pFontInfo->GetKerning(lastChar, ch);

		g_pUiRenderer->DrawRect(currPos.x+pCharInfo->offset.x+kerning, currPos.y+pCharInfo->offset.y, float(pCharInfo->width), float(pCharInfo->height), pCharInfo->u, pCharInfo->v, pCharInfo->du, pCharInfo->dv, bitmapFontInfo.color, clipRect, pCharInfo->pTexture);

		currPos.x += (pCharInfo->advance+kerning);
		lastChar = ch;
	}

	return true;
}
