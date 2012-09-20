/*!
 * \file BitmapStyle.cpp
 * \date 9-20-2012 8:46:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/style/BitmapStyle.h>

BitmapStyle::BitmapStyle(const tstring& id)
:IGraphicsStyle(id)
{
	// TODO: 
}

BitmapStyle::~BitmapStyle()
{
	// TODO: 
}

bool BitmapStyle::Render(const Vector2& pos, const Vector2& size, uint state)
{
	return true;
}

bool BitmapStyle::LoadFromXml(TiXmlElement* pXmlBitmapStyle)
{
	// TODO: 
	return false;
}
