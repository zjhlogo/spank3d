/*!
 * \file TargetCameraControl.cpp
 * \date 7-03-2012 11:11:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/camera/TargetCameraControl.h>

TargetCameraControl::TargetCameraControl(TargetCamera* pCamera)
:ICameraControl(pCamera)
{
	// TODO: 
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
	// TODO: 
	return false;
}

bool TargetCameraControl::HandleKeyboardEvent(KeyboardEvent& keyboardEvent)
{
	// TODO: 
	return false;
}
