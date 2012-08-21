/*!
 * \file VertexCache.cpp
 * \date 8-24-2011 9:35:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "VertexCache.h"

VertexCache::VertexCache(uint nVertsCacheSize, uint nIndisCacheSize)
{
	m_nVertsCacheSize = 0;
	m_nIndisCacheSize = 0;

	m_nMaxVertsCount = 0;
	m_nMaxIndisCount = 0;

	m_nCurrVertsCount = 0;
	m_nCurrIndisCount = 0;

	m_nStrideSize = 0;

	m_pVertsCache = NULL;
	m_pIndisCache = NULL;

	m_pTexture = NULL;
	m_pShader = NULL;
	m_nLayer = 0;

	SetOk(CreateCache(nVertsCacheSize, nIndisCacheSize));
}

VertexCache::~VertexCache()
{
	FreeCache();
}

void VertexCache::SetTexture(ITexture* pTexture)
{
	m_pTexture = pTexture;
}

ITexture* VertexCache::GetTexture() const
{
	return m_pTexture;
}

void VertexCache::SetShader(IShader* pShader)
{
	if (m_pShader == pShader) return;

	m_pShader = pShader;

	m_nStrideSize = 0;
	m_nMaxVertsCount = 0;

	m_nCurrVertsCount = 0;
	m_nCurrIndisCount = 0;

	if (m_pShader)
	{
		m_nStrideSize = m_pShader->GetVertexAttribute()->GetStride();
		m_nMaxVertsCount = m_nVertsCacheSize / m_nStrideSize;
	}
}

IShader* VertexCache::GetShader() const
{
	return m_pShader;
}

void VertexCache::SetLayer(int nLayer)
{
	m_nLayer = nLayer;
}

int VertexCache::GetLayer() const
{
	return m_nLayer;
}

bool VertexCache::AddVerts(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis)
{
	if (m_nCurrVertsCount + nVerts > m_nMaxVertsCount) return false;
	if (m_nCurrIndisCount + nIndis > m_nMaxIndisCount) return false;

	uchar* pVertsCache = m_pVertsCache + m_nCurrVertsCount*m_nStrideSize;
	memcpy(pVertsCache, pVerts, nVerts*m_nStrideSize);

	for (uint i = 0; i < nIndis; ++i)
	{
		m_pIndisCache[m_nCurrIndisCount+i] = m_nCurrVertsCount+pIndis[i];
	}

	m_nCurrVertsCount += nVerts;
	m_nCurrIndisCount += nIndis;

	return true;
}

void VertexCache::Reset()
{
	m_nCurrVertsCount = 0;
	m_nCurrIndisCount = 0;
}

const void* VertexCache::GetVerts() const
{
	return m_pVertsCache;
}

uint VertexCache::GetNumVerts() const
{
	return m_nCurrVertsCount;
}

const ushort* VertexCache::GetIndis() const
{
	return m_pIndisCache;
}

uint VertexCache::GetNumIndis() const
{
	return m_nCurrIndisCount;
}

bool VertexCache::CreateCache(uint nVertsCacheSize, uint nIndisCacheSize)
{
	FreeCache();

	m_nVertsCacheSize = nVertsCacheSize;
	m_nIndisCacheSize = nIndisCacheSize;

	m_pVertsCache = new uchar[m_nVertsCacheSize];
	if (!m_pVertsCache) return false;

	m_pIndisCache = new ushort[m_nIndisCacheSize];
	if (!m_pIndisCache) return false;

	m_nMaxIndisCount = m_nIndisCacheSize;

	return true;
}

void VertexCache::FreeCache()
{
	m_nVertsCacheSize = 0;
	m_nIndisCacheSize = 0;

	m_nMaxVertsCount = 0;
	m_nMaxIndisCount = 0;

	m_nCurrVertsCount = 0;
	m_nCurrIndisCount = 0;

	SAFE_DELETE_ARRAY(m_pVertsCache);
	SAFE_DELETE_ARRAY(m_pIndisCache);
}
