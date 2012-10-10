/*!
 * \file KeyboardEvent.h
 * \date 4-18-2012 1:34:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __KEYBOARDEVENT_H__
#define __KEYBOARDEVENT_H__

#include "../core/Event.h"

class KeyboardEvent : public Event
{
public:
	enum KEYBOARD_EVENT_TYPE
	{
		KET_KEY_DOWN,					// KeyboardEvent
		KET_KEY_UP,						// KeyboardEvent
		KET_CHAR						// KeyboardEvent
	};
public:
	RTTI_DEF(KeyboardEvent, Event);

	KeyboardEvent(KEYBOARD_EVENT_TYPE eEventType, EventDispatcher* pDispatcher);
	virtual ~KeyboardEvent();

	KEYBOARD_EVENT_TYPE GetKeyboardEventType() const;

	void SetKeyCode(uint nKeyCode);
	uint GetKeyCode() const;

private:
	KEYBOARD_EVENT_TYPE m_eKeyboardEventType;
	uint m_nKeyCode;

};
#endif // __KEYBOARDEVENT_H__
