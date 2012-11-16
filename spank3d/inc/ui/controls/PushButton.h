/*!
 * \file PushButton.h
 * \date 10-18-2012 15:13:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

#include "Label.h"

class PushButton : public Label
{
public:
	RTTI_DEF(PushButton, Label);

	PushButton(IWindow* pParent);
	virtual ~PushButton();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);
	virtual void AdjustSize();

private:

};
#endif // __PUSHBUTTON_H__
