/*!
 * \file ITexture.h
 * \date 7-19-2012 14:12:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ITEXTURE_H__
#define __ITEXTURE_H__

#include "../core/IObject.h"

class ITexture : public IObject
{
public:
	RTTI_DEF(ITexture, IObject);

	ITexture() {};
	virtual ~ITexture() {};

	virtual uint GetWidth() const = 0;
	virtual uint GetHeight() const = 0;

};
#endif // __ITEXTURE_H__
