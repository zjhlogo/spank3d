/*!
 * \file UiVertexCache.cpp
 * \date 8-24-2011 9:35:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiVertexCache.h"

UiVertexCache::UiVertexCache(uint nNumPrims)
{
	m_nMaxVerts = 0;
	m_nMaxIndis = 0;

	m_pVerts = NULL;
	m_nVerts = 0;
	m_pIndis = NULL;
	m_nIndis = 0;

	for (int i = 0; i < MAX_TEXTURE_CACHE; ++i)
	{
		m_pTexture[i] = NULL;
	}

	SetOk(CreateCache(nNumPrims));
}

UiVertexCache::~UiVertexCache()
{
	FreeCache();
}

bool UiVertexCache::AddVerts(const IUiRenderer::VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, const Rect& clipRect, ITexture* pTexture)
{
	if (m_nVerts + nVerts > m_nMaxVerts) return false;
	if (m_nIndis + nIndis > m_nMaxIndis) return false;

	ushort texId = AddTexture(pTexture);
	if (texId >= MAX_TEXTURE_CACHE) return false;

	for (uint i = 0; i < nVerts; ++i)
	{
		int index = m_nVerts+i;
		m_pVerts[index].x = pVerts[i].x;
		m_pVerts[index].y = pVerts[i].y;
		m_pVerts[index].z = 0.0f;
		m_pVerts[index].texIndex = texId;

		m_pVerts[index].u = pVerts[i].u;
		m_pVerts[index].v = pVerts[i].v;

		m_pVerts[index].r = pVerts[i].r;
		m_pVerts[index].g = pVerts[i].g;
		m_pVerts[index].b = pVerts[i].b;
		m_pVerts[index].a = pVerts[i].a;

		m_pVerts[index].cl = clipRect.x;
		m_pVerts[index].ct = clipRect.y;
		m_pVerts[index].cr = clipRect.x+clipRect.width;
		m_pVerts[index].cb = clipRect.y+clipRect.height;
	}

	for (uint i = 0; i < nIndis; ++i)
	{
		m_pIndis[m_nIndis+i] = m_nVerts+pIndis[i];
	}

	m_nVerts += nVerts;
	m_nIndis += nIndis;

	return true;
}

bool UiVertexCache::AddVerts(const IUiRenderer::VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, uint color, const Rect& clipRect, ITexture* pTexture)
{
	if (m_nVerts + nVerts > m_nMaxVerts) return false;
	if (m_nIndis + nIndis > m_nMaxIndis) return false;

	ushort texId = AddTexture(pTexture);
	if (texId >= MAX_TEXTURE_CACHE) return false;

	for (uint i = 0; i < nVerts; ++i)
	{
		int index = m_nVerts+i;
		m_pVerts[index].x = pVerts[i].x;
		m_pVerts[index].y = pVerts[i].y;
		m_pVerts[index].z = 0.0f;
		m_pVerts[index].texIndex = texId;

		m_pVerts[index].u = pVerts[i].u;
		m_pVerts[index].v = pVerts[i].v;

		m_pVerts[index].r = ((color >> 24) & 0x000000FF) / 255.0f;
		m_pVerts[index].g = ((color >> 16) & 0x000000FF) / 255.0f;
		m_pVerts[index].b = ((color >> 8) & 0x000000FF) / 255.0f;
		m_pVerts[index].a = (color & 0x000000FF) / 255.0f;

		m_pVerts[index].cl = clipRect.x;
		m_pVerts[index].ct = clipRect.y;
		m_pVerts[index].cr = clipRect.x+clipRect.width;
		m_pVerts[index].cb = clipRect.y+clipRect.height;
	}

	for (uint i = 0; i < nIndis; ++i)
	{
		m_pIndis[m_nIndis+i] = m_nVerts+pIndis[i];
	}

	m_nVerts += nVerts;
	m_nIndis += nIndis;

	return true;
}

void UiVertexCache::Flush(IShader* pShader, const Matrix4x4& matMVP)
{
	static const tchar* s_TexNames[MAX_TEXTURE_CACHE] = 
	{
		_("u_texture1"),
		_("u_texture2"),
		_("u_texture3"),
		_("u_texture4"),
	};

	if (m_nVerts <= 0) return;

	pShader->BeginRender();
	for (int i = 0; i < MAX_TEXTURE_CACHE; ++i)
	{
		if (!m_pTexture[i]) break;
		pShader->SetTexture(m_pTexture[i], s_TexNames[i], i);
		m_pTexture[i] = NULL;
	}
	pShader->SetMatrix4x4(matMVP, _("u_matModelViewProj"));
	pShader->DrawTriangleList(m_pVerts, m_nVerts, m_pIndis, m_nIndis);
	pShader->EndRender();

	m_nVerts = 0;
	m_nIndis = 0;
}

bool UiVertexCache::CreateCache(uint nNumPrims)
{
	FreeCache();

	m_nMaxVerts = NUM_VERTEX_PER_PRIM*nNumPrims;
	m_nMaxIndis = NUM_INDEX_PER_PRIM*nNumPrims;

	m_pVerts = new IUiRenderer::VERTEX_ATTR[m_nMaxVerts];
	if (!m_pVerts) return false;

	m_pIndis = new ushort[m_nMaxIndis];
	if (!m_pIndis) return false;

	return true;
}

void UiVertexCache::FreeCache()
{
	m_nMaxVerts = 0;
	m_nMaxIndis = 0;

	for (int i = 0; i < MAX_TEXTURE_CACHE; ++i)
	{
		m_pTexture[i] = NULL;
	}

	SAFE_DELETE_ARRAY(m_pVerts);
	m_nVerts = 0;

	SAFE_DELETE_ARRAY(m_pIndis);
	m_nIndis = 0;
}

ushort UiVertexCache::AddTexture(ITexture* pTexture)
{
	for (ushort i = 0; i < MAX_TEXTURE_CACHE; ++i)
	{
		if (!m_pTexture[i])
		{
			m_pTexture[i] = pTexture;
			return i;
		}

		if (m_pTexture[i] == pTexture) return i;
	}

	return MAX_TEXTURE_CACHE;
}
