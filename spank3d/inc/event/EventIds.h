/*!
 * \file EventIds.h
 * \date 4-18-2012 1:23:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __EVENTIDS_H__
#define __EVENTIDS_H__

enum EVENT_ID_DEF
{
	EID_UNKNOWN = 0,

	EID_LBUTTON_DOWN,				// MouseEvent
	EID_LBUTTON_UP,					// MouseEvent
	EID_MBUTTON_DOWN,				// MouseEvent
	EID_MBUTTON_UP,					// MouseEvent
	EID_RBUTTON_DOWN,				// MouseEvent
	EID_RBUTTON_UP,					// MouseEvent
	EID_MOUSE_MOVE,					// MouseEvent
	EID_MOUSE_WHEEL,				// MouseEvent

	EID_KEY_DOWN,					// KeyboardEvent
	EID_KEY_UP,						// KeyboardEvent
	EID_CHAR,						// KeyboardEvent

	EID_START_PERFORM,				// IEvent

	EID_USER = 10000,
};
#endif // __EVENTIDS_H__
