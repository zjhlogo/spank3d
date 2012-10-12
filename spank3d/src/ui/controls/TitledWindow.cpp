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
}

TitledWindow::~TitledWindow()
{
	// TODO: 
}

void TitledWindow::Render(uint state)
{
	Vector2 pos = m_Position;
	Vector2 size = m_Size;

	size.y = m_pTitleStyle->GetBestSize().y;
	m_pTitleStyle->Render(pos, size, state);

	pos.y += size.y;
	size.y = m_Size.y - m_pTitleStyle->GetBestSize().y;
	m_pBgStyle->Render(pos, size, state);

	pos.x = m_Position.x + m_pTitleStyle->GetPaddingLT().x;
	pos.y = m_Position.y + 0.5f*(m_pTitleStyle->GetPaddingLT().y + m_pTitleStyle->GetBestSize().y - m_pTitleStyle->GetPaddingRB().y - m_pFontStyle->GetLineHeight());
	m_pFontStyle->Render(_("TitledWindow"), pos, Rect(0.0f, 0.0f, 0.0f, 0.0f), state);
}
