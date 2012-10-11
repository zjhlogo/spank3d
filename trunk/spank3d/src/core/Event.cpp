/*!
 * \file Event.cpp
 * \date 10-10-2012 9:51:36
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/Event.h>
#include <core/EventDispatcher.h>

Event::Event(uint nId)
{
	m_nId = nId;
	m_pDispatcher = NULL;
}

Event::~Event()
{
	// TODO: 
}

uint Event::GetId() const
{
	return m_nId;
}

void Event::SetEventDispatcher(EventDispatcher* pDispatcher)
{
	m_pDispatcher = pDispatcher;
}

EventDispatcher* Event::GetEventDispatcher() const
{
	return m_pDispatcher;
}
