/*!
 * \file MouseEvent.h
 * \date 4-18-2012 1:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include "../core/Event.h"
#include "EventIds.h"
#include "../math/Vector2.h"

class MouseEvent : public Event
{
public:
	enum MOUSE_EVENT_TYPE
	{
		MET_LBUTTON_DOWN,				// MouseEvent
		MET_LBUTTON_UP,					// MouseEvent
		MET_MBUTTON_DOWN,				// MouseEvent
		MET_MBUTTON_UP,					// MouseEvent
		MET_RBUTTON_DOWN,				// MouseEvent
		MET_RBUTTON_UP,					// MouseEvent
		MET_MOUSE_MOVE,					// MouseEvent
		MET_MOUSE_WHEEL					// MouseEvent
	};

public:
	RTTI_DEF(MouseEvent, Event);

	MouseEvent(MOUSE_EVENT_TYPE eEventType);
	MouseEvent(uint nId, MOUSE_EVENT_TYPE eEventType);
	virtual ~MouseEvent();

	MOUSE_EVENT_TYPE GetMouseEventType() const;

	void SetPosition(const Vector2& pos);
	void SetPosition(float x, float y);
	const Vector2& GetPosition() const;

	void SetOffset(const Vector2& offset);
	void SetOffset(float x, float y);
	const Vector2& GetOffset() const;

	void SetWheelDetail(int nWheel);
	int GetWheelDetail() const;

private:
	MOUSE_EVENT_TYPE m_eMouseEventType;
	Vector2 m_vPosition;
	Vector2 m_vOffset;
	int m_nWheelDetail;

};
#endif // __MOUSEEVENT_H__
