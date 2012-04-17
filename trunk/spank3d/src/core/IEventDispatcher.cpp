/*!
 * \file IEventDispatcher.cpp
 * \date 4-17-2012 23:31:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/IEventDispatcher.h>

IEventDispatcher::IEventDispatcher()
{
	// TODO: 
}

IEventDispatcher::~IEventDispatcher()
{
	// TODO: 
}

void IEventDispatcher::RegisterEvent(uint nId, IEventDispatcher* pHandler, FUNC_HANDLER pFunc)
{
	EVENT_HANDLER handler;
	handler.pHandler = pHandler;
	handler.pFunc = pFunc;
	handler.nDepth = 0;
	m_EventMap.insert(std::make_pair(nId, handler));
}

void IEventDispatcher::UnregisterEvent(uint nId, IEventDispatcher* pHandler, FUNC_HANDLER pFunc)
{
	TP_EVENT_HANDLER range = m_EventMap.equal_range(nId);
	if (range.first == range.second) return;

	for (TM_EVENT_HANDLER::iterator it = range.first; it != range.second; ++it)
	{
		EVENT_HANDLER& handler = it->second;
		if (handler.pHandler == pHandler && handler.pFunc == pFunc)
		{
			m_EventMap.erase(it);
			return;
		}
	}
}

bool IEventDispatcher::DispatchEvent(IEvent& event)
{
	TP_EVENT_HANDLER range = m_EventMap.equal_range(event.GetId());
	if (range.first == range.second) return false;

	for (TM_EVENT_HANDLER::iterator it = range.first; it != range.second; ++it)
	{
		EVENT_HANDLER& handler = it->second;

		// check the loop depth, it must always 0 or 1
		if (handler.nDepth > 0)
		{
			assert(false);
			continue;
		}
		++handler.nDepth;

		if (!(handler.pHandler->*handler.pFunc)(event))
		{
			assert(false);
		}

		--handler.nDepth;
	}

	return true;
}
