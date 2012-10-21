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
#include "../style/IFontStyle.h"

class PushButton : public IWindow
{
public:
	RTTI_DEF(PushButton, IWindow);

	PushButton(IWindow* pParent);
	virtual ~PushButton();

protected:
	virtual void Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	IFontStyle* m_pFontStyle;

};
#endif // __PUSHBUTTON_H__
