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
	m_paddingLT.Reset(1.0f, 1.0f);
	m_paddingRB.Reset(1.0f, 1.0f);
}

HContainer::~HContainer()
{
	// TODO: 
}

bool HContainer::AddChild(IWindow* pChild, uint index /*= UINT_MAX*/)
{
	if (!IWindow::AddChild(pChild, index)) return false;

	pChild->RegisterEvent(WindowEvent::WINDOW_SIZE_CHANGED, this, (FUNC_HANDLER)&HContainer::OnChildrenSizeChanged);
	ReLayout();
	return true;
}

bool HContainer::RemoveChild(IWindow* pChild)
{
	if (!IWindow::RemoveChild(pChild)) return false;

	pChild->UnregisterEvent(WindowEvent::WINDOW_SIZE_CHANGED, this, (FUNC_HANDLER)&HContainer::OnChildrenSizeChanged);
	ReLayout();
	return true;
}

bool HContainer::RemoveAllChildren()
{
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		pChild->UnregisterEvent(WindowEvent::WINDOW_SIZE_CHANGED, this, (FUNC_HANDLER)&HContainer::OnChildrenSizeChanged);
	}

	if (!IWindow::RemoveAllChildren()) return false;

	ReLayout();
	return true;
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

		pChild->SetPosition(maxSize.x+m_paddingLT.x, m_paddingLT.y);

		maxSize.x += childSize.x;
		if (maxSize.y < childSize.y) maxSize.y = childSize.y;
	}

	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		const Vector2& childSize = pChild->GetSize();

		pChild->SetPosition(pChild->GetPosition().x, m_paddingLT.y+(maxSize.y-childSize.y)*0.5f);
	}

	SetSize(maxSize+m_paddingLT+m_paddingRB);
}

bool HContainer::OnChildrenSizeChanged(WindowEvent& event)
{
	ReLayout();
	return true;
}
