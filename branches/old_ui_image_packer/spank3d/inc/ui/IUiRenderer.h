/*!
 * \file IUiRenderer.h
 * \date 10-10-2012 9:25:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IUIRENDERER_H__
#define __IUIRENDERER_H__

#include "../core/ISingleton.h"
#include "../math/Vector2.h"
#include "../math/Rect.h"
#include "../render/ITexture.h"
#include "style/PieceInfo.h"

class IUiRenderer : public ISingleton
{
public:
	typedef struct VERTEX_ATTR_tag
	{
		float x, y, z, texIndex;
		float u, v;
		float r, g, b, a;
		float cl, ct, cr, cb;
	} VERTEX_ATTR;

public:
	RTTI_DEF(IUiRenderer, ISingleton);

	IUiRenderer() {};
	virtual ~IUiRenderer() {};

	virtual void DrawRect(const Rect& rect, uint color, const Rect& clipRect, ITexture* pTexture) = 0;
	virtual void DrawRect(const Vector2& pos, const Vector2& size, uint color, const Rect& clipRect, ITexture* pTexture) = 0;
	virtual void DrawRect(float x, float y, float width, float height, uint color, const Rect& clipRect, ITexture* pTexture) = 0;
	virtual void DrawRect(float x, float y, float width, float height, float u, float v, float du, float dv, uint color, const Rect& clipRect, ITexture* pTexture) = 0;

	virtual void DrawRect(const Rect& rect, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo) = 0;
	virtual void DrawRect(const Vector2& pos, const Vector2& size, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo) = 0;
	virtual void DrawRect(float x, float y, float width, float height, uint color, const Rect& clipRect, const PieceInfo* pPieceInfo) = 0;

	virtual void DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, const Rect& clipRect, ITexture* pTexture) = 0;
	virtual void DrawTriangleList(const VERTEX_ATTR* pVerts, uint nVerts, const ushort* pIndis, uint nIndis, uint color, const Rect& clipRect, ITexture* pTexture) = 0;

	virtual void FlushAll() = 0;

};
#endif // __IUIRENDERER_H__
