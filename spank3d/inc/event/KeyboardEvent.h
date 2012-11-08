/*!
 * \file KeyboardEvent.h
 * \date 4-18-2012 1:34:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __KEYBOARDEVENT_H__
#define __KEYBOARDEVENT_H__

#include "IEvent.h"

class KeyboardEvent : public IEvent
{
public:
	static const tstring KEY_DOWN;
	static const tstring KEY_UP;
	static const tstring KEY_CHAR;

public:
	RTTI_DEF(KeyboardEvent, IEvent);

	KeyboardEvent(const tstring& strId);
	virtual ~KeyboardEvent();

	void SetKeyCode(uchar nKeyCode);
	uchar GetKeyCode() const;

private:
	uchar m_nKeyCode;

};
#endif // __KEYBOARDEVENT_H__
