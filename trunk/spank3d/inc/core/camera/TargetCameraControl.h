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

class TargetCameraControl : public ICameraControl
{
public:
	RTTI_DEF(TargetCameraControl, ICameraControl);

	TargetCameraControl(ICamera* pCamera);
	virtual ~TargetCameraControl();

	virtual void Update(float dt);
	virtual bool HandleMouseEvent(MouseEvent& mouseEvent);
	virtual bool HandleKeyboardEvent(KeyboardEvent& keyboardEvent);

private:
	void DoMouseMove(const Vector2i& offset);
	void UpdateMatrix();

private:
	bool m_bMouseDown;
	float m_fRotX;
	float m_fRotY;

};
#endif // __TARGETCAMERACONTROL_H__
