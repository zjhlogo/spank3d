/*!
 * \file IShader.h
 * \date 4-18-2012 10:24:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "../core/IObject.h"
#include "../math/Math.h"
#include "VertexAttribute.h"

class IShader : public IObject
{
public:
	RTTI_DEF(IShader, IObject);

	IShader() {};
	virtual ~IShader() {};

	virtual bool Commit() = 0;
	virtual bool SetMatrix4x4(const Matrix4x4& m, const tstring& strName) = 0;
	virtual bool DrawTriangleList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual const VertexAttribute* GetVertexAttribute() const = 0;
};
#endif // __ISHADER_H__
