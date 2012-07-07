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
	m_IsMatrixInvalid = true;
}

ICamera::~ICamera()
{
	// TODO: 
}

void ICamera::InvalidMatrix()
{
	m_IsMatrixInvalid = true;
}

void ICamera::ClearMatrixInvalid()
{
	m_IsMatrixInvalid = false;
}

bool ICamera::IsMatrixInvalid() const
{
	return m_IsMatrixInvalid;
}

const Matrix4x4& ICamera::GetViewMatrix()
{
	if (IsMatrixInvalid())
	{
		UpdateViewMatrix(m_matView);
		ClearMatrixInvalid();
	}
	return m_matView;
}
