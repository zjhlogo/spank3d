/*!
 * \file ICamera.h
 * \date 7-03-2012 10:33:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ICAMERA_H__
#define __ICAMERA_H__

#include "../../math/Math.h"
#include "../IObject.h"

class ICamera : public IObject
{
public:
	RTTI_DEF(ICamera, IObject);

	ICamera();
	virtual ~ICamera();

	void InvalidMatrix();
	void ClearMatrixInvalid();
	bool IsMatrixInvalid() const;

	const Matrix4x4& GetViewMatrix();

protected:
	virtual void UpdateViewMatrix(Matrix4x4& matView) = 0;

private:
	bool m_IsMatrixInvalid;
	Matrix4x4 m_matView;

};

#endif // __ICAMERA_H__
