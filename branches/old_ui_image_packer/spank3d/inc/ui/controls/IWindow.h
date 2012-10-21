/*!
 * \file IWindow.h
 * \date 9-19-2012 21:28:53
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IWINDOW_H__
#define __IWINDOW_H__

#include "../../core/IObject.h"
#include "../../math/Vector2.h"
#include "../style/IGraphicsStyle.h"
#include "../../event/MouseEvent.h"
#include "../../event/KeyboardEvent.h"
#include <vector>

class IWindow : public IObject
{
public:
	enum WINDOW_STATE
	{
		WS_MOUSE_DOWN					= 1 << 0,
		WS_MOUSE_HOVER					= 1 << 1,
		WS_FOCUS						= 1 << 2,
		WS_ENABLE						= 1 << 3,
		WS_VISIBLE						= 1 << 4,
		WS_MOUSE_LBUTTON_DOWN_ENABLE	= 1 << 5,
		WS_MOUSE_LBUTTON_UP_ENABLE		= 1 << 6,
		WS_MOUSE_MBUTTON_DOWN_ENABLE	= 1 << 7,
		WS_MOUSE_MBUTTON_UP_ENABLE		= 1 << 8,
		WS_MOUSE_RBUTTON_DOWN_ENABLE	= 1 << 9,
		WS_MOUSE_RBUTTON_UP_ENABLE		= 1 << 10,
		WS_MOUSE_MOVE_ENABLE			= 1 << 11,
		WS_MOUSE_WHEEL_ENABLE			= 1 << 12,
		WS_KEYBOARD_DOWN_ENABLE			= 1 << 13,
		WS_KEYBOARD_UP_ENABLE			= 1 << 14,
		WS_SOLID						= 1 << 15,
		WS_CLIP_CHILDREN				= 1 << 16,
		WS_FOCUS_ENABLE					= 1 << 17,

		WS_MOUSE_ENABLE					= WS_MOUSE_LBUTTON_DOWN_ENABLE
										| WS_MOUSE_LBUTTON_UP_ENABLE
										| WS_MOUSE_MBUTTON_DOWN_ENABLE
										| WS_MOUSE_MBUTTON_UP_ENABLE
										| WS_MOUSE_RBUTTON_DOWN_ENABLE
										| WS_MOUSE_RBUTTON_UP_ENABLE
										| WS_MOUSE_MOVE_ENABLE
										| WS_MOUSE_WHEEL_ENABLE,

		WS_KEYBOARD_ENABLE				= WS_KEYBOARD_DOWN_ENABLE
										| WS_KEYBOARD_UP_ENABLE,

		WS_DEFAULT						= WS_ENABLE
										| WS_VISIBLE
										| WS_MOUSE_ENABLE
										| WS_KEYBOARD_ENABLE
										| WS_CLIP_CHILDREN
										| WS_SOLID,
	};

	typedef std::vector<IWindow*> TV_WINDOW;

public:
	RTTI_DEF(IWindow, IObject);

	IWindow(IWindow* pParent);
	virtual ~IWindow();

	void SetPosition(const Vector2& pos);
	void SetPosition(float x, float y);
	const Vector2& GetPosition() const;
	Vector2 GetPositionAbs() const;

	void SetSize(const Vector2& size);
	void SetSize(float width, float height);
	const Vector2& GetSize() const;

	void SetScroll(const Vector2& scroll);
	void SetScroll(float scrollX, float scrollY);
	const Vector2& GetScroll() const;

	IWindow* GetParent() const;

	void SetTag(const tstring& tag);
	const tstring& GetTag() const;

	bool AddChild(IWindow* pChild, uint index = UINT_MAX);
	bool RemoveChild(IWindow* pChild);
	void RemoveAllChildren();
	bool FindChild(IWindow* pChild);

	bool SystemMouseEvent(MouseEvent& event, bool force = false);
	bool SystemKeyboardEvent(KeyboardEvent& event);

	bool IsOnMe(const Vector2& pos);

	inline bool SetWindowState(uint stateMask, bool set)
	{
		uint oldState = m_WindowState;

		if (set) m_WindowState |= stateMask;
		else m_WindowState &= (~stateMask);

		return (oldState != m_WindowState);
	};

	inline bool CheckWindowState(uint stateMask)
	{
		return (m_WindowState & stateMask) == stateMask;
	};

	void SystemRender(const Vector2& basePos, const Rect& clipRect, uint state);

protected:
	virtual void Render(const Vector2& basePos, const Rect& clipRect, uint state) = 0;

	IWindow* FindChildUnderPoint(const Vector2& pos);
	bool PreProcessMouseEvent(const MouseEvent& event);
	bool PostProcessMouseEvent(const MouseEvent& event);

protected:
	Vector2 m_Position;
	Vector2 m_Size;
	Vector2 m_Scroll;

	IWindow* m_pParent;
	tstring m_strTag;
	TV_WINDOW m_vChildren;

	IGraphicsStyle* m_pBgStyle;
	uint m_WindowState;

};
#endif // __IWINDOW_H__
