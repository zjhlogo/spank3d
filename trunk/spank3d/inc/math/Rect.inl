/*!
 * \file Rect.inl
 * \date 8-13-2012 10:18:24
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */

inline Rect::Rect(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fwidth /*= 0.0f*/, float fheight /*= 0.0f*/)
{
	x = fx;
	y = fy;
	width = fwidth;
	height = fheight;
}

inline void Rect::Reset(float fx /*= 0.0f*/, float fy /*= 0.0f*/, float fwidth /*= 0.0f*/, float fheight /*= 0.0f*/)
{
	x = fx;
	y = fy;
	width = fwidth;
	height = fheight;
}
