/*!
 * \file NinePatchStyle.cpp
 * \date 9-20-2012 9:00:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/NinePatchStyle.h>
#include <ui/UiState.h>
#include <Spank3d.h>

NinePatchStyle::NinePatchStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

NinePatchStyle::~NinePatchStyle()
{
	// TODO: 
}

bool NinePatchStyle::Render(const Vector2& pos, const Vector2& size, uint state)
{
	for (TV_NINE_PATCH_PIECE_INFO::const_iterator it = m_vNinePatchPieceInfo.begin(); it != m_vNinePatchPieceInfo.end(); ++it)
	{
		const NINE_PATCH_PIECE_INFO& stateInfo = (*it);
		if ((stateInfo.nState & state) == stateInfo.nState)
		{
			return RenderNinePatchPiece(stateInfo, pos, size);
		}
	}

	return true;
}

bool NinePatchStyle::LoadFromXml(TiXmlElement* pXmlNinePatchStyle)
{
	int minW = 0;
	int minH = 0;
	if (!pXmlNinePatchStyle->Attribute(_("minW"), &minW) || !pXmlNinePatchStyle->Attribute(_("minH"), &minH)) return false;
	SetMinSize(float(minW), float(minH));

	int maxW = 0;
	int maxH = 0;
	if (pXmlNinePatchStyle->Attribute(_("maxW"), &maxW) && pXmlNinePatchStyle->Attribute(_("maxH"), &maxH))
	{
		SetMaxSize(float(maxW), float(maxH));
	}
	else
	{
		SetMaxSize(float(INT_MAX), float(INT_MAX));
	}

	int bestW = 0;
	int bestH = 0;
	if (pXmlNinePatchStyle->Attribute(_("bestW"), &bestW) && pXmlNinePatchStyle->Attribute(_("bestH"), &bestH))
	{
		SetBestSize(float(bestW), float(bestH));
	}
	else
	{
		SetBestSize(float(minW), float(minH));
	}

	int paddingL = 0;
	int paddingT = 0;
	int paddingR = 0;
	int paddingB = 0;
	pXmlNinePatchStyle->Attribute(_("paddingL"), &paddingL);
	pXmlNinePatchStyle->Attribute(_("paddingT"), &paddingT);
	pXmlNinePatchStyle->Attribute(_("paddingR"), &paddingR);
	pXmlNinePatchStyle->Attribute(_("paddingB"), &paddingB);
	SetPaddingLT(float(paddingL), float(paddingT));
	SetPaddingLT(float(paddingR), float(paddingB));

	NINE_PATCH_PIECE_INFO stateInfo;
	for (TiXmlElement* pXmlState = pXmlNinePatchStyle->FirstChildElement(_("State")); pXmlState != NULL; pXmlState = pXmlState->NextSiblingElement(_("State")))
	{
		const char* pszId = pXmlState->Attribute(_("id"));
		if (!pszId) continue;

		stateInfo.nState = UiState::GetStateValue(pszId);
		if (stateInfo.nState == 0) continue;

		const char* pszPieceId = pXmlState->Attribute(_("piece"));
		if (!pszPieceId) continue;

		stateInfo.pPieceInfo = g_pUiResMgr->FindPieceInfo(pszPieceId);
		if (!stateInfo.pPieceInfo) continue;

		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;

		pXmlState->Attribute(_("minX"), &minX);
		pXmlState->Attribute(_("minY"), &minY);
		pXmlState->Attribute(_("maxX"), &maxX);
		pXmlState->Attribute(_("maxY"), &maxY);

		float pieceWidth = float(stateInfo.pPieceInfo->width);
		float pieceHeight = float(stateInfo.pPieceInfo->height);

		stateInfo.patchSize[0].Reset(float(minX), float(minY));
		stateInfo.patchSize[1].Reset(float(maxX-minX), float(maxY-minY));
		stateInfo.patchSize[2].Reset(float(pieceWidth-maxX), float(pieceHeight-maxY));

		float du = stateInfo.pPieceInfo->du;
		float dv = stateInfo.pPieceInfo->dv;

		stateInfo.uv[0].Reset(stateInfo.pPieceInfo->u, stateInfo.pPieceInfo->v);
		stateInfo.dudv[0].Reset(du*(stateInfo.patchSize[0].x/pieceWidth), dv*(stateInfo.patchSize[0].y/pieceHeight));

		stateInfo.uv[1].Reset(stateInfo.uv[0].x+stateInfo.dudv[0].x, stateInfo.uv[0].y+stateInfo.dudv[0].y);
		stateInfo.dudv[1].Reset(du*(stateInfo.patchSize[1].x/pieceWidth), dv*(stateInfo.patchSize[1].y/pieceHeight));

		stateInfo.uv[2].Reset(stateInfo.uv[1].x+stateInfo.dudv[1].x, stateInfo.uv[1].y+stateInfo.dudv[1].y);
		stateInfo.dudv[2].Reset(du*(stateInfo.patchSize[2].x/pieceWidth), du*(stateInfo.patchSize[2].y/pieceHeight));

		m_vNinePatchPieceInfo.push_back(stateInfo);
	}

	return true;
}

bool NinePatchStyle::RenderNinePatchPiece(const NINE_PATCH_PIECE_INFO& stateInfo, const Vector2& pos, const Vector2& size)
{
	Vector2 size0(stateInfo.patchSize[0].x, stateInfo.patchSize[0].y);
	Vector2 size1(size.x-stateInfo.patchSize[0].x-stateInfo.patchSize[2].x, size.y-stateInfo.patchSize[0].y-stateInfo.patchSize[2].y);
	Vector2 size2(stateInfo.patchSize[2].x, stateInfo.patchSize[2].y);

	g_pRendererUi->DrawRect(pos.x,					pos.y,					size0.x, size0.y, stateInfo.uv[0].x, stateInfo.uv[0].y, stateInfo.dudv[0].x, stateInfo.dudv[0].y, stateInfo.pPieceInfo->pTexture);
	g_pRendererUi->DrawRect(pos.x+size0.x,			pos.y,					size1.x, size0.y, stateInfo.uv[1].x, stateInfo.uv[0].y, stateInfo.dudv[1].x, stateInfo.dudv[0].y, stateInfo.pPieceInfo->pTexture);
	g_pRendererUi->DrawRect(pos.x+size0.x+size1.x,	pos.y,					size2.x, size0.y, stateInfo.uv[2].x, stateInfo.uv[0].y, stateInfo.dudv[2].x, stateInfo.dudv[0].y, stateInfo.pPieceInfo->pTexture);

	g_pRendererUi->DrawRect(pos.x,					pos.y+size0.y,			size0.x, size1.y, stateInfo.uv[0].x, stateInfo.uv[1].y, stateInfo.dudv[0].x, stateInfo.dudv[1].y, stateInfo.pPieceInfo->pTexture);
	g_pRendererUi->DrawRect(pos.x+size0.x,			pos.y+size0.y,			size1.x, size1.y, stateInfo.uv[1].x, stateInfo.uv[1].y, stateInfo.dudv[1].x, stateInfo.dudv[1].y, stateInfo.pPieceInfo->pTexture);
	g_pRendererUi->DrawRect(pos.x+size0.x+size1.x,	pos.y+size0.y,			size2.x, size1.y, stateInfo.uv[2].x, stateInfo.uv[1].y, stateInfo.dudv[2].x, stateInfo.dudv[1].y, stateInfo.pPieceInfo->pTexture);

	g_pRendererUi->DrawRect(pos.x,					pos.y+size0.y+size1.y,	size0.x, size2.y, stateInfo.uv[0].x, stateInfo.uv[2].y, stateInfo.dudv[0].x, stateInfo.dudv[2].y, stateInfo.pPieceInfo->pTexture);
	g_pRendererUi->DrawRect(pos.x+size0.x,			pos.y+size0.y+size1.y,	size1.x, size2.y, stateInfo.uv[1].x, stateInfo.uv[2].y, stateInfo.dudv[1].x, stateInfo.dudv[2].y, stateInfo.pPieceInfo->pTexture);
	g_pRendererUi->DrawRect(pos.x+size0.x+size1.x,	pos.y+size0.y+size1.y,	size2.x, size2.y, stateInfo.uv[2].x, stateInfo.uv[2].y, stateInfo.dudv[2].x, stateInfo.dudv[2].y, stateInfo.pPieceInfo->pTexture);

	return false;
}
