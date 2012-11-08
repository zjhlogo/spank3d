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

/*
 * ICameraControl
 *
 * camera control interface
 */
class ICameraControl : public IObject
{
public:
	RTTI_DEF(ICameraControl, IObject);

	ICameraControl(ICamera* pCamera);
	virtual ~ICameraControl();

	/*!
	 * \brief get camera instance
	 * \return 
	 */
	ICamera* GetCamera() const;

	/*!
	 * \brief update camera
	 * \param dt
	 * \return 
	 */
	virtual void Update(float dt) = 0;

protected:
	ICamera* m_pCamera;

};
#endif // __ICAMERACONTROL_H__
