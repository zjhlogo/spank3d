/*!
 * \file FreeCameraControl.cpp
 * \date 11-19-2012 9:44:53
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <camera/FreeCameraControl.h>
#include <Spank3d.h>

FreeCameraControl::FreeCameraControl(ICamera* pCamera, const Vector3& eye, const Vector3& target)
:ICameraControl(pCamera)
{
	// TODO: 
}

FreeCameraControl::~FreeCameraControl()
{
	// TODO: 
}

void FreeCameraControl::BindMouseEvent()
{
	g_pDevice->RegisterEvent(MouseEvent::MOUSE_MOVE, this, FUNC_HANDLER(&FreeCameraControl::OnMouseEvent));
	g_pDevice->RegisterEvent(KeyboardEvent::KEY_DOWN, this, FUNC_HANDLER(&FreeCameraControl::OnKeyboardEvent));
	g_pDevice->RegisterEvent(KeyboardEvent::KEY_UP, this, FUNC_HANDLER(&FreeCameraControl::OnKeyboardEvent));
}

void FreeCameraControl::UnbindMouseEvent()
{
	g_pDevice->UnregisterEvent(MouseEvent::MOUSE_MOVE, this, FUNC_HANDLER(&FreeCameraControl::OnMouseEvent));
	g_pDevice->UnregisterEvent(KeyboardEvent::KEY_DOWN, this, FUNC_HANDLER(&FreeCameraControl::OnKeyboardEvent));
	g_pDevice->UnregisterEvent(KeyboardEvent::KEY_UP, this, FUNC_HANDLER(&FreeCameraControl::OnKeyboardEvent));
}

void FreeCameraControl::Update(float dt)
{
	// TODO: 
}

bool FreeCameraControl::OnMouseEvent(MouseEvent& event)
{
	// TODO: 
	return true;
}

bool FreeCameraControl::OnKeyboardEvent(KeyboardEvent& event)
{
	// TODO: 
	return true;
}

void FreeCameraControl::UpdateMatrix()
{
	// TODO: 
}
