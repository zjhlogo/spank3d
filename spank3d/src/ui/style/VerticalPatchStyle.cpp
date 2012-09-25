/*!
 * \file VerticalPatchStyle.cpp
 * \date 9-20-2012 14:19:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/VerticalPatchStyle.h>
#include <ui/UiState.h>
#include <Spank3d.h>

VerticalPatchStyle::VerticalPatchStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

VerticalPatchStyle::~VerticalPatchStyle()
{
	for (TV_VERTICAL_PATCH_INFO::iterator it = m_vVerticalPatchInfo.begin(); it != m_vVerticalPatchInfo.end(); ++it)
	{
		VERTICAL_PATCH_INFO *pVerticalPatchInfo = (*it);
		SAFE_DELETE(pVerticalPatchInfo);
	}
	m_vVerticalPatchInfo.clear();
}

bool VerticalPatchStyle::Render(const Vector2& pos, const Vector2& size, uint state)
{
	for (TV_VERTICAL_PATCH_INFO::iterator it = m_vVerticalPatchInfo.begin(); it != m_vVerticalPatchInfo.end(); ++it)
	{
		VERTICAL_PATCH_INFO *pVerticalPatchInfo = (*it);
		if ((pVerticalPatchInfo->nState & state) == pVerticalPatchInfo->nState)
		{
			return RenderVerticalPatchPiece(*pVerticalPatchInfo, pos, size);
		}
	}

	return true;
}

bool VerticalPatchStyle::LoadFromXml(TiXmlElement* pXmlVerticalPatchStyle)
{
	if (!IGraphicsStyle::LoadFromXml(pXmlVerticalPatchStyle)) return false;

	for (TiXmlElement* pXmlState = pXmlVerticalPatchStyle->FirstChildElement(_("State")); pXmlState != NULL; pXmlState = pXmlState->NextSiblingElement(_("State")))
	{
		const tchar* pszId = pXmlState->Attribute(_("id"));
		if (!pszId) continue;

		uint nState = UiState::GetStateValue(pszId);
		if (nState == 0) continue;

		const tchar* pszPieceId = pXmlState->Attribute(_("piece"));
		if (!pszPieceId) continue;

		const PieceInfo* pPieceInfo = g_pUiResMgr->FindPieceInfo(pszPieceId);
		if (!pPieceInfo) continue;

		int minY = 0;
		int maxY = 0;

		pXmlState->Attribute(_("minY"), &minY);
		pXmlState->Attribute(_("maxY"), &maxY);

		VERTICAL_PATCH_INFO* pPatchInfo = new VERTICAL_PATCH_INFO();
		pPatchInfo->nState = nState;
		pPatchInfo->pPieceInfo = pPieceInfo;
		pPatchInfo->pieceHeights[0] = float(minY);
		pPatchInfo->pieceHeights[1] = float(maxY-minY);
		pPatchInfo->pieceHeights[2] = float(pPieceInfo->height-maxY);

		float u[2];
		u[0] = pPieceInfo->u;
		u[1] = u[0]+pPieceInfo->du;

		float v[4];
		v[0] = pPieceInfo->v;
		v[1] = v[0]+pPieceInfo->dv*(pPatchInfo->pieceHeights[0]/pPieceInfo->height);
		v[2] = v[1]+pPieceInfo->dv*(pPatchInfo->pieceHeights[1]/pPieceInfo->height);
		v[3] = v[0]+pPieceInfo->dv;
		v[0] = 1.0f - v[0];
		v[1] = 1.0f - v[1];
		v[2] = 1.0f - v[2];
		v[3] = 1.0f - v[3];

		pPatchInfo->verts[0].u = u[0];
		pPatchInfo->verts[0].v = v[0];
		pPatchInfo->verts[1].u = u[1];
		pPatchInfo->verts[1].v = v[0];

		pPatchInfo->verts[2].u = u[0];
		pPatchInfo->verts[2].v = v[1];
		pPatchInfo->verts[3].u = u[1];
		pPatchInfo->verts[3].v = v[1];

		pPatchInfo->verts[4].u = u[0];
		pPatchInfo->verts[4].v = v[2];
		pPatchInfo->verts[5].u = u[1];
		pPatchInfo->verts[5].v = v[2];

		pPatchInfo->verts[6].u = u[0];
		pPatchInfo->verts[6].v = v[3];
		pPatchInfo->verts[7].u = u[1];
		pPatchInfo->verts[7].v = v[3];

		m_vVerticalPatchInfo.push_back(pPatchInfo);
	}

	return true;
}

bool VerticalPatchStyle::RenderVerticalPatchPiece(VERTICAL_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size)
{
	static ushort s_Indis[NUM_INDIS] = {0, 2, 1, 2, 3, 1, 2, 4, 3, 4, 5, 3, 4, 6, 5, 6, 7, 5};

	float px[4];
	px[0] = pos.x;
	px[1] = px[0]+size.x;

	float py[4];
	py[0] = pos.y;
	py[1] = py[0]+patchInfo.pieceHeights[0];
	py[2] = py[1]+(size.y-patchInfo.pieceHeights[0]-patchInfo.pieceHeights[2]);
	py[3] = py[2]+patchInfo.pieceHeights[2];

	patchInfo.verts[0].x = px[0];
	patchInfo.verts[0].y = py[0];
	patchInfo.verts[1].x = px[1];
	patchInfo.verts[1].y = py[0];

	patchInfo.verts[2].x = px[0];
	patchInfo.verts[2].y = py[1];
	patchInfo.verts[3].x = px[1];
	patchInfo.verts[3].y = py[1];

	patchInfo.verts[4].x = px[0];
	patchInfo.verts[4].y = py[2];
	patchInfo.verts[5].x = px[1];
	patchInfo.verts[5].y = py[2];

	patchInfo.verts[6].x = px[0];
	patchInfo.verts[6].y = py[3];
	patchInfo.verts[7].x = px[1];
	patchInfo.verts[7].y = py[3];

	g_pRendererUi->DrawTriangleList(patchInfo.verts, NUM_VERTS, s_Indis, NUM_INDIS, patchInfo.pPieceInfo->pTexture);
	return false;
}
