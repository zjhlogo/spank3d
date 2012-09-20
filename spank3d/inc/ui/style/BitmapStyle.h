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
	RTTI_DEF(BitmapStyle, IGraphicsStyle);

	BitmapStyle(const tstring& id);
	virtual ~BitmapStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size);
	bool LoadFromXml(TiXmlElement* pXmlBitmapStyle);

private:

};
#endif // __BITMAPSTYLE_H__
