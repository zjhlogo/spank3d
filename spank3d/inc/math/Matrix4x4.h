/*!
 * \file Matrix4x4.h
 * \date 26-5-2009 21:50:41
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include <math.h>

class Matrix4x4
{
public:
	Matrix4x4(float fe00 = 1.0f, float fe01 = 0.0f, float fe02 = 0.0f, float fe03 = 0.0f,
			  float fe10 = 0.0f, float fe11 = 1.0f, float fe12 = 0.0f, float fe13 = 0.0f,
			  float fe20 = 0.0f, float fe21 = 0.0f, float fe22 = 1.0f, float fe23 = 0.0f,
			  float fe30 = 0.0f, float fe31 = 0.0f, float fe32 = 0.0f, float fe33 = 1.0f);

	void Reset(float fe00 = 1.0f, float fe01 = 0.0f, float fe02 = 0.0f, float fe03 = 0.0f,
			   float fe10 = 0.0f, float fe11 = 1.0f, float fe12 = 0.0f, float fe13 = 0.0f,
			   float fe20 = 0.0f, float fe21 = 0.0f, float fe22 = 1.0f, float fe23 = 0.0f,
			   float fe30 = 0.0f, float fe31 = 0.0f, float fe32 = 0.0f, float fe33 = 1.0f);

	float Determinant() const;
	Matrix4x4& Transpose();
	Matrix4x4& Invert();

	Matrix4x4& operator +=(const Matrix4x4& m);
	Matrix4x4& operator -=(const Matrix4x4& m);
	Matrix4x4& operator *=(float s);
	Matrix4x4& operator *=(const Matrix4x4& m);
	Matrix4x4& operator /=(float s);

public:
	float e[16];

};

Matrix4x4 operator +(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator -(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator *(const Matrix4x4& m, float s);
Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 operator /(const Matrix4x4& m, float s);

#include "Matrix4x4.inl"

#endif // __MATRIX4X4_H__
