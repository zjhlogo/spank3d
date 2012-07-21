/*!
 * \file TargetCameraControl.cpp
 * \date 7-03-2012 11:11:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <camera/TargetCameraControl.h>

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

bool TargetCameraControl::HandleMouseEvent(MouseEvent& mouseEvent)
{
	switch (mouseEvent.GetMouseEventType())
	{
	case MouseEvent::MET_LBUTTON_DOWN:
		{
			m_bMouseDown = true;
		}
		break;
	case MouseEvent::MET_MOUSE_MOVE:
		{
			if (m_bMouseDown) DoMouseMove(mouseEvent.GetOffset());
		}
		break;
	case MouseEvent::MET_LBUTTON_UP:
		{
			m_bMouseDown = false;
		}
		break;
	case MouseEvent::MET_MOUSE_WHEEL:
		{
			DoMouseWheel(float(mouseEvent.GetWheelDetail()));
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

bool TargetCameraControl::HandleKeyboardEvent(KeyboardEvent& keyboardEvent)
{
	// TODO: 
	return false;
}

void TargetCameraControl::DoMouseMove(const Vector2i& offset)
{
	m_fRotX += offset.x*0.01f;
	m_fRotY -= offset.y*0.01f;

	UpdateMatrix();
}

void TargetCameraControl::DoMouseWheel(float wheel)
{
	Vector3 dir = m_vTarget - m_vEye;
	dir.Normalize();

	m_vEye += (dir*wheel*0.01f);

	UpdateMatrix();
}

void TargetCameraControl::UpdateMatrix()
{
	Matrix4x4 matBase;
	Math::BuildLookAtMatrix(matBase, m_vEye, m_vTarget, Math::VEC_UP);

	Matrix4x4 matRot;
	Math::BuildMatrixFromEulerXYZ(matRot, m_fRotX, m_fRotY, 0.0f);

	m_pCamera->SetViewMatrix(matBase*matRot);
}
