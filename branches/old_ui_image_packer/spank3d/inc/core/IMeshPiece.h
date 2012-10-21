/*!
 * \file IMeshPiece.h
 * \date 4-19-2012 22:41:35
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMESHPIECE_H__
#define __IMESHPIECE_H__

#include "IObject.h"

class IMeshPiece : public IObject
{
public:
	RTTI_DEF(IMeshPiece, IObject);

	IMeshPiece() {};
	virtual ~IMeshPiece() {};

	virtual const tstring& GetName() const = 0;

	virtual int GetMaterialId() const = 0;
	virtual int GetVertDeclMask() const = 0;

	virtual int GetNumVerts() const = 0;
	virtual int GetNumIndis() const = 0;
	virtual void* GetVerts() const = 0;
	virtual ushort* GetIndis() const = 0;
};

#endif // __IMESHPIECE_H__
