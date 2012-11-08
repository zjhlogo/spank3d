/*!
 * \file IEvent.h
 * \date 11-08-2012 16:09:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IEVENT_H__
#define __IEVENT_H__

#include "../core/Rtti.h"

class IEventDispatcher;
class IEvent
{
public:
	RTTI_DEF(IEvent, NoRtti);

	IEvent(const tstring& strId);
	virtual ~IEvent();

	const tstring& GetId() const;

	void SetEventDispatcher(IEventDispatcher* pDispatcher);
	IEventDispatcher* GetEventDispatcher() const;

private:
	tstring m_strId;
	IEventDispatcher* m_pDispatcher;

};

class ObjectEvent : public IEvent
{
public:
	static const tstring OBJECT_DESTROYED;

public:
	RTTI_DEF(ObjectEvent, IEvent);

	ObjectEvent(const tstring& strId);
	virtual ~ObjectEvent();

};
#endif // __IEVENT_H__
