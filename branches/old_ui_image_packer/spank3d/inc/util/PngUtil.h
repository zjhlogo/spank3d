/*!
 * \file PngUtil.h
 * \date 8-11-2012 14:17:15
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PNGUTIL_H__
#define __PNGUTIL_H__

#include "../core/IBitmapData.h"

class PngUtil
{
public:
	static IBitmapData* DecodePngFromFile(const tstring& strFullPath);

};
#endif // __PNGUTIL_H__
