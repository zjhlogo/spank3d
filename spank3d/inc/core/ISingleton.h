/*!
 * \file ISingleton.h
 * \date 4-15-2012 14:09:41
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISINGLETON_H__
#define __ISINGLETON_H__

#include "IEventDispatcher.h"

class ISingleton : public IEventDispatcher
{
public:
	RTTI_DEF(ISingleton, IEventDispatcher);

	virtual bool Initialize() = 0;
	virtual void Terminate() = 0;

};
#endif // __ISINGLETON_H__
