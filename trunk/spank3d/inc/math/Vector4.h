/*!
 * \file Vector4.h
 * \date 26-5-2009 21:27:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include <math.h>

class Vector4
{
public:
	Vector4(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f, float fw = 0.0f);

	void Reset(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f, float fw = 0.0f);
	float Length() const;
	float LengthSquare() const;
	void Normalize();

	Vector4& operator +=(const Vector4& v);
	Vector4& operator -=(const Vector4& v);
	Vector4& operator *=(float s);
	Vector4& operator /=(float s);

	Vector4 operator -() const;

public:
	float x;
	float y;
	float z;
	float w;

};

Vector4 operator *(const Vector4& v, float s);
Vector4 operator *(float s, const Vector4& v);

#include "Vector4.inl"

#endif // __VECTOR4_H__
