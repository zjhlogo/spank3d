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

	EID_OBJECT_DESTROYED,			// IEvent

	EID_MOUSE_EVENT,				// MouseEvent
	EID_KEYBOARD_EVENT,				// KeyboardEvent

	EID_START_PERFORM,				// IEvent

	EID_UI = 10000,
	EID_UI_PRE_MOUSE_EVENT,
	EID_UI_POST_MOUSE_EVENT,
	EID_UI_PRE_KEYBOARD_EVENT,
	EID_UI_POST_KEYBOARD_EVENT,
	EID_UI_RENDER,
	// ...

	EID_USER = 20000,
	// ...
};
#endif // __EVENTIDS_H__
