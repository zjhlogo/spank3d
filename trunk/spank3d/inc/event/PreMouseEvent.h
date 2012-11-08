#ifndef __PREMOUSEEVENT_H__
#define __PREMOUSEEVENT_H__

#include "MouseEvent.h"

class PreMouseEvent : public MouseEvent
{
public:
	static const tstring PRE_LBUTTON_DOWN;
	static const tstring PRE_LBUTTON_UP;
	static const tstring PRE_MBUTTON_DOWN;
	static const tstring PRE_MBUTTON_UP;
	static const tstring PRE_RBUTTON_DOWN;
	static const tstring PRE_RBUTTON_UP;
	static const tstring PRE_MOUSE_MOVE;
	static const tstring PRE_MOUSE_WHEEL;

public:
	RTTI_DEF(PreMouseEvent, MouseEvent);

	PreMouseEvent(const tstring& strId);
	virtual ~PreMouseEvent();

	void StopProcessing();
	bool IsStopProcessing() const;

	static const tstring& Convert2PreMouseEventType(const tstring& strId);

private:
	bool m_stopProcessing;

};
#endif // __PREMOUSEEVENT_H__
