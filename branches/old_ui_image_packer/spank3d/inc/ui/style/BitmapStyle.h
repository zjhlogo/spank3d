/*!
 * \file BitmapStyle.h
 * \date 9-20-2012 8:42:16
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BITMAPSTYLE_H__
#define __BITMAPSTYLE_H__

#include "IGraphicsStyle.h"
#include <tinyxml-2.6.2/tinyxml.h>

class BitmapStyle : public IGraphicsStyle
{
public:
	typedef struct BITMAP_INFO_tag
	{
		uint nState;
		const PieceInfo* pPieceInfo;
	} BITMAP_INFO;

	typedef std::vector<BITMAP_INFO*> TV_BITMAP_INFO;

public:
	RTTI_DEF(BitmapStyle, IGraphicsStyle);

	BitmapStyle(const tstring& id);
	virtual ~BitmapStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size, const Rect& clipRect, uint state);
	bool LoadFromXml(TiXmlElement* pXmlBitmapStyle);

private:
	bool RenderBitmapPiece(const BITMAP_INFO& bitmapInfo, const Vector2& pos, const Vector2& size, const Rect& clipRect);

private:
	TV_BITMAP_INFO m_vBitmapInfo;

};
#endif // __BITMAPSTYLE_H__
