/*!
 * \file Matrix4x4.inl
 * \date 26-5-2009 21:14:14
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
inline Matrix4x4::Matrix4x4(float fe00 /* = 1.0f */, float fe01 /* = 0.0f */, float fe02 /* = 0.0f */, float fe03 /* = 0.0f */,
							float fe10 /* = 0.0f */, float fe11 /* = 1.0f */, float fe12 /* = 0.0f */, float fe13 /* = 0.0f */,
							float fe20 /* = 0.0f */, float fe21 /* = 0.0f */, float fe22 /* = 1.0f */, float fe23 /* = 0.0f */,
							float fe30 /* = 0.0f */, float fe31 /* = 0.0f */, float fe32 /* = 0.0f */, float fe33 /* = 1.0f */)
{
	e[0]  = fe00; e[1]  = fe01; e[2]  = fe02; e[3]  = fe03;
	e[4]  = fe10; e[5]  = fe11; e[6]  = fe12; e[7]  = fe13;
	e[8]  = fe20; e[9]  = fe21; e[10] = fe22; e[11] = fe23;
	e[12] = fe30; e[13] = fe31; e[14] = fe32; e[15] = fe33;
}

inline void Matrix4x4::Reset(float fe00 /* = 1.0f */, float fe01 /* = 0.0f */, float fe02 /* = 0.0f */, float fe03 /* = 0.0f */,
							 float fe10 /* = 0.0f */, float fe11 /* = 1.0f */, float fe12 /* = 0.0f */, float fe13 /* = 0.0f */,
							 float fe20 /* = 0.0f */, float fe21 /* = 0.0f */, float fe22 /* = 1.0f */, float fe23 /* = 0.0f */,
							 float fe30 /* = 0.0f */, float fe31 /* = 0.0f */, float fe32 /* = 0.0f */, float fe33 /* = 1.0f */)
{
	e[0]  = fe00; e[1]  = fe01; e[2]  = fe02; e[3]  = fe03;
	e[4]  = fe10; e[5]  = fe11; e[6]  = fe12; e[7]  = fe13;
	e[8]  = fe20; e[9]  = fe21; e[10] = fe22; e[11] = fe23;
	e[12] = fe30; e[13] = fe31; e[14] = fe32; e[15] = fe33;
}

inline float Matrix4x4::Determinant() const
{
	float fA0 = e[0] * e[5] - e[1] * e[4];
	float fA1 = e[0] * e[6] - e[2] * e[4];
	float fA2 = e[0] * e[7] - e[3] * e[4];
	float fA3 = e[1] * e[6] - e[2] * e[5];
	float fA4 = e[1] * e[7] - e[3] * e[5];
	float fA5 = e[2] * e[7] - e[3] * e[6];

	float fB0 = e[8]  * e[13] - e[9]  * e[12];
	float fB1 = e[8]  * e[14] - e[10] * e[12];
	float fB2 = e[8]  * e[15] - e[11] * e[12];
	float fB3 = e[9]  * e[14] - e[10] * e[13];
	float fB4 = e[9]  * e[15] - e[11] * e[13];
	float fB5 = e[10] * e[15] - e[11] * e[14];

	return fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
}

inline void Matrix4x4::Transpose()
{
	float temp = e[1];
	e[1] = e[4];
	e[4] = temp;

	temp = e[2];
	e[2] = e[8];
	e[8] = temp;

	temp = e[3];
	e[3] = e[12];
	e[12] = temp;

	temp = e[6];
	e[6] = e[9];
	e[9] = temp;

	temp = e[7];
	e[7] = e[13];
	e[13] = temp;

	temp = e[11];
	e[11] = e[14];
	e[14] = temp;
}

inline bool Matrix4x4::Invert()
{
	float fA0 = e[0]*e[5] - e[1]*e[4];
	float fA1 = e[0]*e[6] - e[2]*e[4];
	float fA2 = e[0]*e[7] - e[3]*e[4];
	float fA3 = e[1]*e[6] - e[2]*e[5];
	float fA4 = e[1]*e[7] - e[3]*e[5];
	float fA5 = e[2]*e[7] - e[3]*e[6];

	float fB0 = e[8]*e[13] - e[9]*e[12];
	float fB1 = e[8]*e[14] - e[10]*e[12];
	float fB2 = e[8]*e[15] - e[11]*e[12];
	float fB3 = e[9]*e[14] - e[10]*e[13];
	float fB4 = e[9]*e[15] - e[11]*e[13];
	float fB5 = e[10]*e[15] - e[11]*e[14];

	float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;

	if (fabs(fDet) <= 1E-5f) return false;

	float e0  =  e[5]*fB5 - e[6]*fB4 + e[7]*fB3;
	float e4  = -e[4]*fB5 + e[6]*fB2 - e[7]*fB1;
	float e8  =  e[4]*fB4 - e[5]*fB2 + e[7]*fB0;
	float e12 = -e[4]*fB3 + e[5]*fB1 - e[6]*fB0;

	float e1  = -e[1]*fB5 + e[2]*fB4 - e[3]*fB3;
	float e5  =  e[0]*fB5 - e[2]*fB2 + e[3]*fB1;
	float e9  = -e[0]*fB4 + e[1]*fB2 - e[3]*fB0;
	float e13 =  e[0]*fB3 - e[1]*fB1 + e[2]*fB0;

	float e2  =  e[13]*fA5 - e[14]*fA4 + e[15]*fA3;
	float e6  = -e[12]*fA5 + e[14]*fA2 - e[15]*fA1;
	float e10 =  e[12]*fA4 - e[13]*fA2 + e[15]*fA0;
	float e14 = -e[12]*fA3 + e[13]*fA1 - e[14]*fA0;

	float e3  = -e[9]*fA5 + e[10]*fA4 - e[11]*fA3;
	float e7  =  e[8]*fA5 - e[10]*fA2 + e[11]*fA1;
	float e11 = -e[8]*fA4 + e[9]*fA2  - e[11]*fA0;
	float e15 =  e[8]*fA3 - e[9]*fA1  + e[10]*fA0;

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
	e[9] = e9 * fInvDet;
	e[10] = e10 * fInvDet;
	e[11] = e11 * fInvDet;

	e[12] = e12 * fInvDet;
	e[13] = e13 * fInvDet;
	e[14] = e14 * fInvDet;
	e[15] = e15 * fInvDet;

	return true;
}

inline Matrix4x4& Matrix4x4::operator +=(const Matrix4x4& m)
{
	e[0]  += m.e[0];  e[1]  += m.e[1];  e[2]  += m.e[2];  e[3]  += m.e[3];
	e[4]  += m.e[4];  e[5]  += m.e[5];  e[6]  += m.e[6];  e[7]  += m.e[7];
	e[8]  += m.e[8];  e[9]  += m.e[9];  e[10] += m.e[10]; e[11] += m.e[11];
	e[12] += m.e[12]; e[13] += m.e[13]; e[14] += m.e[14]; e[15] += m.e[15];
	return *this;
}

inline Matrix4x4& Matrix4x4::operator -=(const Matrix4x4& m)
{
	e[0]  -= m.e[0];  e[1]  -= m.e[1];  e[2]  -= m.e[2];  e[3]  -= m.e[3];
	e[4]  -= m.e[4];  e[5]  -= m.e[5];  e[6]  -= m.e[6];  e[7]  -= m.e[7];
	e[8]  -= m.e[8];  e[9]  -= m.e[9];  e[10] -= m.e[10]; e[11] -= m.e[11];
	e[12] -= m.e[12]; e[13] -= m.e[13]; e[14] -= m.e[14]; e[15] -= m.e[15];
	return *this;
}

inline Matrix4x4& Matrix4x4::operator *=(float s)
{
	e[0]  *= s; e[1]  *= s; e[2]  *= s; e[3]  *= s;
	e[4]  *= s; e[5]  *= s; e[6]  *= s; e[7]  *= s;
	e[8]  *= s; e[9]  *= s; e[10] *= s; e[11] *= s;
	e[12] *= s; e[13] *= s; e[14] *= s; e[15] *= s;
	return *this;
}

inline Matrix4x4& Matrix4x4::operator *=(const Matrix4x4& m)
{
	*this = (*this)*m;
	return *this;
}

inline Matrix4x4& Matrix4x4::operator /=(float s)
{
	e[0]  /= s; e[1]  /= s; e[2]  /= s; e[3]  /= s;
	e[4]  /= s; e[5]  /= s; e[6]  /= s; e[7]  /= s;
	e[8]  /= s; e[9]  /= s; e[10] /= s; e[11] /= s;
	e[12] /= s; e[13] /= s; e[14] /= s; e[15] /= s;
	return *this;
}

inline Matrix4x4 operator +(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Matrix4x4(m1.e[0] + m2.e[0], m1.e[1] + m2.e[1], m1.e[2] + m2.e[2], m1.e[3] + m2.e[3],
					 m1.e[4] + m2.e[4], m1.e[5] + m2.e[5], m1.e[6] + m2.e[6], m1.e[7] + m2.e[7],
					 m1.e[8] + m2.e[8], m1.e[9] + m2.e[9], m1.e[10] + m2.e[10], m1.e[11] + m2.e[11],
					 m1.e[12] + m2.e[12], m1.e[13] + m2.e[13], m1.e[14] + m2.e[14], m1.e[15] + m2.e[15]);
}

inline Matrix4x4 operator -(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Matrix4x4(m1.e[0] - m2.e[0], m1.e[1] - m2.e[1], m1.e[2] - m2.e[2], m1.e[3] - m2.e[3],
					 m1.e[4] - m2.e[4], m1.e[5] - m2.e[5], m1.e[6] - m2.e[6], m1.e[7] - m2.e[7],
					 m1.e[8] - m2.e[8], m1.e[9] - m2.e[9], m1.e[10] - m2.e[10], m1.e[11] - m2.e[11],
					 m1.e[12] - m2.e[12], m1.e[13] - m2.e[13], m1.e[14] - m2.e[14], m1.e[15] - m2.e[15]);
}

inline Matrix4x4 operator *(const Matrix4x4& m, float s)
{
	return Matrix4x4(m.e[0] * s, m.e[1] * s, m.e[2] * s, m.e[3] * s,
					 m.e[4] * s, m.e[5] * s, m.e[6] * s, m.e[7] * s,
					 m.e[8] * s, m.e[9] * s, m.e[10] * s, m.e[11] * s,
					 m.e[12] * s, m.e[13] * s, m.e[14] * s, m.e[15] * s);
}

inline Matrix4x4 operator *(const Matrix4x4& m1, const Matrix4x4& m2)
{
	return Matrix4x4(m1.e[0]*m2.e[0] + m1.e[4]*m2.e[1] + m1.e[8]*m2.e[2] + m1.e[12]*m2.e[3],
					 m1.e[1]*m2.e[0] + m1.e[5]*m2.e[1] + m1.e[9]*m2.e[2] + m1.e[13]*m2.e[3],
					 m1.e[2]*m2.e[0] + m1.e[6]*m2.e[1] + m1.e[10]*m2.e[2] + m1.e[14]*m2.e[3],
					 m1.e[3]*m2.e[0] + m1.e[7]*m2.e[1] + m1.e[11]*m2.e[2] + m1.e[15]*m2.e[3],

					 m1.e[0]*m2.e[4] + m1.e[4]*m2.e[5] + m1.e[8]*m2.e[6] + m1.e[12]*m2.e[7],
					 m1.e[1]*m2.e[4] + m1.e[5]*m2.e[5] + m1.e[9]*m2.e[6] + m1.e[13]*m2.e[7],
					 m1.e[2]*m2.e[4] + m1.e[6]*m2.e[5] + m1.e[10]*m2.e[6] + m1.e[14]*m2.e[7],
					 m1.e[3]*m2.e[4] + m1.e[7]*m2.e[5] + m1.e[11]*m2.e[6] + m1.e[15]*m2.e[7],

					 m1.e[0]*m2.e[8] + m1.e[4]*m2.e[9] + m1.e[8]*m2.e[10] + m1.e[12]*m2.e[11],
					 m1.e[1]*m2.e[8] + m1.e[5]*m2.e[9] + m1.e[9]*m2.e[10] + m1.e[13]*m2.e[11],
					 m1.e[2]*m2.e[8] + m1.e[6]*m2.e[9] + m1.e[10]*m2.e[10] + m1.e[14]*m2.e[11],
					 m1.e[3]*m2.e[8] + m1.e[7]*m2.e[9] + m1.e[11]*m2.e[10] + m1.e[15]*m2.e[11],

					 m1.e[0]*m2.e[12] + m1.e[4]*m2.e[13] + m1.e[8]*m2.e[14] + m1.e[12]*m2.e[15],
					 m1.e[1]*m2.e[12] + m1.e[5]*m2.e[13] + m1.e[9]*m2.e[14] + m1.e[13]*m2.e[15],
					 m1.e[2]*m2.e[12] + m1.e[6]*m2.e[13] + m1.e[10]*m2.e[14] + m1.e[14]*m2.e[15],
					 m1.e[3]*m2.e[12] + m1.e[7]*m2.e[13] + m1.e[11]*m2.e[14] + m1.e[15]*m2.e[15]);
}

inline Matrix4x4 operator /(const Matrix4x4& m, float s)
{
	return Matrix4x4(m.e[0] / s, m.e[1] / s, m.e[2] / s, m.e[3] / s,
					 m.e[4] / s, m.e[5] / s, m.e[6] / s, m.e[7] / s,
					 m.e[8] / s, m.e[9] / s, m.e[10] / s, m.e[11] / s,
					 m.e[12] / s, m.e[13] / s, m.e[14] / s, m.e[15] / s);
}
