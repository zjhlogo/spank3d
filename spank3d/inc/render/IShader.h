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
#include "ITexture.h"

class IShader : public IObject
{
public:
	RTTI_DEF(IShader, IObject);

	IShader() {};
	virtual ~IShader() {};

	virtual const tstring& GetId() const = 0;

	virtual bool BeginRender() = 0;
	virtual void EndRender() = 0;

	virtual bool SetFloat(const tstring& strName, float value) = 0;
	virtual bool SetVector3(const tstring& strName, const Vector3& v) = 0;
	virtual bool SetMatrix3x3(const tstring& strName, const Matrix3x3& m) = 0;
	virtual bool SetMatrix4x4(const tstring& strName, const Matrix4x4& m) = 0;

	virtual bool SetTexture(const tstring& strName, ITexture* pTexture, uint nIndex = 0) = 0;

	virtual bool DrawTriangleList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

	virtual const VertexAttribute* GetVertexAttribute() const = 0;
};
#endif // __ISHADER_H__
