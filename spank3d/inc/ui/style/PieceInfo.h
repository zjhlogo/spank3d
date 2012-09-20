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
#include <tinyxml-2.6.2/tinyxml.h>

class PieceInfo
{
public:
	PieceInfo(const tstring& id);
	~PieceInfo();

	const tstring& GetId() const;

	bool LoadFromXml(TiXmlElement* pXmlPieceInfo);

public:
	int x;
	int y;
	int width;
	int height;

	float u;
	float v;
	float du;
	float dv;

	ITexture* pTexture;

private:
	tstring strId;

};
#endif // __PIECEINFO_H__
