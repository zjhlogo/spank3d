/*!
 * \file Event.h
 * \date 10-10-2012 9:51:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __EVENT_H__
#define __EVENT_H__

#include "Rtti.h"
#include "../event/EventIds.h"

class EventDispatcher;
class Event
{
public:
	RTTI_DEF(Event, NoRtti);

	Event(uint nId);
	virtual ~Event();

	uint GetId() const;

	void SetEventDispatcher(EventDispatcher* pDispatcher);
	EventDispatcher* GetEventDispatcher() const;

private:
	uint m_nId;
	EventDispatcher* m_pDispatcher;

};
#endif // __EVENT_H__
