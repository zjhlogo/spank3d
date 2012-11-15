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

class VContainer : public IWindow
{
public:
	RTTI_DEF(VContainer, IWindow);

	VContainer(IWindow* pParent);
	virtual ~VContainer();

	void ReLayout();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	float m_padding;

};
#endif // __VCONTAINER_H__
