/*!
 * \file VerticalPatchStyle.h
 * \date 9-20-2012 14:19:24
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __VERTICALPATCHSTYLE_H__
#define __VERTICALPATCHSTYLE_H__

#include "IGraphicsStyle.h"

class VerticalPatchStyle : public IGraphicsStyle
{
public:
	enum CONST_DEFINE
	{
		NUM_VERTS = 8,
		NUM_INDIS = 18,
	};

	typedef struct VERTEX_ATTR_tag
	{
		float x, y;
		float u, v;
	} VERTEX_ATTR;

	typedef struct VERTICAL_PATCH_INFO_tag
	{
		uint nState;
		const PieceInfo* pPieceInfo;
		float pieceHeights[3];
		VERTEX_ATTR verts[NUM_VERTS];
	} VERTICAL_PATCH_INFO;

	typedef std::vector<VERTICAL_PATCH_INFO*> TV_VERTICAL_PATCH_INFO;


public:
	RTTI_DEF(VerticalPatchStyle, IGraphicsStyle);

	VerticalPatchStyle(const tstring& id);
	virtual ~VerticalPatchStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size, uint state);
	bool LoadFromXml(TiXmlElement* pXmlVerticalPatchStyle);

private:
	bool RenderVerticalPatchPiece(VERTICAL_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size);

private:
	TV_VERTICAL_PATCH_INFO m_vVerticalPatchInfo;

};
#endif // __VERTICALPATCHSTYLE_H__
