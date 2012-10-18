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

inline Rect Rect::Intersect(const Rect& rect) const
{
	float left = x > rect.x ? x : rect.x;
	float top = y > rect.y ? y : rect.y;
	float right = x+width < rect.x+rect.width ? x+width : rect.x+rect.width;
	float bottom = y+height < rect.y+rect.height ? y+height : rect.y+rect.height;

	return Rect(left, top, right - left, bottom - top);
}

inline Rect Rect::Intersect(const Vector2& pos, const Vector2& size) const
{
	float left = x > pos.x ? x : pos.x;
	float top = y > pos.y ? y : pos.y;
	float right = x+width < pos.x+size.x ? x+width : pos.x+size.x;
	float bottom = y+height < pos.y+size.y ? y+height : pos.y+size.y;

	return Rect(left, top, right - left, bottom - top);
}
