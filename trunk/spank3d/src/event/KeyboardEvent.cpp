/*!
 * \file KeyboardEvent.cpp
 * \date 4-18-2012 1:34:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/KeyboardEvent.h>

const tstring KeyboardEvent::KEY_CHAR = _("KEY_CHAR");
const tstring KeyboardEvent::KEY_UP = _("KEY_UP");
const tstring KeyboardEvent::KEY_DOWN = _("KEY_DOWN");

KeyboardEvent::KeyboardEvent(const tstring& strId)
:IEvent(strId)
{
	m_nKeyCode = 0;
}

KeyboardEvent::~KeyboardEvent()
{
	// TODO: 
}

void KeyboardEvent::SetKeyCode(uchar nKeyCode)
{
	m_nKeyCode = nKeyCode;
}

uchar KeyboardEvent::GetKeyCode() const
{
	return m_nKeyCode;
}
