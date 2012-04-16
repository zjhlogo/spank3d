/*!
 * \file Vector3.inl
 * \date 26-5-2009 21:15:59
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */

inline Vector3::Vector3(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fz /*= 0.0f*/)
{
	x = fx;
	y = fy;
	z = fz;
}

inline void Vector3::Reset(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fz /*= 0.0f*/)
{
	x = fx;
	y = fy;
	z = fz;
}

inline float Vector3::Length() const
{
	return sqrtf(x*x+y*y+z*z);
}

inline float Vector3::LengthSquare() const
{
	return x*x+y*y+z*z;
}

inline Vector3& Vector3::Normalize()
{
	float fInvMag = 1.0f / sqrtf(x*x+y*y+z*z);
	x *= fInvMag;
	y *= fInvMag;
	z *= fInvMag;
	return *this;
}

inline Vector3& Vector3::operator +=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vector3& Vector3::operator -=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vector3& Vector3::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline Vector3& Vector3::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 operator +(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

inline Vector3 operator -(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

inline float operator *(const Vector3& v1, const Vector3& v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

inline Vector3 operator *(float s, const Vector3& v)
{
	return Vector3(v.x*s, v.y*s, v.z*s);
}

inline Vector3 operator *(const Vector3& v, float s)
{
	return Vector3(v.x*s, v.y*s, v.z*s);
}

inline Vector3 operator /(const Vector3& v, float s)
{
	return Vector3(v.x/s, v.y/s, v.z/s);
}

inline Vector3 operator ^(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.y*v2.z-v1.z*v2.y, -v1.x*v2.z+v1.z*v2.x, v1.x*v2.y-v1.y*v2.x);
}
