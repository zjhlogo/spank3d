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
	void SetPosition(float x, float y);
	const Vector2& GetPosition() const;
	Vector2 GetPositionAbs() const;

	void SetSize(const Vector2& size);
	void SetSize(float width, float height);
	const Vector2& GetSize() const;

	void SetScroll(const Vector2& scroll);
	void SetScroll(float x, float y);
	const Vector2& GetScroll() const;

	IWindow* GetParent() const;

	void SetId(const tstring& id);
	const tstring& GetId() const;

	bool AddChild(IWindow* pChild, uint index = UINT_MAX);
	bool RemoveChild(IWindow* pChild);
	void RemoveAllChildren();
	bool FindChild(IWindow* pChild);

private:
	Vector2 m_Position;
	Vector2 m_Size;
	Vector2 m_Scroll;

	IWindow* m_pParent;
	tstring m_strId;
	TV_WINDOW m_vChildren;

};
#endif // __IWINDOW_H__
