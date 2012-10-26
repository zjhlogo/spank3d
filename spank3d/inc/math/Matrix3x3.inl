/*!
 * \file Matrix3x3.inl
 * \date 10-26-2012 22:58:02
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
inline Matrix3x3::Matrix3x3(float fe00 /* = 1.0f */, float fe01 /* = 0.0f */, float fe02 /* = 0.0f */,
							float fe10 /* = 0.0f */, float fe11 /* = 1.0f */, float fe12 /* = 0.0f */,
							float fe20 /* = 0.0f */, float fe21 /* = 0.0f */, float fe22 /* = 1.0f */)
{
	e[0] = fe00; e[1] = fe01; e[2] = fe02;
	e[3] = fe10; e[4] = fe11; e[5] = fe12;
	e[6] = fe20; e[7] = fe21; e[8] = fe22;
}

inline void Matrix3x3::Reset(float fe00 /* = 1.0f */, float fe01 /* = 0.0f */, float fe02 /* = 0.0f */,
							 float fe10 /* = 0.0f */, float fe11 /* = 1.0f */, float fe12 /* = 0.0f */,
							 float fe20 /* = 0.0f */, float fe21 /* = 0.0f */, float fe22 /* = 1.0f */)
{
	e[0] = fe00; e[1] = fe01; e[2] = fe02;
	e[3] = fe10; e[4] = fe11; e[5] = fe12;
	e[6] = fe20; e[7] = fe21; e[8] = fe22;
}

inline float Matrix3x3::Determinant() const
{
	float fCo00 = e[4]*e[8] - e[5]*e[7];
	float fCo10 = e[5]*e[6] - e[3]*e[8];
	float fCo20 = e[3]*e[7] - e[4]*e[6];
	return e[0]*fCo00 + e[1]*fCo10 + e[2]*fCo20;
}

inline void Matrix3x3::Transpose()
{
	float temp = e[3];
	e[3] = e[1];
	e[1] = temp;

	temp = e[2];
	e[2] = e[6];
	e[6] = temp;

	temp = e[5];
	e[5] = e[7];
	e[7] = temp;
}

inline bool Matrix3x3::Invert()
{
	float fCo00 = e[4]*e[8] - e[5]*e[7];
	float fCo10 = e[5]*e[6] - e[3]*e[8];
	float fCo20 = e[3]*e[7] - e[4]*e[6];
	float fDet = e[0]*fCo00 + e[1]*fCo10 + e[2]*fCo20;

	if (fabs(fDet) <= 1E-5f) return false;

	Matrix3x3 matInv;

	float e0 = e[4]*e[8] - e[5]*e[7];
	float e1 = e[2]*e[7] - e[1]*e[8];
	float e2 = e[1]*e[5] - e[2]*e[4];
	float e3 = e[5]*e[6] - e[3]*e[8];
	float e4 = e[0]*e[8] - e[2]*e[6];
	float e5 = e[2]*e[3] - e[0]*e[5];
	float e6 = e[3]*e[7] - e[4]*e[6];
	float e7 = e[1]*e[6] - e[0]*e[7];
	float e8 = e[0]*e[4] - e[1]*e[3];

	float fInvDet = 1.0f/fDet;

	e[0] = e0 * fInvDet;
	e[1] = e1 * fInvDet;
	e[2] = e2 * fInvDet;
	e[3] = e3 * fInvDet;
	e[4] = e4 * fInvDet;
	e[5] = e5 * fInvDet;
	e[6] = e6 * fInvDet;
	e[7] = e7 * fInvDet;
	e[8] = e8 * fInvDet;

	return true;
}

inline Matrix3x3& Matrix3x3::operator +=(const Matrix3x3& m)
{
	e[0] += m.e[0]; e[1] += m.e[1]; e[2] += m.e[2];
	e[3] += m.e[3]; e[4] += m.e[4]; e[5] += m.e[5];
	e[6] += m.e[6]; e[7] += m.e[7]; e[8] += m.e[8];
	return *this;
}

inline Matrix3x3& Matrix3x3::operator -=(const Matrix3x3& m)
{
	e[0] -= m.e[0]; e[1] -= m.e[1]; e[2] -= m.e[2];
	e[3] -= m.e[3]; e[4] -= m.e[4]; e[5] -= m.e[5];
	e[6] -= m.e[6]; e[7] -= m.e[7]; e[8] -= m.e[8];
	return *this;
}

inline Matrix3x3& Matrix3x3::operator *=(float s)
{
	e[0] *= s; e[1] *= s; e[2] *= s;
	e[3] *= s; e[4] *= s; e[5] *= s;
	e[6] *= s; e[7] *= s; e[8] *= s;
	return *this;
}

inline Matrix3x3& Matrix3x3::operator *=(const Matrix3x3& m)
{
	*this = (*this)*m;
	return *this;
}

inline Matrix3x3& Matrix3x3::operator /=(float s)
{
	e[0] /= s; e[1] /= s; e[2] /= s;
	e[3] /= s; e[4] /= s; e[5] /= s;
	e[6] /= s; e[7] /= s; e[8] /= s;
	return *this;
}

inline Matrix3x3 operator +(const Matrix3x3& m1, const Matrix3x3& m2)
{
	return Matrix3x3(m1.e[0] + m2.e[0], m1.e[1] + m2.e[1], m1.e[2] + m2.e[2],
					 m1.e[3] + m2.e[3], m1.e[4] + m2.e[4], m1.e[5] + m2.e[5],
					 m1.e[6] + m2.e[6], m1.e[7] + m2.e[7], m1.e[8] + m2.e[8]);
}

inline Matrix3x3 operator -(const Matrix3x3& m1, const Matrix3x3& m2)
{
	return Matrix3x3(m1.e[0] - m2.e[0], m1.e[1] - m2.e[1], m1.e[2] - m2.e[2],
					 m1.e[3] - m2.e[3], m1.e[4] - m2.e[4], m1.e[5] - m2.e[5],
					 m1.e[6] - m2.e[6], m1.e[7] - m2.e[7], m1.e[8] - m2.e[8]);
}

inline Matrix3x3 operator *(const Matrix3x3& m, float s)
{
	return Matrix3x3(m.e[0] * s, m.e[1] * s, m.e[2] * s,
					 m.e[3] * s, m.e[4] * s, m.e[5] * s,
					 m.e[6] * s, m.e[7] * s, m.e[8] * s);
}

inline Matrix3x3 operator *(const Matrix3x3& m1, const Matrix3x3& m2)
{
	return Matrix3x3(m1.e[0]*m2.e[0] + m1.e[3]*m2.e[1] + m1.e[6]*m2.e[2],
					 m1.e[1]*m2.e[0] + m1.e[4]*m2.e[1] + m1.e[7]*m2.e[2],
					 m1.e[2]*m2.e[0] + m1.e[5]*m2.e[1] + m1.e[8]*m2.e[2],

					 m1.e[0]*m2.e[3] + m1.e[3]*m2.e[4] + m1.e[6]*m2.e[5],
					 m1.e[1]*m2.e[3] + m1.e[4]*m2.e[4] + m1.e[7]*m2.e[5],
					 m1.e[2]*m2.e[3] + m1.e[5]*m2.e[4] + m1.e[8]*m2.e[5],

					 m1.e[0]*m2.e[6] + m1.e[3]*m2.e[7] + m1.e[6]*m2.e[8],
					 m1.e[1]*m2.e[6] + m1.e[4]*m2.e[7] + m1.e[7]*m2.e[8],
					 m1.e[2]*m2.e[6] + m1.e[5]*m2.e[7] + m1.e[8]*m2.e[8]);
}

inline Matrix3x3 operator /(const Matrix3x3& m, float s)
{
	return Matrix3x3(m.e[0] / s, m.e[1] / s, m.e[2] / s,
					 m.e[3] / s, m.e[4] / s, m.e[5] / s,
					 m.e[6] / s, m.e[7] / s, m.e[8] / s);
}
