/*!
 * \file TargetCamera.cpp
 * \date 7-03-2012 10:43:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/camera/TargetCamera.h>

TargetCamera::TargetCamera()
{
	// TODO: 
}

TargetCamera::~TargetCamera()
{
	// TODO: 
}

void TargetCamera::SetPosition(const Vector3& pos)
{
	m_vPosition = pos;
	InvalidMatrix();
}

const Vector3& TargetCamera::GetPosition() const
{
	return m_vPosition;
}

void TargetCamera::SetTargetPosition(const Vector3& pos)
{
	m_vTargetPosition = pos;
	InvalidMatrix();
}

const Vector3& TargetCamera::GetTargetPosition() const
{
	return m_vTargetPosition;
}

void TargetCamera::UpdateViewMatrix(Matrix4x4& matView)
{
	Math::BuildLookAtMatrix(matView, m_vPosition, m_vTargetPosition, Math::VEC_UP);
}
