/*!
 * \file HorizontalPatchStyle.cpp
 * \date 9-20-2012 14:20:59
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/HorizontalPatchStyle.h>
#include <ui/UiState.h>
#include <Spank3d.h>

HorizontalPatchStyle::HorizontalPatchStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

HorizontalPatchStyle::~HorizontalPatchStyle()
{
	for (TV_HORIZONTAL_PATCH_INFO::iterator it = m_vHorizontalPatchInfo.begin(); it != m_vHorizontalPatchInfo.end(); ++it)
	{
		HORIZONTAL_PATCH_INFO *pHorizontalPatchInfo = (*it);
		SAFE_DELETE(pHorizontalPatchInfo);
	}
	m_vHorizontalPatchInfo.clear();
}

bool HorizontalPatchStyle::Render(const Vector2& pos, const Vector2& size, uint state)
{
	for (TV_HORIZONTAL_PATCH_INFO::iterator it = m_vHorizontalPatchInfo.begin(); it != m_vHorizontalPatchInfo.end(); ++it)
	{
		HORIZONTAL_PATCH_INFO *pHorizontalPatchInfo = (*it);
		if ((pHorizontalPatchInfo->nState & state) == pHorizontalPatchInfo->nState)
		{
			return RenderHorizontalPatchPiece(*pHorizontalPatchInfo, pos, size);
		}
	}

	return true;
}

bool HorizontalPatchStyle::LoadFromXml(TiXmlElement* pXmlHorizontalPatchStyle)
{
	if (!IGraphicsStyle::LoadFromXml(pXmlHorizontalPatchStyle)) return false;

	for (TiXmlElement* pXmlState = pXmlHorizontalPatchStyle->FirstChildElement(_("State")); pXmlState != NULL; pXmlState = pXmlState->NextSiblingElement(_("State")))
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
		int maxX = 0;

		pXmlState->Attribute(_("minX"), &minX);
		pXmlState->Attribute(_("maxX"), &maxX);

		HORIZONTAL_PATCH_INFO* pPatchInfo = new HORIZONTAL_PATCH_INFO();
		pPatchInfo->nState = nState;
		pPatchInfo->pPieceInfo = pPieceInfo;
		pPatchInfo->pieceWidths[0] = float(minX);
		pPatchInfo->pieceWidths[1] = float(maxX-minX);
		pPatchInfo->pieceWidths[2] = float(pPieceInfo->width-maxX);

		float u[4];
		u[0] = pPieceInfo->u;
		u[1] = u[0]+pPieceInfo->du*(pPatchInfo->pieceWidths[0]/pPieceInfo->width);
		u[2] = u[1]+pPieceInfo->du*(pPatchInfo->pieceWidths[1]/pPieceInfo->width);
		u[3] = u[0]+pPieceInfo->du;

		float v[2];
		v[0] = pPieceInfo->v;
		v[1] = v[0]+pPieceInfo->dv;
		v[0] = 1.0f - v[0];
		v[1] = 1.0f - v[1];

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

		m_vHorizontalPatchInfo.push_back(pPatchInfo);
	}

	return true;
}

bool HorizontalPatchStyle::RenderHorizontalPatchPiece(HORIZONTAL_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size)
{
	static ushort s_Indis[NUM_INDIS] = {0, 4, 1, 4, 5, 1, 1, 5, 2, 5, 6, 2, 2, 6, 3, 6, 7, 3};

	float px[4];
	px[0] = pos.x;
	px[1] = px[0]+patchInfo.pieceWidths[0];
	px[2] = px[1]+(size.x-patchInfo.pieceWidths[0]-patchInfo.pieceWidths[2]);
	px[3] = px[2]+patchInfo.pieceWidths[2];

	float py[2];
	py[0] = pos.y;
	py[1] = py[0]+size.y;

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

	g_pUiRenderer->DrawTriangleList(patchInfo.verts, NUM_VERTS, s_Indis, NUM_INDIS, patchInfo.pPieceInfo->pTexture);
	return false;
}
