/*!
 * \file ICamera.cpp
 * \date 7-03-2012 10:38:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/camera/ICamera.h>

ICamera::ICamera()
{
	// TODO: 
}

ICamera::~ICamera()
{
	// TODO: 
}

void ICamera::SetViewMatrix(const Matrix4x4& m)
{
	m_matView = m;
}

const Matrix4x4& ICamera::GetViewMatrix()
{
	return m_matView;
}
