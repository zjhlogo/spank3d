/*!
 * \file IEvent.cpp
 * \date 4-17-2012 23:40:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/IEvent.h>
#include <core/IEventDispatcher.h>

IEvent::IEvent(uint nId, IEventDispatcher* pDispatcher)
{
	m_nId = nId;
	m_pDispatcher = pDispatcher;
}

IEvent::~IEvent()
{
	// TODO: 
}

uint IEvent::GetId() const
{
	return m_nId;
}

IEventDispatcher* IEvent::GetDispatcher() const
{
	return m_pDispatcher;
}
