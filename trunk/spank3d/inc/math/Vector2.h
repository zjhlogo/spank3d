/*!
 * \file Vector2.h
 * \date 8-13-2012 10:14:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

class Vector2
{
public:
	Vector2(float fx = 0.0f, float fy = 0.0f);

	void Reset(float fx = 0.0f, float fy = 0.0f);

public:
	float x;
	float y;
};

#include "Vector2.inl"

#endif // __VECTOR2_H__
