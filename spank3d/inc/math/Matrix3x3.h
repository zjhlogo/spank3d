/*!
 * \file Matrix3x3.h
 * \date 10-26-2012 22:56:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MATRIX3X3_H__
#define __MATRIX3X3_H__

#include <math.h>

class Matrix3x3
{
public:
	Matrix3x3(float fe00 = 1.0f, float fe01 = 0.0f, float fe02 = 0.0f,
			  float fe10 = 0.0f, float fe11 = 1.0f, float fe12 = 0.0f,
			  float fe20 = 0.0f, float fe21 = 0.0f, float fe22 = 1.0f);

	void Reset(float fe00 = 1.0f, float fe01 = 0.0f, float fe02 = 0.0f,
			   float fe10 = 0.0f, float fe11 = 1.0f, float fe12 = 0.0f,
			   float fe20 = 0.0f, float fe21 = 0.0f, float fe22 = 1.0f);

	float Determinant() const;
	Matrix3x3& Transpose();
	Matrix3x3& Invert();

	Matrix3x3& operator +=(const Matrix3x3& m);
	Matrix3x3& operator -=(const Matrix3x3& m);
	Matrix3x3& operator *=(float s);
	Matrix3x3& operator *=(const Matrix3x3& m);
	Matrix3x3& operator /=(float s);

public:
	float e[9];

};

Matrix3x3 operator +(const Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator -(const Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator *(const Matrix3x3& m, float s);
Matrix3x3 operator *(const Matrix3x3& m1, const Matrix3x3& m2);
Matrix3x3 operator /(const Matrix3x3& m, float s);

#include "Matrix3x3.inl"
#endif // __MATRIX3X3_H__
