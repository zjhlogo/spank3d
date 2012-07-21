/*!
 * \file ICameraControl.h
 * \date 7-03-2012 11:05:22
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ICAMERACONTROL_H__
#define __ICAMERACONTROL_H__

#include "ICamera.h"
#include "../event/MouseEvent.h"
#include "../event/KeyboardEvent.h"

class ICameraControl : public IObject
{
public:
	RTTI_DEF(ICameraControl, IObject);

	ICameraControl(ICamera* pCamera);
	virtual ~ICameraControl();

	ICamera* GetCamera() const;

	virtual void Update(float dt) = 0;
	virtual bool HandleMouseEvent(MouseEvent& mouseEvent) = 0;
	virtual bool HandleKeyboardEvent(KeyboardEvent& keyboardEvent) = 0;

protected:
	ICamera* m_pCamera;

};
#endif // __ICAMERACONTROL_H__
