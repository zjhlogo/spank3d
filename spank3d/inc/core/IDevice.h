/*!
 * \file IDevice.h
 * \date 4-15-2012 12:01:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IDEVICE_H__
#define __IDEVICE_H__

#include "ISingleton.h"

class IDevice : public ISingleton
{
public:
	RTTI_DEF(IDevice, ISingleton);

	IDevice() {};
	virtual ~IDevice() {};

	virtual void StartPerform() = 0;
	virtual void EndPerform() = 0;

	virtual int GetWindowWidth() const = 0;
	virtual int GetWindowHeight() const = 0;

};
#endif // __IDEVICE_H__
