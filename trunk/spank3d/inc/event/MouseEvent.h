/*!
 * \file MouseEvent.h
 * \date 4-18-2012 1:26:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include "IEvent.h"
#include "../math/Vector2.h"

class MouseEvent : public IEvent
{
public:
	static const tstring LBUTTON_DOWN;
	static const tstring LBUTTON_UP;
	static const tstring MBUTTON_DOWN;
	static const tstring MBUTTON_UP;
	static const tstring RBUTTON_DOWN;
	static const tstring RBUTTON_UP;
	static const tstring MOUSE_MOVE;
	static const tstring MOUSE_WHEEL;

public:
	RTTI_DEF(MouseEvent, IEvent);

	MouseEvent(const tstring& strId);
	virtual ~MouseEvent();

	void SetPosition(const Vector2& pos);
	void SetPosition(float x, float y);
	const Vector2& GetPosition() const;

	void SetOffset(const Vector2& offset);
	void SetOffset(float x, float y);
	const Vector2& GetOffset() const;

	void SetWheelDetail(int nWheel);
	int GetWheelDetail() const;

private:
	Vector2 m_vPosition;
	Vector2 m_vOffset;
	int m_nWheelDetail;

};
#endif // __MOUSEEVENT_H__
