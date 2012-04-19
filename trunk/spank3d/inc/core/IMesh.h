/*!
 * \file IMesh.h
 * \date 4-19-2012 22:40:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMESH_H__
#define __IMESH_H__

#include "IMeshPiece.h"
#include "../math/Math.h"

class IMesh : public IObject
{
public:
	RTTI_DEF(IMesh, IObject);

	IMesh() {};
	virtual ~IMesh() {};

	virtual int GetNumPieces() const = 0;
	virtual IMeshPiece* GetPiece(int nIndex) const = 0;
	virtual IMeshPiece* FindPiece(const tstring& strName) const = 0;

	virtual const Vector3& GetBoundingBoxMin() const = 0;
	virtual const Vector3& GetBoundingBoxMax() const = 0;

};

#endif // __IMESH_H__
