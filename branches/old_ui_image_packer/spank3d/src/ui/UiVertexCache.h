/*!
 * \file UiVertexCache.h
 * \date 10-19-2012 9:11:16
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIVERTEXCACHE_H__
#define __UIVERTEXCACHE_H__

#include <core/IObject.h>
#include <ui/IUiRenderer.h>
#include <render/IShader.h>

class UiVertexCache : public IObject
{
public:
	enum CONST_DEFINE
	{
		MAX_TEXTURE_CACHE = 4,
		NUM_VERTEX_PER_PRIM = 4,
		NUM_INDEX_PER_PRIM = 6,
	};

public:
	RTTI_DEF(UiVertexCache, IObject);

	UiVertexCache(uint nNumPrims);
	virtual ~UiVertexCache();

	bool AddVerts(const IUiRenderer::VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, const Rect& clipRect, ITexture* pTexture);
	bool AddVerts(const IUiRenderer::VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, uint color, const Rect& clipRect, ITexture* pTexture);

	void Flush(IShader* pShader, const Matrix4x4& matMVP);

private:
	bool CreateCache(uint nNumPrims);
	void FreeCache();
	ushort AddTexture(ITexture* pTexture);

private:
	uint m_nMaxVerts;
	uint m_nMaxIndis;

	IUiRenderer::VERTEX_ATTR* m_pVerts;
	uint m_nVerts;
	ushort* m_pIndis;
	uint m_nIndis;

	ITexture* m_pTexture[MAX_TEXTURE_CACHE];

};
#endif // __UIVERTEXCACHE_H__
