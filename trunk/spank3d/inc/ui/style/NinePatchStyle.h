/*!
 * \file NinePatchStyle.h
 * \date 9-20-2012 8:56:54
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __NINEPATCHSTYLE_H__
#define __NINEPATCHSTYLE_H__

#include "IGraphicsStyle.h"
#include <tinyxml-2.6.2/tinyxml.h>

class NinePatchStyle : public IGraphicsStyle
{
public:
	typedef struct NINE_PATCH_PIECE_INFO_tag
	{
		uint nState;
		const PieceInfo* pPieceInfo;
		Vector2 patchSize[3];
		Vector2 uv[3];
		Vector2 dudv[3];
	} NINE_PATCH_PIECE_INFO;

	typedef std::vector<NINE_PATCH_PIECE_INFO> TV_NINE_PATCH_PIECE_INFO;

public:
	RTTI_DEF(NinePatchStyle, IGraphicsStyle);

	NinePatchStyle(const tstring& id);
	virtual ~NinePatchStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size, uint state);
	bool LoadFromXml(TiXmlElement* pXmlNinePatchStyle);

private:
	bool RenderNinePatchPiece(const NINE_PATCH_PIECE_INFO& stateInfo, const Vector2& pos, const Vector2& size);

private:
	TV_NINE_PATCH_PIECE_INFO m_vNinePatchPieceInfo;

};
#endif // __NINEPATCHSTYLE_H__
