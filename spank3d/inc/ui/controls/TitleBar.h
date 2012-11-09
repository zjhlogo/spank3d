/*!
 * \file TitleBar.h
 * \date 11-09-2012 13:04:22
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

#include "IWindow.h"
#include "../style/IFontStyle.h"

class TitleBar : public IWindow
{
public:
	RTTI_DEF(TitleBar, IWindow);

	TitleBar(IWindow* parent);
	virtual ~TitleBar();

	void SetTitle(const tstring& strTitle);
	const tstring& GetTitle() const;

	void SetTargetWindow(IWindow* target);
	IWindow* GetTargetWindow();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	bool OnMouseDown(MouseEvent& event);
	bool OnMouseUp(MouseEvent& event);
	bool OnMouseMove(MouseEvent& event);

private:
	IFontStyle* m_pFontStyle;

	tstring m_strTitle;

	Vector2 m_LastMouseDownPos;
	Vector2 m_LastTargetWindowPos;
	bool m_bMouseDown;
	IWindow* m_pTargetWindow;

};
#endif // __TITLEBAR_H__
