/*!
 * \file TitledWindow.cpp
 * \date 10-01-2012 8:34:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/TitledWindow.h>
#include <Spank3d.h>

TitledWindow::TitledWindow(IWindow* parent)
:IWindow(parent)
{
	m_pTitleStyle = g_pUiResMgr->FindHorizontalPatchStyle(_("hps_title"));
	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
	m_bMouseDown = false;

	RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&TitledWindow::OnMouseDown);
	RegisterEvent(MouseEvent::LBUTTON_UP, this, (FUNC_HANDLER)&TitledWindow::OnMouseUp);
	RegisterEvent(MouseEvent::MOUSE_MOVE, this, (FUNC_HANDLER)&TitledWindow::OnMouseMove);
}

TitledWindow::~TitledWindow()
{
	// TODO: 
}

void TitledWindow::SetTitle(const tstring& strTitle)
{
	m_strTitle = strTitle;
}

const tstring& TitledWindow::GetTitle() const
{
	return m_strTitle;
}

void TitledWindow::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	Vector2 pos = m_Position;
	Vector2 size = m_Size;

	size.y = m_pTitleStyle->GetBestSize().y;
	m_pTitleStyle->Render(basePos+pos, size, clipRect, state);

	pos.y += size.y;
	size.y = m_Size.y - m_pTitleStyle->GetBestSize().y;
	m_pBgStyle->Render(basePos+pos, size, clipRect, state);

	if (!m_strTitle.empty())
	{
		pos.x = m_Position.x + m_pTitleStyle->GetPaddingLT().x;
		pos.y = m_Position.y + 0.5f*(m_pTitleStyle->GetPaddingLT().y + m_pTitleStyle->GetBestSize().y - m_pTitleStyle->GetPaddingRB().y - m_pFontStyle->GetLineHeight(state));
		m_pFontStyle->Render(m_strTitle, basePos+pos, clipRect, state);
	}
}

bool TitledWindow::OnMouseDown(MouseEvent& event)
{
	m_LastMouseDownPos = g_pUiInputMgr->GetMousePos();
	m_LastMouseDownWindowPos = GetPosition();
	m_bMouseDown = true;
	g_pUiInputMgr->CaptureMouse(this);

	return true;
}

bool TitledWindow::OnMouseUp(MouseEvent& event)
{
	m_bMouseDown = false;
	g_pUiInputMgr->ReleaseMouse();

	return true;
}

bool TitledWindow::OnMouseMove(MouseEvent& event)
{
	if (m_bMouseDown)
	{
		Vector2 offset = (g_pUiInputMgr->GetMousePos() - m_LastMouseDownPos);
		SetPosition(m_LastMouseDownWindowPos+offset);
	}

	return true;
}
