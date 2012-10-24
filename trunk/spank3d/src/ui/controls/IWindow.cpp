/*!
 * \file IWindow.cpp
 * \date 9-19-2012 21:28:59
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/IWindow.h>
#include <ui/UiState.h>
#include <Spank3d.h>

IWindow::IWindow(IWindow* pParent)
{
	m_pParent = NULL;
	m_pBgStyle = g_pUiResMgr->FindNinePatchStyle(_("nps_border"));
	m_WindowState = WS_DEFAULT;
	if (pParent) pParent->AddChild(this);
}

IWindow::~IWindow()
{
	if (m_pParent)
	{
		m_pParent->RemoveChild(this);
		m_pParent = NULL;
	}

	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		pChild->m_pParent = NULL;
		SAFE_DELETE(pChild);
	}

	m_vChildren.clear();

	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

void IWindow::SetPosition(const Vector2& pos)
{
	SetPosition(pos.x, pos.y);
}

void IWindow::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
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
	SetSize(size.x, size.y);
}

void IWindow::SetSize(float width, float height)
{
	m_Size.x = width;
	m_Size.y = height;
}

const Vector2& IWindow::GetSize() const
{
	return m_Size;
}

void IWindow::SetScroll(const Vector2& scroll)
{
	m_Scroll = scroll;
	SetScroll(scroll.x, scroll.y);
}

void IWindow::SetScroll(float scrollX, float scrollY)
{
	m_Scroll.x = scrollX;
	m_Scroll.y = scrollY;
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

bool IWindow::SystemMouseEvent(MouseEvent& event, bool force /* = false */)
{
	if (!CheckWindowState(WS_ENABLE)) return true;

	Vector2 mousePos = event.GetPosition();
	if (!force && !IsOnMe(mousePos)) return false;

	Vector2 localPos(mousePos.x-m_Position.x, mousePos.y-m_Position.y);

	// find child under point
	IWindow* pChild = FindChildUnderPoint(localPos);
	if (!pChild)
	{
		uint testMask = 0;
		switch (event.GetMouseEventType())
		{
		case MouseEvent::MET_LBUTTON_DOWN:
			testMask = WS_MOUSE_LBUTTON_DOWN_ENABLE;
			break;
		case MouseEvent::MET_LBUTTON_UP:
			testMask = WS_MOUSE_LBUTTON_UP_ENABLE;
			break;
		case MouseEvent::MET_MBUTTON_DOWN:
			testMask = WS_MOUSE_MBUTTON_DOWN_ENABLE;
			break;
		case MouseEvent::MET_MBUTTON_UP:
			testMask = WS_MOUSE_MBUTTON_UP_ENABLE;
			break;
		case MouseEvent::MET_RBUTTON_DOWN:
			testMask = WS_MOUSE_RBUTTON_DOWN_ENABLE;
			break;
		case MouseEvent::MET_RBUTTON_UP:
			testMask = WS_MOUSE_RBUTTON_UP_ENABLE;
			break;
		case MouseEvent::MET_MOUSE_MOVE:
			testMask = WS_MOUSE_MOVE_ENABLE;
			break;
		case MouseEvent::MET_MOUSE_WHEEL:
			testMask = WS_MOUSE_WHEEL_ENABLE;
			break;
		}
		if (!CheckWindowState(testMask)) return false;

		event.SetPosition(localPos);
		// pre process mouse event
		bool bPreProcessed = PreProcessMouseEvent(event);
		// post process mouse event
		bool bPostProcessed = PostProcessMouseEvent(event);
		event.SetPosition(mousePos);

		return (bPreProcessed || bPostProcessed);
	}

	event.SetPosition(localPos);
	bool bProcessed = pChild->SystemMouseEvent(event);
	event.SetPosition(mousePos);

	return bProcessed;
}

bool IWindow::SystemKeyboardEvent(KeyboardEvent& event)
{
	if (!CheckWindowState(WS_ENABLE)) return true;

	return true;
}

bool IWindow::SetBgStyle(const tstring& styleId)
{
	IGraphicsStyle* pGraphicsStyle = g_pUiResMgr->FindStyle(styleId);
	if (!pGraphicsStyle) return false;

	m_pBgStyle = pGraphicsStyle;
	return true;
}

bool IWindow::IsOnMe(const Vector2& pos)
{
	if (!CheckWindowState(WS_VISIBLE|WS_SOLID)) return false;

	if (pos.x < m_Position.x
		|| pos.y < m_Position.y
		|| pos.x > m_Position.x+m_Size.x
		|| pos.y > m_Position.y+m_Size.y) return false;

	return true;
}

void IWindow::SystemRender(const Vector2& basePos, const Rect& clipRect, uint state)
{
	uint myState = UiState::STATE_DEFAULT;
	if (!CheckWindowState(WS_ENABLE) || (state & UiState::STATE_DISABLED) == UiState::STATE_DISABLED) myState |= UiState::STATE_DISABLED;
 	if (CheckWindowState(WS_MOUSE_HOVER)) myState |= UiState::STATE_HOVER;
 	if (CheckWindowState(WS_MOUSE_DOWN)) myState |= UiState::STATE_DOWN;
	if (CheckWindowState(WS_FOCUS)) myState |= UiState::STATE_FOCUS;

	Render(basePos, clipRect, myState);

	Vector2 childBasePos = basePos + m_Position;

	Rect childClipRect;
	if (CheckWindowState(WS_CLIP_CHILDREN))
	{
		childClipRect = clipRect.Intersect(childBasePos, m_Size);
	}
	else
	{
		childClipRect = clipRect;
	}

	for (TV_WINDOW::const_iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		IWindow* pChild = (*it);
		pChild->SystemRender(childBasePos, childClipRect, myState);
	}
}

IWindow* IWindow::FindChildUnderPoint(const Vector2& pos)
{
	for (TV_WINDOW::const_reverse_iterator it = m_vChildren.rbegin(); it != m_vChildren.rend(); ++it)
	{
		IWindow* pChild = (*it);
		if (pChild->IsOnMe(pos)) return pChild;
	}

	return NULL;
}

bool IWindow::PreProcessMouseEvent(const MouseEvent& event)
{
	switch (event.GetMouseEventType())
	{
	case MouseEvent::MET_LBUTTON_DOWN:
		// set down state
		g_pUiSystemMgr->SetWindowDownState(this);
		// set focus state
		g_pUiSystemMgr->SetWindowFocusState(this);
		break;
	case MouseEvent::MET_LBUTTON_UP:
		// clear down state
		if (g_pUiSystemMgr->GetDownWindow() == this) g_pUiSystemMgr->SetWindowDownState(NULL);
		break;
	case MouseEvent::MET_MOUSE_MOVE:
		// set hover state
		g_pUiSystemMgr->SetWindowHoverState(this);
		// set down state
		if (g_pUiInputMgr->IsLButtonDown()) g_pUiSystemMgr->SetWindowDownState(this);
		break;
	}

	MouseEvent mouseEvent(EID_UI_PRE_MOUSE_EVENT, event.GetMouseEventType());
	mouseEvent.SetOffset(event.GetOffset());
	mouseEvent.SetWheelDetail(event.GetWheelDetail());

	mouseEvent.SetPosition(event.GetPosition());
	DispatchEvent(mouseEvent);
	return true;
}

bool IWindow::PostProcessMouseEvent(const MouseEvent& event)
{
	MouseEvent mouseEvent(EID_UI_POST_MOUSE_EVENT, event.GetMouseEventType());
	mouseEvent.SetOffset(event.GetOffset());
	mouseEvent.SetWheelDetail(event.GetWheelDetail());

	IWindow* pCurrWindow = this;
	IWindow* pChild = NULL;
	Vector2 localPos = event.GetPosition();

	while (pCurrWindow)
	{
		DispatchEvent(mouseEvent);

		pChild = pCurrWindow;
		pCurrWindow = pCurrWindow->m_pParent;

		if (pChild) localPos += pChild->GetPosition();
		mouseEvent.SetPosition(localPos);
	}

	return true;
}
