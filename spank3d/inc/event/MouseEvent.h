/*!
 * \file MouseEvent.h
 * \date 4-18-2012 1:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include "../core/IEvent.h"
#include "EventIds.h"
#include "../math/Vector2i.h"

class MouseEvent : public IEvent
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
	RTTI_DEF(MouseEvent, IEvent);

	MouseEvent(MOUSE_EVENT_TYPE eEventType, IEventDispatcher* pDispatcher);
	virtual ~MouseEvent();

	MOUSE_EVENT_TYPE GetMouseEventType() const;

	void SetPosition(const Vector2i& pos);
	void SetPosition(int x, int y);
	const Vector2i& GetPosition() const;

	void SetOffset(const Vector2i& offset);
	void SetOffset(int x, int y);
	const Vector2i& GetOffset() const;

	void SetWheelDetail(int nWheel);
	int GetWheelDetail() const;

private:
	MOUSE_EVENT_TYPE m_eMouseEventType;
	Vector2i m_vPosition;
	Vector2i m_vOffset;
	int m_nWheelDetail;

};
#endif // __MOUSEEVENT_H__
