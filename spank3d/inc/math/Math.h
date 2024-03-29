/*!
 * \file Math.h
 * \date 4-14-2012 18:03:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MATH_H__
#define __MATH_H__

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

class Math
{
public:
	static float Radians(float degrees);
	static float Degrees(float radians);
	static float Random();
	static float Random(float min, float max);

	static void BuildPerspectiveFovMatrix(Matrix4x4& m, float fovDegree, float width, float height, float znear, float zfar);
	static void BuildPerspectiveFovMatrix(Matrix4x4& m, float fovy, float aspect, float znear, float zfar);
	static void BuildOrthoMatrix(Matrix4x4& m, float w, float h, float znear, float zfar);
	static void BuildOrthoMatrix(Matrix4x4& m, float left, float right, float bottom, float top, float znear, float zfar);
	static void BuildLookAtMatrix(Matrix4x4& m, const Vector3& eye, const Vector3& at, const Vector3& up);

	static void BuildTranslateMatrix(Matrix4x4& m, const Vector3& v);
	static void BuildTranslateMatrix(Matrix4x4& m, float x, float y, float z);
	static void GetMatrixTranslate(Vector3& v, const Matrix4x4& m);

	static void BuildScaleMatrix(Matrix4x4& m, const Vector3& v);
	static void BuildScaleMatrix(Matrix4x4& m, float s);
	static void BuildScaleMatrix(Matrix4x4& m, float x, float y, float z);
	static void GetMatrixScale(Vector3& v, const Matrix4x4& m);

	static void BuildRotationXMatrix(Matrix4x4& m, float r);
	static void BuildRotationYMatrix(Matrix4x4& m, float r);
	static void BuildRotationZMatrix(Matrix4x4& m, float r);

	static void BuildMatrixFromEulerXYZ(Matrix4x4& m, float x, float y, float z);

	static void BuildQuaternionFromMatrix(Quaternion& q, const Matrix4x4& m);
	static void BuildQuaternionFromEulerXYZ(Quaternion& q, float x, float y, float z);

	static void GetSubMatrix(Matrix3x3& mOut, const Matrix4x4& mIn);

public:
	static const float PI_2;		// pi/2
	static const float PI;			// pi
	static const float PIX2;		// pi*2
	static const float G;			// acceleration due to gravity, ft/s^2
	static const float RHO;			// desity of air at sea level, slugs/ft^3
	static const float TOL;			// float type tolerance
	static const float FLOAT_MIN;	// 1.175494351e-38F
	static const float FLOAT_MAX;	// 3.402823466e+38F

	static const Vector2 VEC2_ZERO;

	static const Vector3 VEC3_ZERO;
	static const Vector3 VEC3_UP;

	static const Matrix3x3 MAT3_IDENTITY;
	static const Matrix4x4 MAT4_IDENTITY;

};
#endif // __MATH_H__
