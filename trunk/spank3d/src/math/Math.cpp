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

const Vector3 Math::VEC_UP = Vector3(0.0f, 1.0f, 0.0f);

float Math::Radians(float degrees)
{
	return PI/180.0f*degrees;
}

float Math::Degrees(float radians)
{
	return radians*180.0f/PI;
}

void Math::BuildPerspectiveFovMatrix(Matrix4x4& m, float fovDegree, int width, int height, float znear, float zfar)
{
	BuildPerspectiveFovMatrix(m, Radians(fovDegree), (float)width/(float)height, znear, zfar);
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

void Math::GetMatrixTranslate(Vector3& v, const Matrix4x4& m)
{
	v.x = m.e[12];
	v.y = m.e[13];
	v.z = m.e[14];
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

void Math::GetMatrixScale(Vector3& v, const Matrix4x4& m)
{
	v.x = sqrtf(m.e[0]*m.e[0] + m.e[1]*m.e[1] + m.e[2]*m.e[2]);
	v.y = sqrtf(m.e[4]*m.e[4] + m.e[5]*m.e[5] + m.e[6]*m.e[6]);
	v.z = sqrtf(m.e[8]*m.e[8] + m.e[9]*m.e[9] + m.e[10]*m.e[10]);
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

void Math::BuildMatrixFromEulerXYZ(Matrix4x4& m, float x, float y, float z)
{
	// formula
	//     [ cos(x)*cos(z)-cos(y)*sin(x)*sin(z),     -cos(y)*cos(z)*sin(x)-cos(x)*sin(z),      sin(x)*sin(y),     0 ]
	// R = [ cos(z)*sin(x)+cos(x)*cos(y)*sin(z),      cos(x)*cos(y)*cos(z)-sin(x)*sin(z),     -cos(x)*sin(y),     0 ]
	//     [-sin(y)*sin(z),                           cos(z)*sin(y),                           cos(y),            0 ]
	//     [ 0,                                       0,                                       0,                 1 ]

	float cx = cosf(x);
	float cy = cosf(y);
	float cz = cosf(z);
	float sx = sinf(x);
	float sy = sinf(y);
	float sz = sinf(z);

	m.e[0] = cx*cz-cy*sx*sz;
	m.e[1] = -cy*cz*sx-cx*sz;
	m.e[2] = sx*sy;
	m.e[3] = 0.0f;

	m.e[4] = cz*sx+cx*cy*sz;
	m.e[5] = cx*cy*cz-sx*sz;
	m.e[6] = -cx*sy;
	m.e[7] = 0.0f;

	m.e[8] = -sy*sz;
	m.e[9] = cz*sy;
	m.e[10] = cy;
	m.e[11] = 0.0f;

	m.e[12] = 0.0f;
	m.e[13] = 0.0f;
	m.e[14] = 0.0f;
	m.e[15] = 1.0f;
}

void Math::BuildQuaternionFromMatrix(Quaternion& q, const Matrix4x4& m)
{
	float fWSquaredMinus1 = m.e[0] + m.e[5] + m.e[10];
	float fXSquaredMinus1 = m.e[0] - m.e[5] - m.e[10];
	float fYSquaredMinus1 = m.e[5] - m.e[0] - m.e[10];
	float fZSquaredMinus1 = m.e[10] - m.e[0] - m.e[5];

	int nBiggestIndex = 0;
	float fBiggestSquaredMinus1 = fWSquaredMinus1;

	if (fXSquaredMinus1 > fBiggestSquaredMinus1)
	{
		fBiggestSquaredMinus1 = fXSquaredMinus1;
		nBiggestIndex = 1;
	}

	if (fYSquaredMinus1 > fBiggestSquaredMinus1)
	{
		fBiggestSquaredMinus1 = fYSquaredMinus1;
		nBiggestIndex = 2;
	}

	if (fZSquaredMinus1 > fBiggestSquaredMinus1)
	{
		fBiggestSquaredMinus1 = fZSquaredMinus1;
		nBiggestIndex = 3;
	}

	// 
	float fBiggestValue = sqrt(fBiggestSquaredMinus1 + 1.0f) * 0.5f;
	float fMult = 0.25f / fBiggestValue;

	// 
	switch (nBiggestIndex)
	{
	case 0:
		q.w = fBiggestValue;
		q.x = (m.e[6] - m.e[9]) * fMult;
		q.y = (m.e[8] - m.e[2]) * fMult;
		q.z = (m.e[1] - m.e[4]) * fMult;
		break;
	case 1:
		q.x = fBiggestValue;
		q.w = (m.e[6] - m.e[9]) * fMult;
		q.y = (m.e[1] + m.e[4]) * fMult;
		q.z = (m.e[8] + m.e[2]) * fMult;
		break;
	case 2:
		q.y = fBiggestValue;
		q.w = (m.e[8] - m.e[2]) * fMult;
		q.x = (m.e[1] + m.e[4]) * fMult;
		q.z = (m.e[6] + m.e[9]) * fMult;
		break;
	case 3:
		q.z = fBiggestValue;
		q.w = (m.e[1] - m.e[4]) * fMult;
		q.x = (m.e[8] + m.e[2]) * fMult;
		q.y = (m.e[6] + m.e[9]) * fMult;
		break;
	}
}

void Math::BuildQuaternionFromEulerXYZ(Quaternion& q, float x, float y, float z)
{
	// from http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// 根据欧拉角计算四元数的变换如下(四元数的连接顺序是从右到左):
	// q = qz * qy * qx

	// 其中
	// qz = [ cos(z/2), [0, 0, sin(z/2)] ];
	// qy = [ cos(y/2), [0, sin(y/2), 0] ];
	// qx = [ cos(x/2), [sin(x/2), 0, 0] ];

	// 结果
	// q.w = [ cos(x/2)*cos(y/2)*cos(z/2) + sin(x/2)*sin(y/2)*sin(z/2) ]
	// q.x = [ sin(x/2)*cos(y/2)*cos(z/2) - cos(x/2)*sin(y/2)*sin(z/2) ]
	// q.y = [ cos(x/2)*sin(y/2)*cos(z/2) + sin(x/2)*cos(y/2)*sin(z/2) ]
	// q.z = [ cos(x/2)*cos(y/2)*sin(z/2) - sin(x/2)*sin(y/2)*cos(z/2) ]

	// 计算求四元数时使用到的所有三角值
	float cx = cosf(x/2.0f);
	float cy = cosf(y/2.0f);
	float cz = cosf(z/2.0f);
	float sx = sinf(x/2.0f);
	float sy = sinf(y/2.0f);
	float sz = sinf(z/2.0f);

	//组合这些值,生成四元数的向量和w
	q.w = cx*cy*cz + sx*sy*sz;
	q.x = sx*cy*cz - cx*sy*sz;
	q.y = cx*sy*cz + sx*cy*sz;
	q.z = cx*cy*sz - sx*sy*cz;
}
