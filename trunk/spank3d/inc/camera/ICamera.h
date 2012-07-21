/*!
 * \file ICamera.h
 * \date 7-03-2012 10:33:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ICAMERA_H__
#define __ICAMERA_H__

#include "../math/Math.h"
#include "../core/IObject.h"

class ICamera : public IObject
{
public:
	RTTI_DEF(ICamera, IObject);

	ICamera();
	virtual ~ICamera();

	void SetViewMatrix(const Matrix4x4& m);
	const Matrix4x4& GetViewMatrix();

private:
	Matrix4x4 m_matView;

};

#endif // __ICAMERA_H__
