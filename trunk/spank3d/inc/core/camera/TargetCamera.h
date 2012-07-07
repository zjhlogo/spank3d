/*!
 * \file TargetCamera.h
 * \date 7-03-2012 10:41:22
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TARGETCAMERA_H__
#define __TARGETCAMERA_H__

#include "ICamera.h"

class TargetCamera : public ICamera
{
public:
	RTTI_DEF(TargetCamera, ICamera);

	TargetCamera();
	virtual ~TargetCamera();

	virtual void SetPosition(const Vector3& pos);
	virtual const Vector3& GetPosition() const;

	virtual void SetTargetPosition(const Vector3& pos);
	virtual const Vector3& GetTargetPosition() const;

protected:
	virtual void UpdateViewMatrix(Matrix4x4& matView);

private:
	Vector3 m_vPosition;
	Vector3 m_vTargetPosition;

};
#endif // __TARGETCAMERA_H__
