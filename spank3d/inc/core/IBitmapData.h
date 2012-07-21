/*!
 * \file IBitmapData.h
 * \date 7-21-2012 16:25:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IBITMAPDATA_H__
#define __IBITMAPDATA_H__

#include "IObject.h"

class IBitmapData : public IObject
{
public:
	RTTI_DEF(IBitmapData, IObject);

	IBitmapData() {};
	virtual ~IBitmapData() {};

	virtual uint GetWidth() const = 0;
	virtual uint GetHeight() const = 0;
	virtual uint GetBPP() const = 0;

	virtual void* GetData() = 0;
	virtual const void* GetData() const = 0;

};
#endif // __IBITMAPDATA_H__
