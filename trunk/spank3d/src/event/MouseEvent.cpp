/*!
 * \file MouseEvent.cpp
 * \date 4-18-2012 1:31:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/MouseEvent.h>

MouseEvent::MouseEvent(uint nId, IEventDispatcher* pDispatcher)
:IEvent(nId, pDispatcher)
{
	m_Position.x = 0;
	m_Position.y = 0;
	m_nWheelDetail = 0;
}

MouseEvent::~MouseEvent()
{
	// TODO: 
}

void MouseEvent::SetPosition(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
}

const MouseEvent::POSITION& MouseEvent::GetPosition() const
{
	return m_Position;
}

void MouseEvent::SetWheelDetail(int nWheel)
{
	m_nWheelDetail = nWheel;
}

int MouseEvent::GetWheelDetail() const
{
	return m_nWheelDetail;
}