/*!
 * \file HorizontalPatchStyle.h
 * \date 9-20-2012 14:20:55
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __HORIZONTALPATCHSTYLE_H__
#define __HORIZONTALPATCHSTYLE_H__

#include "IGraphicsStyle.h"
#include "../IUiRenderer.h"
#include <vector>

class HorizontalPatchStyle : public IGraphicsStyle
{
public:
	enum CONST_DEFINE
	{
		NUM_VERTS = 8,
		NUM_INDIS = 18,
	};

	typedef struct HORIZONTAL_PATCH_INFO_tag
	{
		uint nState;
		const PieceInfo* pPieceInfo;
		float pieceWidths[3];
		IUiRenderer::VERTEX_ATTR verts[NUM_VERTS];
	} HORIZONTAL_PATCH_INFO;

	typedef std::vector<HORIZONTAL_PATCH_INFO*> TV_HORIZONTAL_PATCH_INFO;

public:
	RTTI_DEF(HorizontalPatchStyle, IGraphicsStyle);

	HorizontalPatchStyle(const tstring& id);
	virtual ~HorizontalPatchStyle();

	virtual bool Render(const Vector2& pos, const Vector2& size, const Rect& clipRect, uint state);

	bool FromXml(TiXmlElement* pXmlHorizontalPatchStyle);
	TiXmlElement* ToXml();

private:
	bool RenderHorizontalPatchPiece(HORIZONTAL_PATCH_INFO& patchInfo, const Vector2& pos, const Vector2& size, const Rect& clipRect);

private:
	TV_HORIZONTAL_PATCH_INFO m_vHorizontalPatchInfo;

};
#endif // __HORIZONTALPATCHSTYLE_H__
