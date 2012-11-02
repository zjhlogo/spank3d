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
#include "../render/RenderTypes.h"

class TextureUtil
{
public:
	static IBitmapData* DecodePngFromFile(const tstring& strFullPath);
	static bool IsValidTextureSize(uint width, uint height);
	static uint Bpp2TexFormat(uint bpp);

};

#endif // __TEXTUREUTIL_H__
