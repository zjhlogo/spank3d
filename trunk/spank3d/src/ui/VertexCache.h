/*!
 * \file VertexCache.h
 * \date 8-24-2011 9:35:44
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __VERTEXCACHE_H__
#define __VERTEXCACHE_H__

#include <core/IObject.h>
#include <render/ITexture.h>
#include <render/IShader.h>

class VertexCache : public IObject
{
public:
	RTTI_DEF(VertexCache, IObject);

	VertexCache(uint nVertsCacheSize, uint nIndisCacheSize);
	virtual ~VertexCache();

	void SetTexture(ITexture* pTexture);
	ITexture* GetTexture() const;

	void SetShader(IShader* pShader);
	IShader* GetShader() const;

	void SetLayer(int nLayer);
	int GetLayer() const;

	bool AddVerts(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	void Reset();

	const void* GetVerts() const;
	uint GetNumVerts() const;

	uint GetNumIndis() const;
	const ushort* GetIndis() const;

private:
	bool CreateCache(uint nVertsCacheSize, uint nIndisCacheSize);
	void FreeCache();

private:
	uint m_nVertsCacheSize;
	uint m_nIndisCacheSize;

	uint m_nMaxVertsCount;
	uint m_nMaxIndisCount;

	uint m_nCurrVertsCount;
	uint m_nCurrIndisCount;

	uint m_nStrideSize;

	uchar* m_pVertsCache;
	ushort* m_pIndisCache;

	ITexture* m_pTexture;
	IShader* m_pShader;
	int m_nLayer;

};
#endif // __VERTEXCACHE_H__
