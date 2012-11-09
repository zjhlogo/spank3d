/*!
 * \file TitledWindow.h
 * \date 10-01-2012 8:33:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TITLEDWINDOW_H__
#define __TITLEDWINDOW_H__

#include "TitleBar.h"

class TitledWindow : public IWindow
{
public:
	RTTI_DEF(TitledWindow, IWindow);

	TitledWindow(IWindow* parent);
	virtual ~TitledWindow();

	virtual void SetSize(float width, float height);

	void SetTitle(const tstring& strTitle);
	const tstring& GetTitle() const;

	void SetTitleStyle(const tstring& styleId);
	IGraphicsStyle* GetTitleStyle();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	TitleBar* m_pTitleBar;

};
#endif // __TITLEDWINDOW_H__
