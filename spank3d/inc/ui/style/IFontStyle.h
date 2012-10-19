/*!
 * \file IFontStyle.h
 * \date 9-26-2012 17:00:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IFONTSTYLE_H__
#define __IFONTSTYLE_H__

#include "IStyle.h"
#include "../../render/ITexture.h"
#include "../../math/Rect.h"

class IFontStyle : public IStyle
{
public:
	typedef struct CHAR_INFO_tag
	{
		int id;
		int x;
		int y;
		int width;
		int height;
		float u;
		float v;
		float du;
		float dv;
		ITexture* pTexture;
		Vector2 offset;
		float advance;
	} CHAR_INFO;
	typedef std::map<int, CHAR_INFO> TM_CHAR_INFO;

public:
	RTTI_DEF(IFontStyle, IStyle);

	IFontStyle(const tstring& id):IStyle(id) {};
	virtual ~IFontStyle() {};

	virtual float GetLineHeight() const = 0;
	virtual Vector2 CalcSize(const tstring& strText, uint state) = 0;
	virtual bool Render(const tstring& strText, const Vector2& pos, const Rect& clipRect, uint state) = 0;
};
#endif // __IFONTSTYLE_H__
