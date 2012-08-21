/*!
 * \file IRendererUi.h
 * \date 8-13-2012 10:42:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IRENDERERUI_H__
#define __IRENDERERUI_H__

#include "../core/ISingleton.h"
#include "../math/Vector2.h"
#include "../math/Rect.h"
#include "../render/ITexture.h"

class IRendererUi : public ISingleton
{
public:
	RTTI_DEF(IRendererUi, ISingleton);

	IRendererUi() {};
	virtual ~IRendererUi() {};

	virtual void DrawRect(const Rect& rect, ITexture* pTexture) = 0;
	virtual void DrawRect(const Vector2& pos, const Vector2& size, ITexture* pTexture) = 0;
	virtual void DrawRect(float x, float y, float width, float height, ITexture* pTexture) = 0;

	virtual void FlushAll() = 0;

};
#endif // __IRENDERERUI_H__
