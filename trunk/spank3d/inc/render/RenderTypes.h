/*!
 * \file RenderTypes.h
 * \date 11-02-2012 9:22:07
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RENDERTYPES_H__
#define __RENDERTYPES_H__

#include "../type/BaseType.h"

class TEXTURE_TYPE
{
public:
	static const uint TT_TEXTURE_1D;
	static const uint TT_TEXTURE_2D;
	static const uint TT_TEXTURE_3D;
	static const uint TT_TEXTURE_1D_ARRAY;
	static const uint TT_TEXTURE_2D_ARRAY;
	static const uint TT_TEXTURE_RECTANGLE;
	static const uint TT_TEXTURE_CUBE_MAP;
};

class TEXTURE_FORMAT
{
public:
	static const uint TF_DEPTH;
	static const uint TF_STENCIL;
	static const uint TF_R;
	static const uint TF_RG;
	static const uint TF_RGB;
	static const uint TF_RGBA;
};

class TEXTURE_FILTER
{
public:
	static const uint TF_NEAREST;
	static const uint TF_LINEAR;

};
#endif // __RENDERTYPES_H__
