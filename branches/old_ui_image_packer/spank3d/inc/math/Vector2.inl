/*!
 * \file Vector2.inl
 * \date 8-13-2012 10:15:53
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */

inline Vector2::Vector2(float fx /*= 0.0f*/, float fy /*= 0.0f*/)
{
	x = fx;
	y = fy;
}

inline void Vector2::Reset(float fx /*= 0.0f*/, float fy /*= 0.0f*/)
{
	x = fx;
	y = fy;
}

inline float Vector2::Length() const
{
	return sqrtf(x*x+y*y);
}

inline float Vector2::LengthSquare() const
{
	return x*x+y*y;
}

inline Vector2& Vector2::Normalize()
{
	float fInvMag = 1.0f / sqrtf(x*x+y*y);
	x *= fInvMag;
	y *= fInvMag;
	return *this;
}

inline Vector2& Vector2::operator +=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

inline Vector2& Vector2::operator -=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

inline Vector2& Vector2::operator *=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

inline Vector2& Vector2::operator /=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

inline Vector2 Vector2::operator -() const
{
	return Vector2(-x, -y);
}

inline Vector2 operator +(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.x+v2.x, v1.y+v2.y);
}

inline Vector2 operator -(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.x-v2.x, v1.y-v2.y);
}

inline float operator *(const Vector2& v1, const Vector2& v2)
{
	return (v1.x*v2.x + v1.y*v2.y);
}

inline Vector2 operator *(float s, const Vector2& v)
{
	return Vector2(v.x*s, v.y*s);
}

inline Vector2 operator *(const Vector2& v, float s)
{
	return Vector2(v.x*s, v.y*s);
}

inline Vector2 operator /(const Vector2& v, float s)
{
	return Vector2(v.x/s, v.y/s);
}
