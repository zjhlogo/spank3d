/*!
 * \file PushButton.h
 * \date 10-18-2012 15:13:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include "IWindow.h"

class PushButton : public IWindow
{
public:
	RTTI_DEF(PushButton, IWindow);

	PushButton(IWindow* pParent);
	virtual ~PushButton();

protected:
	virtual void Render(uint state);

};
#endif // __PUSHBUTTON_H__
