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
	RTTI_DEF(VerticalPatchStyle, IGraphicsStyle);

	VerticalPatchStyle(const tstring& id);
	virtual ~VerticalPatchStyle();

};
#endif // __VERTICALPATCHSTYLE_H__
