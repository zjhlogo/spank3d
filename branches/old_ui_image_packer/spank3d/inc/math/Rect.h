/*!
 * \file Rect.h
 * \date 8-13-2012 10:13:42
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RECT_H__
#define __RECT_H__

#include "Vector2.h"

class Rect
{
public:
	Rect(float fx = 0.0f, float fy = 0.0f, float fwidth = 0.0f, float fheight = 0.0f);

	void Reset(float fx = 0.0f, float fy = 0.0f, float fwidth = 0.0f, float fheight = 0.0f);
	Rect Intersect(const Rect& rect) const;
	Rect Intersect(const Vector2& pos, const Vector2& size) const;

public:
	float x;
	float y;
	float width;
	float height;

};

#include "Rect.inl"

#endif // __RECT_H__
