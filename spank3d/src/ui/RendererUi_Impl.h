/*!
 * \file RendererUi_Impl.h
 * \date 8-13-2012 10:47:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RENDERERUI_IMPL_H__
#define __RENDERERUI_IMPL_H__

#include <ui/IRendererUi.h>
#include "VertexCache.h"

class RendererUi_Impl : public IRendererUi
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
	RTTI_DEF(RendererUi_Impl, IRendererUi);

	static RendererUi_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void DrawRect(const Rect& rect, ITexture* pTexture);
	virtual void DrawRect(const Vector2& pos, const Vector2& size, ITexture* pTexture);
	virtual void DrawRect(float x, float y, float width, float height, ITexture* pTexture);
	virtual void DrawRect(float x, float y, float width, float height, float u, float v, float du, float dv, ITexture* pTexture);

	virtual void FlushAll();

protected:
	RendererUi_Impl();
	virtual ~RendererUi_Impl();

private:
	bool AddPrimetive(VertexCache** pCache, int nNumCache, IShader* pShader, ITexture* pTexture, const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	void FlushCache(VertexCache* pVertexCache);

private:
	VertexCache* m_pVertexCaches[NUM_CACHE];
	IShader* m_pShader;
	Matrix4x4 m_matModelViewProj;

};
#endif // __RENDERERUI_IMPL_H__
