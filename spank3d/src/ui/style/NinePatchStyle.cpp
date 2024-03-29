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
	for (TV_NINE_PATCH_INFO::iterator it = m_vNinePatchInfo.begin(); it != m_vNinePatchInfo.end(); ++it)
	{
		NINE_PATCH_INFO* pNinePatchInfo = (*it);
		SAFE_DELETE(pNinePatchInfo);
	}
	m_vNinePatchInfo.clear();
}

bool NinePatchStyle::Render(const Vector2& pos, const Vector2& size, const Rect& clipRect, uint state)
{
	for (TV_NINE_PATCH_INFO::iterator it = m_vNinePatchInfo.begin(); it != m_vNinePatchInfo.end(); ++it)
	{
		NINE_PATCH_INFO* pNinePatchInfo = (*it);
		if ((pNinePatchInfo->nState & state) != 0)
		{
			return RenderNinePatchPiece(*pNinePatchInfo, pos, size, clipRect);
		}
	}

	return true;
}

bool NinePatchStyle::FromXml(TiXmlElement* pXmlNinePatchStyle)
{
	if (!IGraphicsStyle::FromXml(pXmlNinePatchStyle)) return false;

	for (TiXmlElement* pXmlState = pXmlNinePatchStyle->FirstChildElement(_("State")); pXmlState != NULL; pXmlState = pXmlState->NextSiblingElement(_("State")))
	{
		const tchar* pszId = pXmlState->Attribute(_("id"));
		if (!pszId) continue;

		uint nState = UiState::GetStateValue(pszId);
		if (nState == 0) continue;

		const tchar* pszPieceId = pXmlState->Attribute(_("piece"));
		if (!pszPieceId) continue;

		const PieceInfo* pPieceInfo = g_pUiResMgr->FindPieceInfo(pszPieceId);
		if (!pPieceInfo) continue;

		float minX = 0.0f;
		float minY = 0.0f;
		float maxX = 0.0f;
		float maxY = 0.0f;
		pXmlState->QueryFloatAttribute(_("minX"), &minX);
		pXmlState->QueryFloatAttribute(_("minY"), &minY);
		pXmlState->QueryFloatAttribute(_("maxX"), &maxX);
		pXmlState->QueryFloatAttribute(_("maxY"), &maxY);

		NINE_PATCH_INFO* pPatchInfo = new NINE_PATCH_INFO();
		pPatchInfo->nState = nState;
		pPatchInfo->pPieceInfo = pPieceInfo;
		pPatchInfo->pieceSize[0].Reset(minX, minY);
		pPatchInfo->pieceSize[1].Reset(maxX-minX, maxY-minY);
		pPatchInfo->pieceSize[2].Reset(pPieceInfo->width-maxX, pPieceInfo->height-maxY);

		float u[4];
		u[0] = pPieceInfo->u;
		u[1] = u[0]+pPieceInfo->du*(pPatchInfo->pieceSize[0].x/pPieceInfo->width);
		u[2] = u[1]+pPieceInfo->du*(pPatchInfo->pieceSize[1].x/pPieceInfo->width);
		u[3] = u[0]+pPieceInfo->du;

		float v[4];
		v[0] = pPieceInfo->v;
		v[1] = v[0]+pPieceInfo->dv*(pPatchInfo->pieceSize[0].y/pPieceInfo->height);
		v[2] = v[1]+pPieceInfo->dv*(pPatchInfo->pieceSize[1].y/pPieceInfo->height);
		v[3] = v[0]+pPieceInfo->dv;
		v[0] = 1.0f - v[0];
		v[1] = 1.0f - v[1];
		v[2] = 1.0f - v[2];
		v[3] = 1.0f - v[3];

		pPatchInfo->verts[0].u = u[0];
		pPatchInfo->verts[0].v = v[0];
		pPatchInfo->verts[1].u = u[1];
		pPatchInfo->verts[1].v = v[0];
		pPatchInfo->verts[2].u = u[2];
		pPatchInfo->verts[2].v = v[0];
		pPatchInfo->verts[3].u = u[3];
		pPatchInfo->verts[3].v = v[0];

		pPatchInfo->verts[4].u = u[0];
		pPatchInfo->verts[4].v = v[1];
		pPatchInfo->verts[5].u = u[1];
		pPatchInfo->verts[5].v = v[1];
		pPatchInfo->verts[6].u = u[2];
		pPatchInfo->verts[6].v = v[1];
		pPatchInfo->verts[7].u = u[3];
		pPatchInfo->verts[7].v = v[1];

		pPatchInfo->verts[8].u = u[0];
		pPatchInfo->verts[8].v = v[2];
		pPatchInfo->verts[9].u = u[1];
		pPatchInfo->verts[9].v = v[2];
		pPatchInfo->verts[10].u = u[2];
		pPatchInfo->verts[10].v = v[2];
		pPatchInfo->verts[11].u = u[3];
		pPatchInfo->verts[11].v = v[2];

		pPatchInfo->verts[12].u = u[0];
		pPatchInfo->verts[12].v = v[3];
		pPatchInfo->verts[13].u = u[1];
		pPatchInfo->verts[13].v = v[3];
		pPatchInfo->verts[14].u = u[2];
		pPatchInfo->verts[14].v = v[3];
		pPatchInfo->verts[15].u = u[3];
		pPatchInfo->verts[15].v = v[3];

		m_vNinePatchInfo.push_back(pPatchInfo);
	}

	return true;
}

TiXmlElement* NinePatchStyle::ToXml()
{
	TiXmlElement* pXmlNinePatchStyle = new TiXmlElement(_("NinePatchStyle"));
	if (!IGraphicsStyle::ToXml(pXmlNinePatchStyle))
	{
		SAFE_DELETE(pXmlNinePatchStyle);
		return NULL;
	}

	for (TV_NINE_PATCH_INFO::const_iterator it = m_vNinePatchInfo.begin(); it != m_vNinePatchInfo.end(); ++it)
	{
		const NINE_PATCH_INFO* pNinePatchInfo = (*it);

		tstring strState = UiState::GetStateString(pNinePatchInfo->nState);
		TiXmlElement* pXmlState = new TiXmlElement(_("State"));
		pXmlState->SetAttribute(_("id"), strState.c_str());
		pXmlState->SetAttribute(_("piece"), pNinePatchInfo->pPieceInfo->GetId().c_str());
		pXmlState->SetAttribute(_("minX"), int(pNinePatchInfo->pieceSize[0].x));
		pXmlState->SetAttribute(_("minY"), int(pNinePatchInfo->pieceSize[0].y));
		pXmlState->SetAttribute(_("maxX"), int(pNinePatchInfo->pieceSize[0].x+pNinePatchInfo->pieceSize[1].x));
		pXmlState->SetAttribute(_("maxY"), int(pNinePatchInfo->pieceSize[0].y+pNinePatchInfo->pieceSize[1].y));

		pXmlNinePatchStyle->LinkEndChild(pXmlState);
	}

	return pXmlNinePatchStyle;
}

bool NinePatchStyle::RenderNinePatchPiece(NINE_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size, const Rect& clipRect)
{
	static ushort s_Indis[NUM_INDIS] =
	{
		0,  4,  1,  4,  5,  1,  1,  5,  2,  5,  6,  2,  2,  6,  3,  6,  7,  3, 
		4,  8,  5,  8,  9,  5,  5,  9,  6,  9, 10,  6,  6, 10,  7, 10, 11,  7, 
		8, 12,  9, 12, 13,  9,  9, 13, 10, 13, 14, 10, 10, 14, 11, 14, 15, 11
	};

	float px[4];
	px[0] = pos.x;
	px[1] = px[0]+patchInfo.pieceSize[0].x;
	px[2] = px[1]+(size.x-patchInfo.pieceSize[0].x-patchInfo.pieceSize[2].x);
	px[3] = px[2]+patchInfo.pieceSize[2].x;

	float py[4];
	py[0] = pos.y;
	py[1] = py[0]+patchInfo.pieceSize[0].y;
	py[2] = py[1]+(size.y-patchInfo.pieceSize[0].y-patchInfo.pieceSize[2].y);
	py[3] = py[2]+patchInfo.pieceSize[2].y;

	patchInfo.verts[0].x = px[0];
	patchInfo.verts[0].y = py[0];
	patchInfo.verts[1].x = px[1];
	patchInfo.verts[1].y = py[0];
	patchInfo.verts[2].x = px[2];
	patchInfo.verts[2].y = py[0];
	patchInfo.verts[3].x = px[3];
	patchInfo.verts[3].y = py[0];

	patchInfo.verts[4].x = px[0];
	patchInfo.verts[4].y = py[1];
	patchInfo.verts[5].x = px[1];
	patchInfo.verts[5].y = py[1];
	patchInfo.verts[6].x = px[2];
	patchInfo.verts[6].y = py[1];
	patchInfo.verts[7].x = px[3];
	patchInfo.verts[7].y = py[1];

	patchInfo.verts[8].x = px[0];
	patchInfo.verts[8].y = py[2];
	patchInfo.verts[9].x = px[1];
	patchInfo.verts[9].y = py[2];
	patchInfo.verts[10].x = px[2];
	patchInfo.verts[10].y = py[2];
	patchInfo.verts[11].x = px[3];
	patchInfo.verts[11].y = py[2];

	patchInfo.verts[12].x = px[0];
	patchInfo.verts[12].y = py[3];
	patchInfo.verts[13].x = px[1];
	patchInfo.verts[13].y = py[3];
	patchInfo.verts[14].x = px[2];
	patchInfo.verts[14].y = py[3];
	patchInfo.verts[15].x = px[3];
	patchInfo.verts[15].y = py[3];

	g_pUiRenderer->DrawTriangleList(patchInfo.verts, NUM_VERTS, s_Indis, NUM_INDIS, 0xFFFFFFFF, clipRect, patchInfo.pPieceInfo->pTexture);
	return false;
}
