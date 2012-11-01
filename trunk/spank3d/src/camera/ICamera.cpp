/*!
 * \file ICamera.cpp
 * \date 7-03-2012 10:38:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <camera/ICamera.h>
#include <Spank3d.h>

ICamera::ICamera()
{
	const Vector2& windowSize = g_pDevice->GetSize();
	Math::BuildPerspectiveFovMatrix(m_matProj, 45.0f, windowSize.x, windowSize.y, 0.1f, 100.0f);
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

void ICamera::SetProjectionMatrix(const Matrix4x4& m)
{
	m_matProj = m;
}

const Matrix4x4& ICamera::GetProjectionMatrix()
{
	return m_matProj;
}
