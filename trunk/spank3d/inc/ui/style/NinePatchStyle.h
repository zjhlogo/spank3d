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

class NinePatchStyle : public IGraphicsStyle
{
public:
	enum CONST_DEFINE
	{
		NUM_VERTS = 16,
		NUM_INDIS = 54,
		NUM_PIECE_SIZE = 3,
	};

	typedef struct VERTEX_ATTR_tag
	{
		float x, y;
		float u, v;
	} VERTEX_ATTR;

	typedef struct NINE_PATCH_INFO_tag
	{
		uint nState;
		const PieceInfo* pPieceInfo;
		Vector2 pieceSize[NUM_PIECE_SIZE];
		VERTEX_ATTR verts[NUM_VERTS];
	} NINE_PATCH_INFO;

	typedef std::vector<NINE_PATCH_INFO*> TV_NINE_PATCH_INFO;

public:
	RTTI_DEF(NinePatchStyle, IGraphicsStyle);

	NinePatchStyle(const tstring& id);
	virtual ~NinePatchStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size, uint state);
	bool LoadFromXml(TiXmlElement* pXmlNinePatchStyle);

private:
	bool RenderNinePatchPiece(NINE_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size);

private:
	TV_NINE_PATCH_INFO m_vNinePatchInfo;

};
#endif // __NINEPATCHSTYLE_H__
