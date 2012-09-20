/*!
 * \file NinePatchStyle.h
 * \date 9-20-2012 8:56:54
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __NINEPATCHSTYLE_H__
#define __NINEPATCHSTYLE_H__

#include "IGraphicsStyle.h"
#include <tinyxml-2.6.2/tinyxml.h>

class NinePatchStyle : public IGraphicsStyle
{
public:
	RTTI_DEF(NinePatchStyle, IGraphicsStyle);

	NinePatchStyle(const tstring& id);
	virtual ~NinePatchStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size);
	bool LoadFromXml(TiXmlElement* pXmlNinePatchStyle);

private:
	TV_STATE_INFO m_vStateInfo;

};
#endif // __NINEPATCHSTYLE_H__
