/*!
 * \file PieceInfo.h
 * \date 9-20-2012 14:08:04
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PIECEINFO_H__
#define __PIECEINFO_H__

#include "../../type/BaseType.h"
#include "../../render/ITexture.h"

class PieceInfo
{
public:
	PieceInfo();
	~PieceInfo();

public:
	uint x;
	uint y;
	uint width;
	uint height;

	float u;
	float v;
	float du;
	float dv;

	ITexture* pTexture;
};
#endif // __PIECEINFO_H__
