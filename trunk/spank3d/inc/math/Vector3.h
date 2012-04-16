/*!
 * \file Vector3.h
 * \date 26-5-2009 21:09:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <math.h>

class Vector3
{
public:
	Vector3(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f);

	void Reset(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f);
	float Length() const;
	float LengthSquare() const;
	Vector3& Normalize();

	Vector3& operator +=(const Vector3& v);
	Vector3& operator -=(const Vector3& v);
	Vector3& operator *=(float s);
	Vector3& operator /=(float s);

	Vector3 operator -() const;

public:
	float x;
	float y;
	float z;

};

Vector3 operator +(const Vector3& v1, const Vector3& v2);
Vector3 operator -(const Vector3& v1, const Vector3& v2);
float operator *(const Vector3& v1, const Vector3& v2);
Vector3 operator *(float s, const Vector3& v);
Vector3 operator *(const Vector3& v, float s);
Vector3 operator /(const Vector3& v, float s);
Vector3 operator ^(const Vector3& v1, const Vector3& v2);

#include "Vector3.inl"

#endif // __VECTOR3_H__
