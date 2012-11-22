/*!
 * \file wxDocEvent.cpp
 * \date 11-22-2012 11:04:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "wxDocEvent.h"

DEFINE_EVENT_TYPE(wxEVT_DOCUMENT_RESET)
IMPLEMENT_DYNAMIC_CLASS(wxDocEvent, wxNotifyEvent)

wxDocEvent::wxDocEvent(wxEventType commandType /*= wxEVT_NULL*/, int id /*= 0*/)
:wxNotifyEvent(commandType, id)
{
	// TODO: 
}

wxDocEvent::wxDocEvent(const wxDocEvent& event)
:wxNotifyEvent(event)
{
	// TODO: 
}

wxEvent* wxDocEvent::Clone() const
{
	return new wxDocEvent(*this);
}
