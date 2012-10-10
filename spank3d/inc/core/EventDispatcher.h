/*!
 * \file IEventDispatcher.h
 * \date 4-17-2012 23:31:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IEVENTDISPATCHER_H__
#define __IEVENTDISPATCHER_H__

#include "Event.h"
#include "Rtti.h"
#include <map>

class EventDispatcher;
typedef bool (EventDispatcher::*FUNC_HANDLER)(Event& event);

class EventDispatcher
{
public:
	typedef struct EVENT_HANDLER_tag
	{
		EventDispatcher* pHandler;
		FUNC_HANDLER pFunc;
		int nDepth;
	} EVENT_HANDLER;

	typedef std::multimap<uint, EVENT_HANDLER> TM_EVENT_HANDLER;
	typedef std::pair<TM_EVENT_HANDLER::iterator, TM_EVENT_HANDLER::iterator> TP_EVENT_HANDLER;

public:
	RTTI_DEF(EventDispatcher, NoRtti);

	EventDispatcher();
	virtual ~EventDispatcher();

	void RegisterEvent(uint nId, EventDispatcher* pHandler, FUNC_HANDLER pFunc);
	void UnregisterEvent(uint nId, EventDispatcher* pHandler, FUNC_HANDLER pFunc);
	bool DispatchEvent(Event& event);

private:
	TM_EVENT_HANDLER m_EventMap;

};
#endif // __IEVENTDISPATCHER_H__
