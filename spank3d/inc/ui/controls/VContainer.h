/*!
 * \file VContainer.h
 * \date 11-15-2012 10:18:38
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __VCONTAINER_H__
#define __VCONTAINER_H__

#include "IWindow.h"
#include "../../event/WindowEvent.h"

class VContainer : public IWindow
{
public:
	RTTI_DEF(VContainer, IWindow);

	VContainer(IWindow* pParent);
	virtual ~VContainer();

	virtual bool AddChild(IWindow* pChild, uint index = UINT_MAX);
	virtual bool RemoveChild(IWindow* pChild);
	virtual bool RemoveAllChildren();

	void ReLayout();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	bool OnChildrenSizeChanged(WindowEvent& event);

private:
	Vector2 m_paddingLT;
	Vector2 m_paddingRB;

};
#endif // __VCONTAINER_H__
