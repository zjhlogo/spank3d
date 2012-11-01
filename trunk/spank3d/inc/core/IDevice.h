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
#include "../math/Vector2.h"

class IDevice : public ISingleton
{
public:
	RTTI_DEF(IDevice, ISingleton);

	IDevice() {};
	virtual ~IDevice() {};

	virtual void StartPerform() = 0;
	virtual void EndPerform() = 0;

	virtual const Vector2& GetSize() const = 0;
	virtual uint GetWidth() const = 0;
	virtual uint GetHeight() const = 0;
};
#endif // __IDEVICE_H__
