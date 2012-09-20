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

class HorizontalPatchStyle : public IGraphicsStyle
{
public:
	RTTI_DEF(HorizontalPatchStyle, IGraphicsStyle);

	HorizontalPatchStyle(const tstring& id);
	virtual ~HorizontalPatchStyle();

};
#endif // __HORIZONTALPATCHSTYLE_H__
