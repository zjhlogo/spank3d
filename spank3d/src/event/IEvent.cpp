/*!
 * \file IEvent.cpp
 * \date 11-08-2012 16:09:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/IEvent.h>
#include <event/IEventDispatcher.h>

IEvent::IEvent(const tstring& strId)
{
	m_strId = strId;
	m_pDispatcher = NULL;
}

IEvent::~IEvent()
{
	// TODO: 
}

const tstring& IEvent::GetId() const
{
	return m_strId;
}

void IEvent::SetEventDispatcher(IEventDispatcher* pDispatcher)
{
	m_pDispatcher = pDispatcher;
}

IEventDispatcher* IEvent::GetEventDispatcher() const
{
	return m_pDispatcher;
}

const tstring ObjectEvent::OBJECT_DESTROYED = _("OBJECT_DESTROYED");

ObjectEvent::ObjectEvent(const tstring& strId)
:IEvent(OBJECT_DESTROYED)
{
	// TODO: 
}

ObjectEvent::~ObjectEvent()
{
	// TODO: 
}
