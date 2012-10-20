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
	m_pVertexCache = NULL;
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

	int nVertexAttributeSize = m_pShader->GetVertexAttribute()->GetStride();
	m_pVertexCache = new UiVertexCache(NUM_PRIM_PER_CACHE);
	if (!m_pVertexCache || !m_pVertexCache->IsOk()) return false;

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
	SAFE_DELETE(m_pVertexCache);
	SAFE_RELEASE(m_pShader);
}

void UiRenderer_Impl::DrawRect(const Rect& rect, uint color, const Rect& clipRect, ITexture* pTexture)
{
	DrawRect(rect.x, rect.y, rect.width, rect.height, 0.0f, 0.0f, 1.0f, 1.0f, color, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(const Vector2& pos, const Vector2& size, uint color, const Rect& clipRect, ITexture* pTexture)
{
	DrawRect(pos.x, pos.y, size.x, size.y, 0.0f, 0.0f, 1.0f, 1.0f, color, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(float x, float y, float width, float height, uint color, const Rect& clipRect, ITexture* pTexture)
{
	DrawRect(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, color, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(float x, float y, float width, float height, float u, float v, float du, float dv, uint color, const Rect& clipRect, ITexture* pTexture)
{
	static VERTEX_ATTR s_Verts[4] =
	{	// x     y    z   texIndex u    v     r     g     b     a     cl    ct    cr    cb
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
	};

	static const ushort s_Indis[6] = {0, 1, 2, 1, 3, 2};

	s_Verts[0].x = x;
	s_Verts[0].y = y;
	s_Verts[0].u = u;
	s_Verts[0].v = 1.0f - v;

	s_Verts[1].x = x;
	s_Verts[1].y = y + height;
	s_Verts[1].u = u;
	s_Verts[1].v = 1.0f - v - dv;

	s_Verts[2].x = x + width;
	s_Verts[2].y = y;
	s_Verts[2].u = u + du;
	s_Verts[2].v = 1.0f - v;

	s_Verts[3].x = x + width;
	s_Verts[3].y = y + height;
	s_Verts[3].u = u + du;
	s_Verts[3].v = 1.0f - v - dv;

	DrawTriangleList(s_Verts, 4, s_Indis, 6, color, clipRect, pTexture);
}

void UiRenderer_Impl::DrawRect(const Rect& rect, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo)
{
	DrawRect(rect.x, rect.y, rect.width, rect.height, color, clipRect, pPieceInfo);
}

void UiRenderer_Impl::DrawRect(const Vector2& pos, const Vector2& size, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo)
{
	DrawRect(pos.x, pos.y, size.x, size.y, color, clipRect, pPieceInfo);
}

void UiRenderer_Impl::DrawRect(float x, float y, float width, float height, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo)
{
	DrawRect(x, y, width, height, pPieceInfo->u, pPieceInfo->v, pPieceInfo->du, pPieceInfo->dv, color, clipRect, pPieceInfo->pTexture);
}

void UiRenderer_Impl::DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, const Rect& clipRect, ITexture* pTexture)
{
	if (!m_pVertexCache->AddVerts(pVerts, nVerts, pIndis, nIndis, clipRect, pTexture))
	{
		m_pVertexCache->Flush(m_pShader, m_matModelViewProj);
		bool bOK = m_pVertexCache->AddVerts(pVerts, nVerts, pIndis, nIndis, clipRect, pTexture);
		if (!bOK) LOG(_T("IRendererUi AddPrimetive Failed with nVerts:%d, nIndis:%d"), nVerts, nIndis);
	}
}

void UiRenderer_Impl::DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, uint color, const Rect& clipRect, ITexture* pTexture)
{
	if (!m_pVertexCache->AddVerts(pVerts, nVerts, pIndis, nIndis, color, clipRect, pTexture))
	{
		m_pVertexCache->Flush(m_pShader, m_matModelViewProj);
		bool bOK = m_pVertexCache->AddVerts(pVerts, nVerts, pIndis, nIndis, color, clipRect, pTexture);
		if (!bOK) LOG(_T("IRendererUi AddPrimetive Failed with nVerts:%d, nIndis:%d"), nVerts, nIndis);
	}
}

void UiRenderer_Impl::FlushAll()
{
	m_pVertexCache->Flush(m_pShader, m_matModelViewProj);
}
