/*!
 * \file MouseEvent.h
 * \date 4-18-2012 1:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include <core/IEvent.h>

class MouseEvent : public IEvent
{
public:
	typedef struct POSITION_tag
	{
		int x, y;
	} POSITION;

public:
	RTTI_DEF(MouseEvent, IEvent);

	MouseEvent(uint nId, IEventDispatcher* pDispatcher);
	virtual ~MouseEvent();

	void SetPosition(int x, int y);
	const POSITION& GetPosition() const;

	void SetWheelDetail(int nWheel);
	int GetWheelDetail() const;

private:
	POSITION m_Position;
	int m_nWheelDetail;

};
#endif // __MOUSEEVENT_H__
