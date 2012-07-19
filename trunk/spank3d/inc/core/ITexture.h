/*!
 * \file ITexture.h
 * \date 7-19-2012 14:12:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

#include "IObject.h"
#include "../math/Vector2i.h"

class ITexture : public IObject
{
public:
	RTTI_DEF(ITexture, IObject);

	ITexture() {};
	virtual ~ITexture() {};

	virtual const Vector2i& GetSize() const = 0;

};
#endif // __ITEXTURE_H__
