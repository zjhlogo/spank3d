/*!
 * \file TargetCameraControl.cpp
 * \date 7-03-2012 11:11:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <camera/TargetCameraControl.h>
#include <Spank3d.h>

TargetCameraControl::TargetCameraControl(ICamera* pCamera, const Vector3& eye, const Vector3& target)
:ICameraControl(pCamera)
{
	m_bMouseDown = false;
	m_fRotX = 0.0f;
	m_fRotY = 0.0f;
	m_vEye = eye;
	m_vTarget = target;

	UpdateMatrix();
}

TargetCameraControl::~TargetCameraControl()
{
	// TODO: 
}

void TargetCameraControl::Update(float dt)
{
	// nothing to do
}

void TargetCameraControl::BindMouseEvent()
{
	g_pDevice->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, FUNC_HANDLER(&TargetCameraControl::OnMouseEvent));
	g_pDevice->RegisterEvent(MouseEvent::LBUTTON_UP, this, FUNC_HANDLER(&TargetCameraControl::OnMouseEvent));
	g_pDevice->RegisterEvent(MouseEvent::MOUSE_MOVE, this, FUNC_HANDLER(&TargetCameraControl::OnMouseEvent));
}

void TargetCameraControl::UnbindMouseEvent()
{
	g_pDevice->UnregisterEvent(MouseEvent::LBUTTON_DOWN, this, FUNC_HANDLER(&TargetCameraControl::OnMouseEvent));
	g_pDevice->UnregisterEvent(MouseEvent::LBUTTON_UP, this, FUNC_HANDLER(&TargetCameraControl::OnMouseEvent));
	g_pDevice->UnregisterEvent(MouseEvent::MOUSE_MOVE, this, FUNC_HANDLER(&TargetCameraControl::OnMouseEvent));
}

bool TargetCameraControl::OnMouseEvent(MouseEvent& event)
{
	if (event.GetId() == MouseEvent::LBUTTON_DOWN)
	{
		m_bMouseDown = true;
	}
	else if (event.GetId() == MouseEvent::LBUTTON_UP)
	{
		m_bMouseDown = false;
	}
	else if (event.GetId() == MouseEvent::MOUSE_MOVE)
	{
		if (m_bMouseDown)
		{
			const Vector2& offset = event.GetOffset();
			m_fRotX += offset.x*0.01f;
			m_fRotY -= offset.y*0.01f;
			UpdateMatrix();
		}
	}
	else if (event.GetId() == MouseEvent::MOUSE_WHEEL)
	{
		Vector3 dir = m_vTarget - m_vEye;
		dir.Normalize();

		m_vEye += (dir*float(event.GetWheelDetail())*0.01f);

		UpdateMatrix();
	}
	else
	{
		return false;
	}

	return true;
}

void TargetCameraControl::UpdateMatrix()
{
	Matrix4x4 matBase;
	Math::BuildLookAtMatrix(matBase, m_vEye, m_vTarget, Math::VEC3_UP);

	Matrix4x4 matRot;
	Math::BuildMatrixFromEulerXYZ(matRot, m_fRotX, m_fRotY, 0.0f);

	m_pCamera->SetViewMatrix(matBase*matRot);
}
