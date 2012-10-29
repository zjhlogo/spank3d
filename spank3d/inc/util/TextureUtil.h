/*!
 * \file TextureUtil.h
 * \date 10-29-2012 10:28:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TEXTUREUTIL_H__
#define __TEXTUREUTIL_H__

#include "../core/IBitmapData.h"

class TextureUtil
{
public:
	static IBitmapData* DecodePngFromFile(const tstring& strFullPath);
	static bool IsValidTextureSize(uint width, uint height);

};
#endif // __TEXTUREUTIL_H__
