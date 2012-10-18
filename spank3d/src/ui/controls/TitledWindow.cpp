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

	RegisterEvent(EID_UI_PRE_MOUSE_EVENT, this, (FUNC_HANDLER)&TitledWindow::OnMouseEvent);
}

TitledWindow::~TitledWindow()
{
	// TODO: 
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

	pos.x = m_Position.x + m_pTitleStyle->GetPaddingLT().x;
	pos.y = m_Position.y + 0.5f*(m_pTitleStyle->GetPaddingLT().y + m_pTitleStyle->GetBestSize().y - m_pTitleStyle->GetPaddingRB().y - m_pFontStyle->GetLineHeight());
	m_pFontStyle->Render(_("TitledWindow"), basePos+pos, clipRect, state);
}

bool TitledWindow::OnMouseEvent(MouseEvent& event)
{
	switch (event.GetMouseEventType())
	{
	case MouseEvent::MET_LBUTTON_DOWN:
		m_LastMouseDownPos = g_pUiInputMgr->GetMousePos();
		m_LastMouseDownWindowPos = GetPosition();
		g_pUiInputMgr->CaptureMouse(this);
		break;
	case MouseEvent::MET_LBUTTON_UP:
		g_pUiInputMgr->ReleaseMouse();
		break;
	case MouseEvent::MET_MOUSE_MOVE:
		if (g_pUiInputMgr->IsLButtonDown())
		{
			Vector2 offset = (g_pUiInputMgr->GetMousePos() - m_LastMouseDownPos);
			SetPosition(m_LastMouseDownWindowPos+offset);
		}
		break;
	}

	return true;
}
