/*!
 * \file VContainer.cpp
 * \date 11-15-2012 10:18:44
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/VContainer.h>

VContainer::VContainer(IWindow* pParent)
:IWindow(pParent)
{
	m_padding = 1.0f;
}

VContainer::~VContainer()
{
	// TODO: 
}

bool VContainer::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	return true;
}

void VContainer::ReLayout()
{
	Vector2 maxSize(0.0f, 0.0f);
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		pChild->SetPosition(m_padding, m_padding+maxSize.y);

		const Vector2& childSize = pChild->GetSize();
		maxSize.y += childSize.y;
		if (maxSize.x < childSize.x) maxSize.x = childSize.x;
	}

	SetSize(maxSize.x+m_padding*2.0f, maxSize.y+m_padding*2.0f);
}
