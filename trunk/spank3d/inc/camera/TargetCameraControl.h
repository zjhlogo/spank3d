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

/*
 * TargetCameraControl
 *
 * target camera control, make camera lock target at an object
 */
class TargetCameraControl : public ICameraControl
{
public:
	RTTI_DEF(TargetCameraControl, ICameraControl);

	TargetCameraControl(ICamera* pCamera, const Vector3& eye, const Vector3& target);
	virtual ~TargetCameraControl();

	/*!
	 * \brief update camera control
	 * \param dt
	 * \return 
	 */
	virtual void Update(float dt);
	
	/*!
	 * \brief 
	 * \param mouseEvent
	 * \return 
	 */
	virtual bool HandleMouseEvent(MouseEvent& mouseEvent);
	
	/*!
	 * \brief 
	 * \param keyboardEvent
	 * \return 
	 */
	virtual bool HandleKeyboardEvent(KeyboardEvent& keyboardEvent);

private:
	void DoMouseMove(const Vector2i& offset);
	void DoMouseWheel(float wheel);
	void UpdateMatrix();

private:
	bool m_bMouseDown;
	float m_fRotX;
	float m_fRotY;

	Vector3 m_vEye;
	Vector3 m_vTarget;

};
#endif // __TARGETCAMERACONTROL_H__
