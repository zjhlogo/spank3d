/*!
 * \file MouseEvent.cpp
 * \date 4-18-2012 1:31:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/MouseEvent.h>

MouseEvent::MouseEvent(MOUSE_EVENT_TYPE eEventType)
:Event(EID_MOUSE_EVENT)
{
	m_eMouseEventType = eEventType;
	m_nWheelDetail = 0;
}

MouseEvent::MouseEvent(uint nId, MOUSE_EVENT_TYPE eEventType)
:Event(nId)
{
	m_eMouseEventType = eEventType;
	m_nWheelDetail = 0;
}

MouseEvent::~MouseEvent()
{
	// TODO: 
}

MouseEvent::MOUSE_EVENT_TYPE MouseEvent::GetMouseEventType() const
{
	return m_eMouseEventType;
}

void MouseEvent::SetPosition(const Vector2& pos)
{
	SetPosition(pos.x, pos.y);
}

void MouseEvent::SetPosition(float x, float y)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
}

const Vector2& MouseEvent::GetPosition() const
{
	return m_vPosition;
}

void MouseEvent::SetOffset(const Vector2& offset)
{
	SetOffset(offset.x, offset.y);
}

void MouseEvent::SetOffset(float x, float y)
{
	m_vOffset.x = x;
	m_vOffset.y = y;
}

const Vector2& MouseEvent::GetOffset() const
{
	return m_vOffset;
}

void MouseEvent::SetWheelDetail(int nWheel)
{
	m_nWheelDetail = nWheel;
}

int MouseEvent::GetWheelDetail() const
{
	return m_nWheelDetail;
}