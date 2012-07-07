/*!
 * \file ICameraControl.cpp
 * \date 7-03-2012 11:05:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/camera/ICameraControl.h>

ICameraControl::ICameraControl(ICamera* pCamera)
{
	m_pCamera = pCamera;
}

ICameraControl::~ICameraControl()
{
	// TODO: 
}

ICamera* ICameraControl::GetCamera() const
{
	return m_pCamera;
}
