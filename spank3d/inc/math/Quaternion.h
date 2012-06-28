/*!
 * \file Quaternion.h
 * \date 26-5-2009 22:22:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Vector3.h"

class Quaternion
{
public:
	Quaternion(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f, float fw = 1.0f);
// 	Quaternion(const Vector3& dir, float radian);

	void Reset(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f, float fw = 1.0f);
	float Length() const;
	float SquaredLength() const;
	void Normalize();
	float GetRadian() const;

	Quaternion& operator +=(const Quaternion& q);
	Quaternion& operator -=(const Quaternion& q);
	Quaternion& operator *=(float s);
	Quaternion& operator /=(float s);
	Quaternion operator -() const;

public:
	float x;
	float y;
	float z;
	float w;	// number (scalar) part

};

Quaternion operator +(const Quaternion& q1, const Quaternion& q2);
Quaternion operator -(const Quaternion& q1, const Quaternion& q2);
Quaternion operator *(const Quaternion& q1, const Quaternion& q2);
// Quaternion operator *(const Quaternion& q, const Vector3& v);
// Quaternion operator *(const Vector3& v, const Quaternion& q);
Quaternion operator *(float s, const Quaternion& q);
Quaternion operator *(const Quaternion& q, float s);
Quaternion operator /(const Quaternion& q, float s);

#include "Quaternion.inl"

#endif // __QUATERNION_H__
