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
	m_paddingLT.Reset(1.0f, 1.0f);
	m_paddingRB.Reset(1.0f, 1.0f);
}

VContainer::~VContainer()
{
	// TODO: 
}

bool VContainer::AddChild(IWindow* pChild, uint index /*= UINT_MAX*/)
{
	if (!IWindow::AddChild(pChild, index)) return false;

	pChild->RegisterEvent(WindowEvent::WINDOW_SIZE_CHANGED, this, (FUNC_HANDLER)&VContainer::OnChildrenSizeChanged);
	ReLayout();
	return true;
}

bool VContainer::RemoveChild(IWindow* pChild)
{
	if (!IWindow::RemoveChild(pChild)) return false;

	pChild->UnregisterEvent(WindowEvent::WINDOW_SIZE_CHANGED, this, (FUNC_HANDLER)&VContainer::OnChildrenSizeChanged);
	ReLayout();
	return true;
}

bool VContainer::RemoveAllChildren()
{
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		pChild->UnregisterEvent(WindowEvent::WINDOW_SIZE_CHANGED, this, (FUNC_HANDLER)&VContainer::OnChildrenSizeChanged);
	}

	if (!IWindow::RemoveAllChildren()) return false;

	ReLayout();
	return true;
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
		pChild->SetPosition(m_paddingLT.x, maxSize.y+m_paddingLT.y);

		const Vector2& childSize = pChild->GetSize();
		maxSize.y += childSize.y;
		if (maxSize.x < childSize.x) maxSize.x = childSize.x;
	}

	SetSize(maxSize+m_paddingLT+m_paddingRB);
}

bool VContainer::OnChildrenSizeChanged(WindowEvent& event)
{
	ReLayout();
	return true;
}
