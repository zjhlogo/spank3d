/*!
 * \file Math.cpp
 * \date 4-14-2012 18:03:37
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "math/Math.h"

const float Math::PI_2 = 1.570796325f;
const float Math::PI = 3.14159265f;
const float Math::PIX2 = 6.2831853f;
const float Math::G = -32.174f;							// acceleration due to gravity, ft/s^2
const float Math::RHO = 0.0023769f;						// desity of air at sea level, slugs/ft^3
const float Math::TOL = 1E-5f;							// float type tolerance
const float Math::FLOAT_MIN = 1.175494351e-38F;			// 1.175494351e-38F
const float Math::FLOAT_MAX = 3.402823466e+38F;			// 3.402823466e+38F

float Math::Radians(float degrees)
{
	return PI/180.0f*degrees;
}

float Math::Degrees(float radians)
{
	return radians*180.0f/PI;
}

void Math::BuildPerspectiveFovMatrix(Matrix4x4& m, float fovy, float aspect, float znear, float zfar)
{
// 	float range = tanf(fovy*0.5f) * znear;
// 	float left = -range * aspect;
// 	float right = range * aspect;
// 	float bottom = -range;
// 	float top = range;
// 
// 	m.Reset((2.0f*znear)/(right-left),        0.0f,        0.0f,                    0.0f,
// 			0.0f,     (2.0f*znear)/(top-bottom),           0.0f,                    0.0f,
// 			0.0f,     0.0f,        -(zfar+znear)/(zfar-znear),          -1.0f,
// 			0.0f,     0.0f,        -(2.0f*zfar*znear)/(zfar-znear),    0.0f);
	float y = 1.0f / tanf(fovy * 0.5f);
	float x = y / aspect;
	float zdist = (znear - zfar);
	float zfar_per_zdist = zfar / zdist;

	m.Reset(x,        0.0f,        0.0f,                    0.0f,
			0.0f,     y,           0.0f,                    0.0f,
			0.0f,     0.0f,        zfar_per_zdist,          -1.0f,
			0.0f,     0.0f,        znear*zfar_per_zdist,    0.0f);
}

void Math::BuildOrthoMatrix(Matrix4x4& m, float w, float h, float znear, float zfar)
{
	const float dz = (znear - zfar);
	const float dzi = 1.0f/dz;

	m.Reset(2.0f/w,     0.0f,       0.0f,       0.0f,
			0.0f,       2.0f/h,     0.0f,       0.0f,
			0.0f,       0.0f,       dzi,        0.0f,
			0.0f,       0.0f,       znear*dzi,  1.0f);
}

void Math::BuildLookAtMatrix(Matrix4x4& m, const Vector3& eye, const Vector3& at, const Vector3& up)
{
	Vector3 zaxis = eye-at;
	zaxis.Normalize();

	Vector3 xaxis = up^zaxis;
	xaxis.Normalize();

	Vector3 yaxis = zaxis^xaxis;

	m.Reset(xaxis.x,           yaxis.x,           zaxis.x,          0.0f,
			xaxis.y,           yaxis.y,           zaxis.y,          0.0f,
			xaxis.z,           yaxis.z,           zaxis.z,          0.0f,
			-(xaxis*eye),      -(yaxis*eye),      -(zaxis*eye),     1.0f);
}

void Math::BuildTranslateMatrix(Matrix4x4& m, const Vector3& v)
{
	BuildTranslateMatrix(m, v.x, v.y, v.z);
}

void Math::BuildTranslateMatrix(Matrix4x4& m, float x, float y, float z)
{
	m.Reset(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x,    y,    z,    1.0f);
}

void Math::BuildScaleMatrix(Matrix4x4& m, const Vector3& v)
{
	BuildScaleMatrix(m, v.x, v.y, v.z);
}

void Math::BuildScaleMatrix(Matrix4x4& m, float s)
{
	BuildScaleMatrix(m, s, s, s);
}

void Math::BuildScaleMatrix(Matrix4x4& m, float x, float y, float z)
{
	m.Reset(x,    0.0f, 0.0f, 0.0f,
			0.0f, y,    0.0f, 0.0f,
			0.0f, 0.0f, z,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
}

void Math::BuildRotationXMatrix(Matrix4x4& m, float r)
{
	float c = cosf(r);
	float s = sinf(r);

	m.Reset(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, c,    s,    0.0f,
			0.0f, -s,   c,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
}

void Math::BuildRotationYMatrix(Matrix4x4& m, float r)
{
	float c = cosf(r);
	float s = sinf(r);

	m.Reset(c,    0.0f, -s,   0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			s,    0.0f, c,    0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
}

void Math::BuildRotationZMatrix(Matrix4x4& m, float r)
{
	float c = cosf(r);
	float s = sinf(r);

	m.Reset(c,    s,    0.0f, 0.0f,
			-s,   c,    0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
}
