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

ushort NinePatchStyle::m_Indis[NinePatchStyle::NUM_INDIS] =
{
	0,  4,  1,  4,  5,  1,  1,  5,  2,  5,  6,  2,  2,  6,  3,  6,  7,  3, 
	4,  8,  5,  8,  9,  5,  5,  9,  6,  9, 10,  6,  6, 10,  7, 10, 11,  7, 
	8, 12,  9, 12, 13,  9,  9, 13, 10, 13, 14, 10, 10, 14, 11, 14, 15, 11
};

NinePatchStyle::NinePatchStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

NinePatchStyle::~NinePatchStyle()
{
	for (TV_NINE_PATCH_INFO::iterator it = m_vNinePatchInfo.begin(); it != m_vNinePatchInfo.end(); ++it)
	{
		NINE_PATCH_INFO *pNinePatchInfo = (*it);
		SAFE_DELETE(pNinePatchInfo);
	}
	m_vNinePatchInfo.clear();
}

bool NinePatchStyle::Render(const Vector2& pos, const Vector2& size, uint state)
{
	for (TV_NINE_PATCH_INFO::iterator it = m_vNinePatchInfo.begin(); it != m_vNinePatchInfo.end(); ++it)
	{
		NINE_PATCH_INFO *pNinePatchInfo = (*it);
		if ((pNinePatchInfo->nState & state) == pNinePatchInfo->nState)
		{
			return RenderNinePatchPiece(*pNinePatchInfo, pos, size);
		}
	}

	return true;
}

bool NinePatchStyle::LoadFromXml(TiXmlElement* pXmlNinePatchStyle)
{
	if (!IGraphicsStyle::LoadFromXml(pXmlNinePatchStyle)) return false;

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

		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;

		pXmlState->Attribute(_("minX"), &minX);
		pXmlState->Attribute(_("minY"), &minY);
		pXmlState->Attribute(_("maxX"), &maxX);
		pXmlState->Attribute(_("maxY"), &maxY);

		NINE_PATCH_INFO* pPatchInfo = new NINE_PATCH_INFO();
		pPatchInfo->nState = nState;
		pPatchInfo->pPieceInfo = pPieceInfo;
		pPatchInfo->pieceSize[0].Reset(float(minX), float(minY));
		pPatchInfo->pieceSize[1].Reset(float(maxX-minX), float(maxY-minY));
		pPatchInfo->pieceSize[2].Reset(float(pPieceInfo->width-maxX), float(pPieceInfo->height-maxY));

		float u[4];
		float v[4];

		u[0] = pPieceInfo->u;
		v[0] = pPieceInfo->v;
		u[1] = u[0]+pPieceInfo->du*(pPatchInfo->pieceSize[0].x/pPieceInfo->width);
		v[1] = v[0]+pPieceInfo->dv*(pPatchInfo->pieceSize[0].y/pPieceInfo->height);
		u[2] = u[1]+pPieceInfo->du*(pPatchInfo->pieceSize[1].x/pPieceInfo->width);
		v[2] = v[1]+pPieceInfo->dv*(pPatchInfo->pieceSize[1].y/pPieceInfo->height);
		u[3] = u[2]+pPieceInfo->du*(pPatchInfo->pieceSize[2].x/pPieceInfo->width);
		v[3] = v[2]+pPieceInfo->dv*(pPatchInfo->pieceSize[2].y/pPieceInfo->height);

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

bool NinePatchStyle::RenderNinePatchPiece(NINE_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size)
{
	float px[4];
	float py[4];

	px[0] = pos.x;
	py[0] = pos.y;
	px[1] = px[0]+patchInfo.pieceSize[0].x;
	py[1] = py[0]+patchInfo.pieceSize[0].y;
	px[2] = px[1]+size.x-patchInfo.pieceSize[2].x;
	py[2] = py[1]+size.y-patchInfo.pieceSize[2].y;
	px[3] = px[2]+patchInfo.pieceSize[2].x;
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

	g_pRendererUi->DrawTriangleList(patchInfo.verts, NUM_VERTS, m_Indis, NUM_INDIS, patchInfo.pPieceInfo->pTexture);
	return false;
}
