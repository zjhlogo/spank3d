/*!
 * \file TargetCameraControl.h
 * \date 7-03-2012 11:11:23
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TARGETCAMERACONTROL_H__
#define __TARGETCAMERACONTROL_H__

#include "ICameraControl.h"
#include "TargetCamera.h"

class TargetCameraControl : public ICameraControl
{
public:
	RTTI_DEF(TargetCameraControl, ICameraControl);

	TargetCameraControl(TargetCamera* pCamera);
	virtual ~TargetCameraControl();

	virtual void Update(float dt);
	virtual bool HandleMouseEvent(MouseEvent& mouseEvent);
	virtual bool HandleKeyboardEvent(KeyboardEvent& keyboardEvent);

};
#endif // __TARGETCAMERACONTROL_H__
