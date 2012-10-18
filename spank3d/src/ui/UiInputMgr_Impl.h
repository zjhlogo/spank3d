/*!
 * \file UiInputMgr_Impl.h
 * \date 10-10-2012 9:30:37
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIINPUTMGR_IMPL_H__
#define __UIINPUTMGR_IMPL_H__

#include <ui/IUiInputMgr.h>
#include <event/MouseEvent.h>
#include <event/KeyboardEvent.h>

class UiInputMgr_Impl : public IUiInputMgr
{
public:
	enum CONST_DEFINE
	{
		VK_LBUTTON = 0x01,
		VK_RBUTTON = 0x02,
		VK_MBUTTON = 0x04,

		NUM_KEYS = 256,
	};

public:
	RTTI_DEF(UiInputMgr_Impl, IUiInputMgr);

	UiInputMgr_Impl();
	virtual ~UiInputMgr_Impl();

	static UiInputMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool IsKeyDown(uchar keyCode) const;
	virtual bool IsLButtonDown() const;
	virtual bool IsMButtonDown() const;
	virtual bool IsRButtonDown() const;

	virtual const Vector2& GetMousePos() const;

	virtual bool CaptureMouse(IWindow* pWindow);
	virtual bool ReleaseMouse();

private:
	bool OnMouseEvent(MouseEvent& event);
	bool OnKeyboardEvent(KeyboardEvent& event);

private:
	bool m_KeyMap[NUM_KEYS];
	Vector2 m_MousePos;
	IWindow* m_pCaptureWindow;

};
#endif // __UIINPUTMGR_IMPL_H__
