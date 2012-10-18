/*!
 * \file UiRenderer_Impl.cpp
 * \date 10-10-2012 9:26:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiRenderer_Impl.h"
#include <util/LogUtil.h>
#include <util/SystemUtil.h>
#include <Spank3d.h>

UiRenderer_Impl::UiRenderer_Impl()
{
	SystemUtil::ZeroMemory(m_pVertexCaches, sizeof(m_pVertexCaches));
	m_pShader = NULL;

	g_pUiRenderer = this;
}

UiRenderer_Impl::~UiRenderer_Impl()
{
	g_pUiRenderer = NULL;
}

UiRenderer_Impl& UiRenderer_Impl::GetInstance()
{
	static UiRenderer_Impl s_RendererUi_Impl;
	return s_RendererUi_Impl;
}

bool UiRenderer_Impl::Initialize()
{
	m_pShader = g_pResMgr->CreateShader(_("ui_shader.xml"));
	if (!m_pShader) return false;

	for (int i = 0; i < NUM_CACHE; ++i)
	{
		int nVertexAttributeSize = m_pShader->GetVertexAttribute()->GetStride();
		m_pVertexCaches[i] = new VertexCache(4*NUM_PRIM_PER_CACHE*nVertexAttributeSize, 6*NUM_PRIM_PER_CACHE);
		if (!m_pVertexCaches[i] || !m_pVertexCaches[i]->IsOk()) return false;
	}

	Matrix4x4 matOrtho;
	Math::BuildOrthoMatrix(matOrtho, 0.0f, g_pDevice->GetSize().x, 0.0f, g_pDevice->GetSize().y, -100.0f, 100.0f);

	Matrix4x4 matModelView;
	Math::BuildTranslateMatrix(matModelView, 0.0f, g_pDevice->GetSize().y, 0.0f);
	matModelView.e[5] = -1.0f;

	m_matModelViewProj = matOrtho*matModelView;
	return true;
}

void UiRenderer_Impl::Terminate()
{
	for (int i = 0; i < NUM_CACHE; ++i)
	{
		SAFE_DELETE(m_pVertexCaches[i]);
	}
	SAFE_RELEASE(m_pShader);
}

void UiRenderer_Impl::DrawRect(const Rect& rect, const Rect& clipRect, ITexture* pTexture)
{
	DrawRect(rect.x, rect.y, rect.width, rect.height, 0.0f, 0.0f, 1.0f, 1.0f, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(const Vector2& pos, const Vector2& size, const Rect& clipRect, ITexture* pTexture)
{
	DrawRect(pos.x, pos.y, size.x, size.y, 0.0f, 0.0f, 1.0f, 1.0f, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(float x, float y, float width, float height, const Rect& clipRect, ITexture* pTexture)
{
	DrawRect(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(float x, float y, float width, float height, float u, float v, float du, float dv, const Rect& clipRect, ITexture* pTexture)
{
	static VERTEX_ATTR s_Verts[4] =
	{
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	};

	static const ushort s_Indis[6] = {0, 1, 2, 1, 3, 2};

	s_Verts[0].x = x;
	s_Verts[0].y = y;
	s_Verts[0].u = u;
	s_Verts[0].v = 1.0f - v;
	s_Verts[0].cl = clipRect.x;
	s_Verts[0].ct = clipRect.y;
	s_Verts[0].cr = clipRect.x+clipRect.width;
	s_Verts[0].cb = clipRect.y+clipRect.height;

	s_Verts[1].x = x;
	s_Verts[1].y = y + height;
	s_Verts[1].u = u;
	s_Verts[1].v = 1.0f - v - dv;
	s_Verts[1].cl = s_Verts[0].cl;
	s_Verts[1].ct = s_Verts[0].ct;
	s_Verts[1].cr = s_Verts[0].cr;
	s_Verts[1].cb = s_Verts[0].cb;

	s_Verts[2].x = x + width;
	s_Verts[2].y = y;
	s_Verts[2].u = u + du;
	s_Verts[2].v = 1.0f - v;
	s_Verts[2].cl = s_Verts[0].cl;
	s_Verts[2].ct = s_Verts[0].ct;
	s_Verts[2].cr = s_Verts[0].cr;
	s_Verts[2].cb = s_Verts[0].cb;

	s_Verts[3].x = x + width;
	s_Verts[3].y = y + height;
	s_Verts[3].u = u + du;
	s_Verts[3].v = 1.0f - v - dv;
	s_Verts[3].cl = s_Verts[0].cl;
	s_Verts[3].ct = s_Verts[0].ct;
	s_Verts[3].cr = s_Verts[0].cr;
	s_Verts[3].cb = s_Verts[0].cb;

	AddPrimetive(m_pVertexCaches, NUM_CACHE, m_pShader, pTexture, s_Verts, 4, s_Indis, 6);
}

void UiRenderer_Impl::DrawRect(const Rect& rect, const Rect& clipRect, const PieceInfo* pPieceInfo)
{
	DrawRect(rect.x, rect.y, rect.width, rect.height, clipRect, pPieceInfo);
}

void UiRenderer_Impl::DrawRect(const Vector2& pos, const Vector2& size, const Rect& clipRect, const PieceInfo* pPieceInfo)
{
	DrawRect(pos.x, pos.y, size.x, size.y, clipRect, pPieceInfo);
}

void UiRenderer_Impl::DrawRect(float x, float y, float width, float height, const Rect& clipRect, const PieceInfo* pPieceInfo)
{
	DrawRect(x, y, width, height, pPieceInfo->u, pPieceInfo->v, pPieceInfo->du, pPieceInfo->dv, clipRect, pPieceInfo->pTexture);
}

void UiRenderer_Impl::DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, ITexture* pTexture)
{
	AddPrimetive(m_pVertexCaches, NUM_CACHE, m_pShader, pTexture, pVerts, nVerts, pIndis, nIndis);
}

void UiRenderer_Impl::FlushAll()
{
	for (int i = 0; i < NUM_CACHE; ++i)
	{
		FlushCache(m_pVertexCaches[i]);
	}
}

void UiRenderer_Impl::FlushCache(VertexCache* pVertexCache)
{
	if (!pVertexCache || pVertexCache->GetNumIndis() <= 0) return;

	ITexture* pTexture = pVertexCache->GetTexture();
	IShader* pShader = pVertexCache->GetShader();

	pShader->Commit();
	if (pTexture) pShader->SetTexture(pTexture, _("u_texture"));
	pShader->SetMatrix4x4(m_matModelViewProj, _("u_matModelViewProj"));

	pShader->DrawTriangleList(pVertexCache->GetVerts(), pVertexCache->GetNumVerts(), pVertexCache->GetIndis(), pVertexCache->GetNumIndis());
	pVertexCache->Reset();
}

bool UiRenderer_Impl::AddPrimetive(VertexCache** pCache, int nNumCache, IShader* pShader, ITexture* pTexture, const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	VertexCache* pEmptyCache = NULL;
	VertexCache* pMatchCache = NULL;

	for (int i = 0; i < nNumCache; ++i)
	{
		if (pCache[i]->GetShader() == pShader
			&& pCache[i]->GetTexture() == pTexture)
		{
			pMatchCache = pCache[i];
			break;
		}

		if (!pEmptyCache && pCache[i]->GetNumVerts() == 0)
		{
			pEmptyCache = pCache[i];
		}
	}

	if (pMatchCache)
	{
		if (!pMatchCache->AddVerts(pVerts, nVerts, pIndis, nIndis))
		{
			FlushCache(pMatchCache);
			bool bOK = pMatchCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
			if (!bOK)
			{
				LOG(_("IRendererUi AddPrimetive Failed with nVerts:%d, nIndis:%d"), nVerts, nIndis);
				return false;
			}
		}
	}
	else if (pEmptyCache)
	{
		pEmptyCache->SetTexture(pTexture);
		pEmptyCache->SetShader(pShader);
		bool bOK = pEmptyCache->AddVerts(pVerts, nVerts, pIndis, nIndis);
		if (!bOK)
		{
			LOG(_("IRendererUi AddPrimetive Failed with nVerts:%d, nIndis:%d"), nVerts, nIndis);
			return false;
		}
	}
	else
	{
		LOG(_("IRendererUI no more valid vertex cache"));
		return false;
	}

	return true;
}
