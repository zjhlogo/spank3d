/*!
 * \file IUiResMgr.h
 * \date 9-20-2012 14:04:39
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IUIRESMGR_H__
#define __IUIRESMGR_H__

#include "../core/ISingleton.h"
#include "style/PieceInfo.h"
#include "style/BitmapStyle.h"
#include "style/NinePatchStyle.h"
#include "style/VerticalPatchStyle.h"
#include "style/HorizontalPatchStyle.h"

#include "style/IFontInfo.h"
#include "style/IFontStyle.h"

class IUiResMgr : public ISingleton
{
public:
	RTTI_DEF(IUiResMgr, ISingleton);

	IUiResMgr() {};
	virtual ~IUiResMgr() {};

	virtual PieceInfo* FindPieceInfo(const tstring& strId) = 0;

	virtual IGraphicsStyle* FindStyle(const tstring& strId) = 0;
	virtual BitmapStyle* FindBitmapStyle(const tstring& strId) = 0;
	virtual NinePatchStyle* FindNinePatchStyle(const tstring& strId) = 0;
	virtual VerticalPatchStyle* FindVerticalPatchStyle(const tstring& strId) = 0;
	virtual HorizontalPatchStyle* FindHorizontalPatchStyle(const tstring& strId) = 0;

	virtual IFontInfo* FindFontInfo(const tstring& strId) = 0;
	virtual IFontStyle* FindFontStyle(const tstring& strId) = 0;

};
#endif // __IUIRESMGR_H__
