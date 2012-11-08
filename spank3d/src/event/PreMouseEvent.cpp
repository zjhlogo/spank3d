/*!
 * \file PreMouseEvent.cpp
 * \date 11-08-2012 16:49:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/PreMouseEvent.h>

const tstring PreMouseEvent::PRE_MOUSE_WHEEL = _("PRE_MOUSE_WHEEL");
const tstring PreMouseEvent::PRE_MOUSE_MOVE = _("PRE_MOUSE_MOVE");
const tstring PreMouseEvent::PRE_RBUTTON_UP = _("PRE_RBUTTON_UP");
const tstring PreMouseEvent::PRE_RBUTTON_DOWN = _("PRE_RBUTTON_DOWN");
const tstring PreMouseEvent::PRE_MBUTTON_UP = _("PRE_MBUTTON_UP");
const tstring PreMouseEvent::PRE_MBUTTON_DOWN = _("PRE_MBUTTON_DOWN");
const tstring PreMouseEvent::PRE_LBUTTON_UP = _("PRE_LBUTTON_UP");
const tstring PreMouseEvent::PRE_LBUTTON_DOWN = _("PRE_LBUTTON_DOWN");

PreMouseEvent::PreMouseEvent(const tstring& strId)
:MouseEvent(strId)
{
	m_stopProcessing = false;
}

PreMouseEvent::~PreMouseEvent()
{
	// TODO: 
}

void PreMouseEvent::StopProcessing()
{
	m_stopProcessing = true;
}

bool PreMouseEvent::IsStopProcessing() const
{
	return m_stopProcessing;
}

const tstring& PreMouseEvent::Convert2PreMouseEventType(const tstring& strId)
{
	if (strId == LBUTTON_DOWN)
	{
		return PRE_LBUTTON_DOWN;
	}
	else if (strId == LBUTTON_UP)
	{
		return PRE_LBUTTON_UP;
	}
	else if (strId == MBUTTON_DOWN)
	{
		return PRE_MBUTTON_DOWN;
	}
	else if (strId == MBUTTON_UP)
	{
		return PRE_MBUTTON_UP;
	}
	else if (strId == RBUTTON_DOWN)
	{
		return PRE_RBUTTON_DOWN;
	}
	else if (strId == RBUTTON_UP)
	{
		return PRE_RBUTTON_UP;
	}
	else if (strId == MOUSE_MOVE)
	{
		return PRE_MOUSE_MOVE;
	}
	else if (strId == MOUSE_WHEEL)
	{
		return PRE_MOUSE_WHEEL;
	}

	return PRE_LBUTTON_DOWN;
}
