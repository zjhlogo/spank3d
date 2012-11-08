/*!
 * \file MouseEvent.cpp
 * \date 4-18-2012 1:31:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <event/MouseEvent.h>

const tstring MouseEvent::MOUSE_WHEEL = _("MOUSE_WHEEL");
const tstring MouseEvent::MOUSE_MOVE = _("MOUSE_MOVE");
const tstring MouseEvent::RBUTTON_UP = _("RBUTTON_UP");
const tstring MouseEvent::RBUTTON_DOWN = _("RBUTTON_DOWN");
const tstring MouseEvent::MBUTTON_UP = _("MBUTTON_UP");
const tstring MouseEvent::MBUTTON_DOWN = _("MBUTTON_DOWN");
const tstring MouseEvent::LBUTTON_UP = _("LBUTTON_UP");
const tstring MouseEvent::LBUTTON_DOWN = _("LBUTTON_DOWN");

MouseEvent::MouseEvent(const tstring& strId)
:IEvent(strId)
{
	m_nWheelDetail = 0;
}

MouseEvent::~MouseEvent()
{
	// TODO: 
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
