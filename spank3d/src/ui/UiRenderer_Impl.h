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
#include "UiVertexCache.h"

class UiRenderer_Impl : public IUiRenderer
{
public:
	enum CONST_DEFINE
	{
		NUM_PRIM_PER_CACHE = 5*1024,
	};

public:
	RTTI_DEF(UiRenderer_Impl, IUiRenderer);

	static UiRenderer_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void DrawRect(const Rect& rect, uint color, const Rect& clipRect, ITexture* pTexture);
	virtual void DrawRect(const Vector2& pos, const Vector2& size, uint color, const Rect& clipRect, ITexture* pTexture);
	virtual void DrawRect(float x, float y, float width, float height, uint color, const Rect& clipRect, ITexture* pTexture);
	virtual void DrawRect(float x, float y, float width, float height, float u, float v, float du, float dv, uint color, const Rect& clipRect, ITexture* pTexture);

	virtual void DrawRect(const Rect& rect, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo);
	virtual void DrawRect(const Vector2& pos, const Vector2& size, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo);
	virtual void DrawRect(float x, float y, float width, float height, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo);

	virtual void DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, const Rect& clipRect, ITexture* pTexture);
	virtual void DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, uint color, const Rect& clipRect, ITexture* pTexture);

	virtual void FlushAll();

private:
	UiRenderer_Impl();
	virtual ~UiRenderer_Impl();

private:
	UiVertexCache* m_pVertexCache;
	IShader* m_pShader;
	Matrix4x4 m_matModelViewProj;

};
#endif // __UIRENDERER_IMPL_H__
