/*!
 * \file wxDocEvent.h
 * \date 11-22-2012 11:04:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __WXDOCEVENT_H__
#define __WXDOCEVENT_H__

#include <wx/event.h>

class wxDocEvent : public wxNotifyEvent
{
public:
	wxDocEvent(wxEventType commandType = wxEVT_NULL, int id = 0);
	wxDocEvent(const wxDocEvent& event);
	virtual wxEvent* Clone() const;

	DECLARE_DYNAMIC_CLASS(wxDocEvent);
};

typedef void (wxEvtHandler::*wxDocEventFunction)(wxDocEvent&);

#define wxDocEventHandler(func) wxEVENT_HANDLER_CAST(wxDocEventFunction, func)

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_DOCUMENT_RESET, 801)
END_DECLARE_EVENT_TYPES()

#define EVT_DOCUMENT_RESET(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_DOCUMENT_RESET, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxDocEventFunction)&fn, (wxObject*)NULL),
#endif // __WXDOCEVENT_H__
