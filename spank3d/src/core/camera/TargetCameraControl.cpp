/*!
 * \file TargetCameraControl.cpp
 * \date 7-03-2012 11:11:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/camera/TargetCameraControl.h>

TargetCameraControl::TargetCameraControl(ICamera* pCamera)
:ICameraControl(pCamera)
{
	m_bMouseDown = false;
	m_fRotX = 0.0f;
	m_fRotY = 0.0f;
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

void TargetCameraControl::UpdateMatrix()
{
	Matrix4x4 matBase;
	Math::BuildLookAtMatrix(matBase, Vector3(0.0f, 0.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), Math::VEC_UP);

	Matrix4x4 matRot;
	Math::BuildMatrixFromEulerXYZ(matRot, m_fRotX, m_fRotY, 0.0f);

	m_pCamera->SetViewMatrix(matBase*matRot);
}
