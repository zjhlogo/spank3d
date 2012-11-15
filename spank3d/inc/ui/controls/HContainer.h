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

class HContainer : public IWindow
{
public:
	RTTI_DEF(HContainer, IWindow);

	HContainer(IWindow* pParent);
	virtual ~HContainer();

	void ReLayout();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	float m_padding;

};
#endif // __HCONTAINER_H__
