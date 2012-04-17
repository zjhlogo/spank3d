/*!
 * \file KeyboardEvent.h
 * \date 4-18-2012 1:34:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __KEYBOARDEVENT_H__
#define __KEYBOARDEVENT_H__

#include <core/IEvent.h>

class KeyboardEvent : public IEvent
{
public:
	RTTI_DEF(KeyboardEvent, IEvent);

	KeyboardEvent(uint nId, IEventDispatcher* pDispatcher);
	virtual ~KeyboardEvent();

	void SetKeyCode(uint nKeyCode);
	uint GetKeyCode() const;

private:
	uint m_nKeyCode;

};
#endif // __KEYBOARDEVENT_H__
