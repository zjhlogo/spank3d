/*!
 * \file IWindow.cpp
 * \date 9-19-2012 21:28:59
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/IWindow.h>
#include <Spank3d.h>

IWindow::IWindow(IWindow* pParent)
{
	m_pParent = pParent;
	m_pBgStyle = g_pUiResMgr->FindNinePatchStyle(_("nps_default"));
}

IWindow::~IWindow()
{
	// TODO: 
}

void IWindow::SetPosition(const Vector2& pos)
{
	m_Position = pos;
}

const Vector2& IWindow::GetPosition() const
{
	return m_Position;
}

Vector2 IWindow::GetPositionAbs() const
{
	Vector2 posAbs = m_Position;
	IWindow* pParent = m_pParent;
	while (pParent)
	{
		posAbs += pParent->m_Position;
		pParent = pParent->m_pParent;
	}

	return posAbs;
}

void IWindow::SetSize(const Vector2& size)
{
	m_Size = size;
}

const Vector2& IWindow::GetSize() const
{
	return m_Size;
}

void IWindow::SetScroll(const Vector2& scroll)
{
	m_Scroll = scroll;
}

const Vector2& IWindow::GetScroll() const
{
	return m_Scroll;
}

IWindow* IWindow::GetParent() const
{
	return m_pParent;
}

void IWindow::SetTag(const tstring& tag)
{
	m_strTag = tag;
}

const tstring& IWindow::GetTag() const
{
	return m_strTag;
}

bool IWindow::AddChild(IWindow* pChild, uint index /*= UINT_MAX*/)
{
	if (!pChild) return false;

	// check child exist
	for (TV_WINDOW::const_iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		if ((*it) == pChild) return false;
	}

	// adjust the index
	if (index > m_vChildren.size()) index = m_vChildren.size();

	// remove child from its parent
	if (pChild->m_pParent) pChild->m_pParent->RemoveChild(pChild);

	// insert child
	m_vChildren.insert(m_vChildren.begin()+index, pChild);
	pChild->m_pParent = this;

	return true;
}

bool IWindow::RemoveChild(IWindow* pChild)
{
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		if ((*it) == pChild)
		{
			m_vChildren.erase(it);
			pChild->m_pParent = NULL;
			return true;
		}
	}

	return false;
}

void IWindow::RemoveAllChildren()
{
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		pChild->m_pParent = NULL;
	}

	m_vChildren.clear();
}

bool IWindow::FindChild(IWindow* pChild)
{
	for (TV_WINDOW::const_iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		if ((*it) == pChild) return true;
	}

	return false;
}

bool IWindow::SystemMouseEvent(MouseEvent& event)
{
	// TODO: 
	return true;
}

bool IWindow::SystemKeyboardEvent(KeyboardEvent& event)
{
	// TODO: 
	return true;
}

bool IWindow::IsOnMe(const Vector2& pos)
{
	if (pos.x < m_Position.x
		|| pos.y < m_Position.y
		|| pos.x > m_Position.x+m_Size.x
		|| pos.y > m_Position.y+m_Size.y) return false;
	return true;
}
