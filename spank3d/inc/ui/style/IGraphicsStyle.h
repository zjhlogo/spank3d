/*!
 * \file IGraphicsStyle.h
 * \date 9-20-2012 7:28:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IGRAPHICSSTYLE_H__
#define __IGRAPHICSSTYLE_H__

#include "IStyle.h"

class IGraphicsStyle : public IStyle
{
public:
	RTTI_DEF(IGraphicsStyle, IStyle);

	IGraphicsStyle(const tstring& id);
	virtual ~IGraphicsStyle();

	void SetClientRect(const Rect& rect);
	void SetClientRect(const Vector2& pos, const Vector2& size);
	void SetClientRect(float x, float y, float width, float height);
	const Rect& GetClientRect() const;

	virtual bool Render(const Vector2& pos, const Vector2& size) = 0;
};
#endif // __IGRAPHICSSTYLE_H__
