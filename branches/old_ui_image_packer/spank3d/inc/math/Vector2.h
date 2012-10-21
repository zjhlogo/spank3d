/*!
 * \file Vector2.h
 * \date 8-13-2012 10:14:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <math.h>

class Vector2
{
public:
	Vector2(float fx = 0.0f, float fy = 0.0f);

	void Reset(float fx = 0.0f, float fy = 0.0f);
	float Length() const;
	float LengthSquare() const;
	Vector2& Normalize();

	Vector2& operator +=(const Vector2& v);
	Vector2& operator -=(const Vector2& v);
	Vector2& operator *=(float s);
	Vector2& operator /=(float s);

	Vector2 operator -() const;

public:
	float x;
	float y;
};

Vector2 operator +(const Vector2& v1, const Vector2& v2);
Vector2 operator -(const Vector2& v1, const Vector2& v2);
float operator *(const Vector2& v1, const Vector2& v2);
Vector2 operator *(float s, const Vector2& v);
Vector2 operator *(const Vector2& v, float s);
Vector2 operator /(const Vector2& v, float s);

#include "Vector2.inl"

#endif // __VECTOR2_H__
