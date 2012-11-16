/*!
 * \file WindowEvent.h
 * \date 11-16-2012 13:32:46
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __WINDOWEVENT_H__
#define __WINDOWEVENT_H__

#include "IEvent.h"

class WindowEvent : public IEvent
{
public:
	static const tstring WINDOW_SIZE_CHANGED;

public:
	RTTI_DEF(WindowEvent, IEvent);

	WindowEvent(const tstring& strId);
	virtual ~WindowEvent();

};
#endif // __WINDOWEVENT_H__
