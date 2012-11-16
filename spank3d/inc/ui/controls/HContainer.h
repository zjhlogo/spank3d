/*!
 * \file HContainer.h
 * \date 11-15-2012 11:04:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __HCONTAINER_H__
#define __HCONTAINER_H__

#include "IWindow.h"
#include "../../event/WindowEvent.h"

class HContainer : public IWindow
{
public:
	RTTI_DEF(HContainer, IWindow);

	HContainer(IWindow* pParent);
	virtual ~HContainer();

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
#endif // __HCONTAINER_H__
