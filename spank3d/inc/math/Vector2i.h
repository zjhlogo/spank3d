/*!
 * \file Vector2i.h
 * \date 26-5-2009 21:09:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR2I_H__
#define __VECTOR2I_H__

#include <math.h>

class Vector2i
{
public:
	Vector2i(int nx = 0, int ny = 0);

	void Reset(int nx = 0, int ny = 0);

public:
	int x;
	int y;

};

#include "Vector2i.inl"

#endif // __VECTOR2I_H__
