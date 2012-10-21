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
#include "../math/Vector2.h"

class ITexture : public IObject
{
public:
	enum TEXTURE_SAMPLE
	{
		TS_NEAREST = 1,
		TS_LINEAR,
	};

public:
	RTTI_DEF(ITexture, IObject);

	ITexture() {};
	virtual ~ITexture() {};

	virtual const tstring& GetId() const = 0;
	virtual const Vector2& GetSize() const = 0;

};
#endif // __ITEXTURE_H__
