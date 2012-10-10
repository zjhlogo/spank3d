/*!
 * \file UiRenderer_Impl.h
 * \date 10-10-2012 9:26:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIRENDERER_IMPL_H__
#define __UIRENDERER_IMPL_H__

#include <ui/IUiRenderer.h>
#include "VertexCache.h"

class UiRenderer_Impl : public IUiRenderer
{
public:
	enum CONST_DEFINE
	{
		NUM_CACHE = 10,
		NUM_PRIM_PER_CACHE = 128,
	};

	typedef struct VATTR_XYUV_tag
	{
		float x, y;
		float u, v;
	} VATTR_XYUV;

public:
	RTTI_DEF(UiRenderer_Impl, IUiRenderer);

	static UiRenderer_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void DrawRect(const Rect& rect, ITexture* pTexture);
	virtual void DrawRect(const Vector2& pos, const Vector2& size, ITexture* pTexture);
	virtual void DrawRect(float x, float y, float width, float height, ITexture* pTexture);
	virtual void DrawRect(float x, float y, float width, float height, float u, float v, float du, float dv, ITexture* pTexture);

	virtual void DrawRect(const Rect& rect, const PieceInfo* pPieceInfo);
	virtual void DrawRect(const Vector2& pos, const Vector2& size, const PieceInfo* pPieceInfo);
	virtual void DrawRect(float x, float y, float width, float height, const PieceInfo* pPieceInfo);

	virtual void DrawTriangleList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, ITexture* pTexture);

	virtual void FlushAll();

private:
	UiRenderer_Impl();
	virtual ~UiRenderer_Impl();

private:
	bool AddPrimetive(VertexCache** pCache, int nNumCache, IShader* pShader, ITexture* pTexture, const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	void FlushCache(VertexCache* pVertexCache);

private:
	VertexCache* m_pVertexCaches[NUM_CACHE];
	IShader* m_pShader;
	Matrix4x4 m_matModelViewProj;

};
#endif // __UIRENDERER_IMPL_H__
