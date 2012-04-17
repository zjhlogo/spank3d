/*!
 * \file KeyboardEvent.cpp
 * \date 4-18-2012 1:34:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/KeyboardEvent.h>

KeyboardEvent::KeyboardEvent(uint nId, IEventDispatcher* pDispatcher)
:IEvent(nId, pDispatcher)
{
	m_nKeyCode = 0;
}

KeyboardEvent::~KeyboardEvent()
{
	// TODO: 
}

void KeyboardEvent::SetKeyCode(uint nKeyCode)
{
	m_nKeyCode = nKeyCode;
}

uint KeyboardEvent::GetKeyCode() const
{
	return m_nKeyCode;
}