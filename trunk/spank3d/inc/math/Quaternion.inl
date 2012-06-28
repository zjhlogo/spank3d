/*!
 * \file Quaternion.inl
 * \date 26-5-2009 21:14:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __QUATERNION_INL__
#define __QUATERNION_INL__

#include <math.h>

inline Quaternion::Quaternion(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fz /*= 0.0f*/, float fw /*= 1.0f*/)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

// inline Quaternion::Quaternion(const Vector3& dir, float radian)
// {
// 	float fHalfRadian = radian/2.0f;
// 	float fSinTheta = sinf(fHalfRadian);
// 
// 	Vector3 dirNormalized = dir;
// 	dirNormalized.Normalize();
// 
// 	x = dirNormalized.x*fSinTheta;
// 	y = dirNormalized.y*fSinTheta;
// 	z = dirNormalized.z*fSinTheta;
// 	w = cosf(fHalfRadian);
// }

inline void Quaternion::Reset(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fz /*= 0.0f*/, float fw /*= 1.0f*/)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline float Quaternion::Length() const
{
	return sqrtf(x*x+y*y+z*z+w*w);
}

inline float Quaternion::SquaredLength() const
{
	return (x*x+y*y+z*z+w*w);
}

inline void Quaternion::Normalize()
{
	float fInvMag = 1.0f/Length();
	x *= fInvMag;
	y *= fInvMag;
	z *= fInvMag;
	w *= fInvMag;
}

inline float Quaternion::GetRadian() const
{
	return (2*acosf(w));
}

inline Quaternion& Quaternion::operator +=(const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

inline Quaternion& Quaternion::operator -=(const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

inline Quaternion& Quaternion::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

inline Quaternion& Quaternion::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

inline Quaternion Quaternion::operator -() const
{
	return Quaternion(-x, -y, -z, w);
}

inline Quaternion operator +(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1.x+q2.x, q1.y+q2.y, q1.z+q2.z, q1.w+q2.w);
}

inline Quaternion operator -(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1.x-q2.x, q1.y-q2.y, q1.z-q2.z, q1.w-q2.w);
}

inline Quaternion operator *(const Quaternion& q1, const Quaternion& q2)
{
	float prd_0 = (q1.z - q1.y) * (q2.y - q2.z);
	float prd_1 = (q1.w + q1.x) * (q2.w + q2.x);
	float prd_2 = (q1.w - q1.x) * (q2.y + q2.z);
	float prd_3 = (q1.y + q1.z) * (q2.w - q2.x);
	float prd_4 = (q1.z - q1.x) * (q2.x - q2.y);
	float prd_5 = (q1.z + q1.x) * (q2.x + q2.y);
	float prd_6 = (q1.w + q1.y) * (q2.w - q2.z);
	float prd_7 = (q1.w - q1.y) * (q2.w + q2.z);

	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5f * (prd_4 + prd_8);

	return Quaternion(prd_1 + prd_9 - prd_8,
					   prd_2 + prd_9 - prd_7,
					   prd_3 + prd_9 - prd_6,
					   prd_0 + prd_9 - prd_5);
}

// inline Quaternion operator *(const Quaternion& q, const Vector3& v)
// {
// 	return Quaternion(q.w*v.x + q.y*v.z - q.z*v.y,
// 		q.w*v.y + q.z*v.x - q.x*v.z,
// 		q.w*v.z + q.x*v.y - q.y*v.x,
// 		-(q.x*v.x + q.y*v.y + q.z*v.z));
// }
// 
// inline Quaternion operator *(const Vector3& v, const Quaternion& q)
// {
// 	return Quaternion(q.w*v.x + q.z*v.y - q.y*v.z,
// 		q.w*v.y + q.x*v.z - q.z*v.x,
// 		q.w*v.z + q.y*v.x - q.x*v.y,
// 		-(q.x*v.x + q.y*v.y + q.z*v.z));
// }

inline Quaternion operator *(float s, const Quaternion& q)
{
	return Quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

inline Quaternion operator *(const Quaternion& q, float s)
{
	return Quaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

inline Quaternion operator /(const Quaternion& q, float s)
{
	return Quaternion(q.x/s, q.y/s, q.z/s, q.w/s);
}

#endif // __QUATERNION_INL__
