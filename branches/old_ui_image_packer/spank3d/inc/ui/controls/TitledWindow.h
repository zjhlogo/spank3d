/*!
 * \file TitledWindow.h
 * \date 10-01-2012 8:33:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TITLEDWINDOW_H__
#define __TITLEDWINDOW_H__

#include "IWindow.h"
#include "../style/IFontStyle.h"

class TitledWindow : public IWindow
{
public:
	RTTI_DEF(TitledWindow, IWindow);

	TitledWindow(IWindow* parent);
	virtual ~TitledWindow();

protected:
	virtual void Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	bool OnMouseEvent(MouseEvent& event);

private:
	IGraphicsStyle* m_pTitleStyle;
	IFontStyle* m_pFontStyle;

	Vector2 m_LastMouseDownPos;
	Vector2 m_LastMouseDownWindowPos;
	bool m_bMouseDown;

};
#endif // __TITLEDWINDOW_H__
