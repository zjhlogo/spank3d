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

/*
 * ICamera
 *
 * camera interface
 */
class ICamera : public IObject
{
public:
	RTTI_DEF(ICamera, IObject);

	ICamera();
	virtual ~ICamera();

	/*!
	 * \brief set view matrix
	 * \param m
	 * \return 
	 */
	void SetViewMatrix(const Matrix4x4& m);
	
	/*!
	 * \brief get view matrix
	 * \return 
	 */
	const Matrix4x4& GetViewMatrix();

	void SetProjectionMatrix(const Matrix4x4& m);
	const Matrix4x4& GetProjectionMatrix();

private:
	Matrix4x4 m_matView;
	Matrix4x4 m_matProj;
};

#endif // __ICAMERA_H__
