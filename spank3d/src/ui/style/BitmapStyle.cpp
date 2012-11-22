/*!
 * \file BitmapStyle.cpp
 * \date 9-20-2012 8:46:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/BitmapStyle.h>
#include <ui/UiState.h>
#include <Spank3d.h>

BitmapStyle::BitmapStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

BitmapStyle::~BitmapStyle()
{
	for (TV_BITMAP_INFO::iterator it = m_vBitmapInfo.begin(); it != m_vBitmapInfo.end(); ++it)
	{
		BITMAP_INFO* pBitmapInfo = (*it);
		SAFE_DELETE(pBitmapInfo);
	}

	m_vBitmapInfo.clear();
}

bool BitmapStyle::Render(const Vector2& pos, const Vector2& size, const Rect& clipRect, uint state)
{
	for (TV_BITMAP_INFO::iterator it = m_vBitmapInfo.begin(); it != m_vBitmapInfo.end(); ++it)
	{
		BITMAP_INFO* pBitmapInfo = (*it);
		if ((pBitmapInfo->nState & state) != 0)
		{
			return RenderBitmapPiece(*pBitmapInfo, pos, size, clipRect);
		}
	}

	return true;
}

bool BitmapStyle::FromXml(TiXmlElement* pXmlBitmapStyle)
{
	if (!IGraphicsStyle::FromXml(pXmlBitmapStyle)) return false;

	for (TiXmlElement* pXmlState = pXmlBitmapStyle->FirstChildElement(_("State")); pXmlState != NULL; pXmlState = pXmlState->NextSiblingElement(_("State")))
	{
		const tchar* pszId = pXmlState->Attribute(_("id"));
		if (!pszId) continue;

		uint nState = UiState::GetStateValue(pszId);
		if (nState == 0) continue;

		const tchar* pszPieceId = pXmlState->Attribute(_("piece"));
		if (!pszPieceId) continue;

		const PieceInfo* pPieceInfo = g_pUiResMgr->FindPieceInfo(pszPieceId);
		if (!pPieceInfo) continue;

		BITMAP_INFO* pBitmapInfo = new BITMAP_INFO();
		pBitmapInfo->nState = nState;
		pBitmapInfo->pPieceInfo = pPieceInfo;

		m_vBitmapInfo.push_back(pBitmapInfo);
	}

	return true;
}

TiXmlElement* BitmapStyle::ToXml()
{
	TiXmlElement* pXmlBitmapStyle = new TiXmlElement(_("BitmapStyle"));
	if (!IGraphicsStyle::ToXml(pXmlBitmapStyle))
	{
		SAFE_DELETE(pXmlBitmapStyle);
		return NULL;
	}

	for (TV_BITMAP_INFO::const_iterator it = m_vBitmapInfo.begin(); it != m_vBitmapInfo.end(); ++it)
	{
		const BITMAP_INFO* pBitmapInfo = (*it);

		tstring strState = UiState::GetStateString(pBitmapInfo->nState);
		TiXmlElement* pXmlState = new TiXmlElement(_("State"));
		pXmlState->SetAttribute(_("id"), strState.c_str());
		pXmlState->SetAttribute(_("piece"), pBitmapInfo->pPieceInfo->GetId().c_str());

		pXmlBitmapStyle->LinkEndChild(pXmlState);
	}

	return pXmlBitmapStyle;
}

bool BitmapStyle::RenderBitmapPiece(const BITMAP_INFO& bitmapInfo, const Vector2& pos, const Vector2& size, const Rect& clipRect)
{
	g_pUiRenderer->DrawRect(pos, size, 0xFFFFFFFF, clipRect, bitmapInfo.pPieceInfo);
	return true;
}
