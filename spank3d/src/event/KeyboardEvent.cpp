/*!
 * \file KeyboardEvent.cpp
 * \date 4-18-2012 1:34:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/KeyboardEvent.h>

KeyboardEvent::KeyboardEvent(KEYBOARD_EVENT_TYPE eEventType)
:Event(EID_KEYBOARD_EVENT)
{
	m_eKeyboardEventType = eEventType;
	m_nKeyCode = 0;
}

KeyboardEvent::KeyboardEvent(uint nId, KEYBOARD_EVENT_TYPE eEventType)
:Event(nId)
{
	m_eKeyboardEventType = eEventType;
	m_nKeyCode = 0;
}

KeyboardEvent::~KeyboardEvent()
{
	// TODO: 
}

KeyboardEvent::KEYBOARD_EVENT_TYPE KeyboardEvent::GetKeyboardEventType() const
{
	return m_eKeyboardEventType;
}

void KeyboardEvent::SetKeyCode(uint nKeyCode)
{
	m_nKeyCode = nKeyCode;
}

uint KeyboardEvent::GetKeyCode() const
{
	return m_nKeyCode;
}