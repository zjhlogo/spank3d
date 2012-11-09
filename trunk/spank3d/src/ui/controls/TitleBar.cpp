/*!
 * \file TitleBar.cpp
 * \date 11-09-2012 13:05:05
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/TitleBar.h>
#include <Spank3d.h>

TitleBar::TitleBar(IWindow* parent)
:IWindow(parent)
{
	SetBgStyle(_("hps_title"));
	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
	m_bMouseDown = false;
	m_pTargetWindow = NULL;

	RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&TitleBar::OnMouseDown);
	RegisterEvent(MouseEvent::LBUTTON_UP, this, (FUNC_HANDLER)&TitleBar::OnMouseUp);
	RegisterEvent(MouseEvent::MOUSE_MOVE, this, (FUNC_HANDLER)&TitleBar::OnMouseMove);
}

TitleBar::~TitleBar()
{
	// TODO: 
}

void TitleBar::SetTitle(const tstring& strTitle)
{
	m_strTitle = strTitle;
}

const tstring& TitleBar::GetTitle() const
{
	return m_strTitle;
}

void TitleBar::SetTargetWindow(IWindow* target)
{
	m_pTargetWindow = target;
}

IWindow* TitleBar::GetTargetWindow()
{
	return m_pTargetWindow;
}

bool TitleBar::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!IWindow::Render(basePos, clipRect, state)) return true;

	if (!m_strTitle.empty())
	{
		Vector2 pos;
		pos.x = m_pBgStyle->GetPaddingLT().x;
		pos.y = 0.5f*(m_pBgStyle->GetPaddingLT().y + m_pBgStyle->GetBestSize().y - m_pBgStyle->GetPaddingRB().y - m_pFontStyle->GetLineHeight(state));
		m_pFontStyle->Render(m_strTitle, basePos+m_Position+pos, clipRect, state);
	}

	return true;
}

bool TitleBar::OnMouseDown(MouseEvent& event)
{
	m_LastMouseDownPos = g_pUiInputMgr->GetMousePos();
	if (m_pTargetWindow) m_LastTargetWindowPos = m_pTargetWindow->GetPosition();
	m_bMouseDown = true;
	g_pUiInputMgr->CaptureMouse(this);

	return true;
}

bool TitleBar::OnMouseUp(MouseEvent& event)
{
	m_bMouseDown = false;
	g_pUiInputMgr->ReleaseMouse();

	return true;
}

bool TitleBar::OnMouseMove(MouseEvent& event)
{
	if (m_bMouseDown && m_pTargetWindow)
	{
		Vector2 offset = (g_pUiInputMgr->GetMousePos() - m_LastMouseDownPos);
		m_pTargetWindow->SetPosition(m_LastTargetWindowPos+offset);
	}

	return true;
}
