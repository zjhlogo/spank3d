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
	typedef std::vector<IWindow*> TV_WINDOW;

public:
	RTTI_DEF(IWindow, IObject);

	IWindow(IWindow* pParent);
	virtual ~IWindow();

	void SetPosition(const Vector2& pos);
	const Vector2& GetPosition() const;
	Vector2 GetPositionAbs() const;

	void SetSize(const Vector2& size);
	const Vector2& GetSize() const;

	void SetScroll(const Vector2& scroll);
	const Vector2& GetScroll() const;

	IWindow* GetParent() const;

	void SetTag(const tstring& tag);
	const tstring& GetTag() const;

	bool AddChild(IWindow* pChild, uint index = UINT_MAX);
	bool RemoveChild(IWindow* pChild);
	void RemoveAllChildren();
	bool FindChild(IWindow* pChild);

	bool SystemMouseEvent(MouseEvent& event);
	bool SystemKeyboardEvent(KeyboardEvent& event);

	bool IsOnMe(const Vector2& pos);

	virtual void Render(uint state) = 0;

protected:
	Vector2 m_Position;
	Vector2 m_Size;
	Vector2 m_Scroll;

	IWindow* m_pParent;
	tstring m_strTag;
	TV_WINDOW m_vChildren;

	IGraphicsStyle* m_pBgStyle;

};
#endif // __IWINDOW_H__
