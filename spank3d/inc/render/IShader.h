/*!
 * \file IShader.h
 * \date 4-18-2012 10:24:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISHADER_H__
#define __ISHADER_H__

#include <core/IObject.h>
#include <math/Math.h>

class IShader : public IObject
{
public:
	RTTI_DEF(IShader, IObject);

	IShader() {};
	virtual ~IShader() {};

	virtual bool SetMatrix4x4(const Matrix4x4& m, const tstring& strName) = 0;
	virtual bool Commit() = 0;

};
#endif // __ISHADER_H__
