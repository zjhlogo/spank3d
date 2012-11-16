/*!
 * \file WindowEvent.cpp
 * \date 11-16-2012 13:32:51
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/WindowEvent.h>

const tstring WindowEvent::WINDOW_SIZE_CHANGED = _("WINDOW_SIZE_CHANGED");

WindowEvent::WindowEvent(const tstring& strId)
:IEvent(strId)
{

}

WindowEvent::~WindowEvent()
{

}
