/*!
 * \file ISingleton.h
 * \date 4-15-2012 14:09:41
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISINGLETON_H__
#define __ISINGLETON_H__

#include "EventDispatcher.h"

class ISingleton : public EventDispatcher
{
public:
	RTTI_DEF(ISingleton, EventDispatcher);

	ISingleton() {};
	virtual ~ISingleton() {};

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

};
#endif // __ISINGLETON_H__
