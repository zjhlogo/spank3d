/*!
 * \file IEvent.h
 * \date 4-17-2012 23:40:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IEVENT_H__
#define __IEVENT_H__

#include <core/Rtti.h>

class IEventDispatcher;
class IEvent
{
public:
	RTTI_DEF(IEvent, NoRtti);

	IEvent(uint nId, IEventDispatcher* pDispatcher);
	virtual ~IEvent();

	uint GetId() const;
	IEventDispatcher* GetDispatcher() const;

private:
	uint nId;
	IEventDispatcher* m_pDispatcher;

};
#endif // __IEVENT_H__
