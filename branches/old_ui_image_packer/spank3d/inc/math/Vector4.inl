/*!
 * \file Vector4.inl
 * \date 26-5-2009 21:48:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
inline Vector4::Vector4(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fz /*= 0.0f*/, float fw /*= 0.0f*/)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline void Vector4::Reset(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fz /*= 0.0f*/, float fw /*= 0.0f*/)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline float Vector4::Length() const
{
	return sqrtf(x*x+y*y+z*z+w*w);
}

inline float Vector4::LengthSquare() const
{
	return x*x+y*y+z*z+w*w;
}

inline void Vector4::Normalize()
{
	float fInvMag = 1.0f / sqrtf(x*x+y*y+z*z+w*w);
	x *= fInvMag;
	y *= fInvMag;
	z *= fInvMag;
	w *= fInvMag;
}

inline Vector4& Vector4::operator +=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline Vector4& Vector4::operator -=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline Vector4& Vector4::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

inline Vector4& Vector4::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

inline Vector4 Vector4::operator -() const
{
	return Vector4(-x, -y, -z, -w);
}

inline Vector4 operator *(const Vector4& v, float s)
{
	return Vector4(v.x*s, v.y*s, v.z*s, v.w*s);
}

inline Vector4 operator *(float s, const Vector4& v)
{
	return Vector4(v.x*s, v.y*s, v.z*s, v.w*s);
}
