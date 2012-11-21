/*!
 * \file FreeCameraControl.h
 * \date 11-19-2012 9:44:47
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FREECAMERACONTROL_H__
#define __FREECAMERACONTROL_H__

#include "ICameraControl.h"

class FreeCameraControl : public ICameraControl
{
public:
	RTTI_DEF(FreeCameraControl, ICameraControl);

	FreeCameraControl(ICamera* pCamera, const Vector3& eye, const Vector3& target);
	virtual ~FreeCameraControl();

	void BindMouseEvent();
	void UnbindMouseEvent();

	/*!
	 * \brief update camera control
	 * \param dt
	 * \return 
	 */
	virtual void Update(float dt);

private:
	bool OnMouseEvent(MouseEvent& event);
	bool OnKeyboardEvent(KeyboardEvent& event);

	void UpdateMatrix();
};

#endif // __FREECAMERACONTROL_H__
