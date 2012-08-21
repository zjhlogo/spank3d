/*!
 * \file RendererUi_Impl.cpp
 * \date 8-13-2012 10:49:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "RendererUi_Impl.h"
#include <util/LogUtil.h>
#include <util/SystemUtil.h>
#include <Spank3d.h>

RendererUi_Impl::RendererUi_Impl()
{
	SystemUtil::ZeroMemory(m_pVertexCaches, sizeof(m_pVertexCaches));
	m_pShader = NULL;

	g_pRendererUi = this;
}

RendererUi_Impl::~RendererUi_Impl()
{
	g_pRendererUi = NULL;
}

RendererUi_Impl& RendererUi_Impl::GetInstance()
{
	static RendererUi_Impl s_RendererUi_Impl;
	return s_RendererUi_Impl;
}

bool RendererUi_Impl::Initialize()
{
	m_pShader = g_pResMgr->CreateShader("ui_shader.xml");
	if (!m_pShader) return false;

	for (int i = 0; i < NUM_CACHE; ++i)
	{
		int nVertexAttributeSize = m_pShader->GetVertexAttribute()->GetStride();
		m_pVertexCaches[i] = new VertexCache(4*NUM_PRIM_PER_CACHE*nVertexAttributeSize, 6*NUM_PRIM_PER_CACHE);
		if (!m_pVertexCaches[i] || !m_pVertexCaches[i]->IsOk()) return false;
	}

	return true;
}

void RendererUi_Impl::Terminate()
{
	for (int i = 0; i < NUM_CACHE; ++i)
	{
		SAFE_DELETE(m_pVertexCaches[i]);
	}
	SAFE_RELEASE(m_pShader);
}

void RendererUi_Impl::DrawRect(const Rect& rect, ITexture* pTexture)
{
	// TODO: 
}

void RendererUi_Impl::DrawRect(const Vector2& pos, const Vector2& size, ITexture* pTexture)
{
	// TODO: 
}

void RendererUi_Impl::DrawRect(float x, float y, float width, float height, ITexture* pTexture)
{
	// TODO: 
}

void RendererUi_Impl::FlushAll()
{
	for (int i = 0; i < NUM_CACHE; ++i)
	{
		FlushCache(m_pVertexCaches[i]);
	}
}

void RendererUi_Impl::FlushCache(VertexCache* pVertexCache)
{
	if (!pVertexCache || pVertexCache->GetNumIndis() <= 0) return;

	ITexture* pTexture = pVertexCache->GetTexture();
	IShader* pShader = pVertexCache->GetShader();

	if (pTexture) pShader->SetTexture(pTexture, "u_texture");
	pShader->SetMatrix4x4(Math::MAT_IDENTITY, "u_matModelViewProj");

	pShader->DrawTriangleList(pVertexCache->GetVerts(), pVertexCache->GetNumVerts(), pVertexCache->GetIndis(), pVertexCache->GetNumIndis());
	pVertexCache->Reset();
}

bool RendererUi_Impl::AddPrimetive(VertexCache** pCache, int nNumCache, IShader* pShader, ITexture* pTexture, const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
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
				LOG("IRendererUi AddPrimetive Failed with nVerts:%d, nIndis:%d", nVerts, nIndis);
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
			LOG("IRendererUi AddPrimetive Failed with nVerts:%d, nIndis:%d", nVerts, nIndis);
			return false;
		}
	}
	else
	{
		LOG("IRendererUI no more valid vertex cache");
		return false;
	}

	return true;
}
