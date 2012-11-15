/*!
 * \file HContainer.cpp
 * \date 11-15-2012 11:04:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/HContainer.h>

HContainer::HContainer(IWindow* pParent)
:IWindow(pParent)
{
	m_padding = 1.0f;
}

HContainer::~HContainer()
{
	// TODO: 
}

bool HContainer::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	return true;
}

void HContainer::ReLayout()
{
	Vector2 maxSize(0.0f, 0.0f);
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		const Vector2& childSize = pChild->GetSize();

		pChild->SetPosition(m_padding+maxSize.x, m_padding);

		maxSize.x += childSize.x;
		if (maxSize.y < childSize.y) maxSize.y = childSize.y;
	}

	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		const Vector2& childSize = pChild->GetSize();

		pChild->SetPosition(pChild->GetPosition().x, m_padding+(maxSize.y-childSize.y)*0.5f);
	}

	SetSize(maxSize.x+m_padding*2.0f, maxSize.y+m_padding*2.0f);
}
