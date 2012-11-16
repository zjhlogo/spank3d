/*!
 * \file PushButton.cpp
 * \date 10-18-2012 15:13:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/PushButton.h>
#include <ui/UiState.h>
#include <Spank3d.h>

PushButton::PushButton(IWindow* pParent)
:Label(pParent, true)
{
	m_paddingLT.Reset(5.0f, 1.0f);
	m_paddingRB.Reset(5.0f, 1.0f);
}

PushButton::~PushButton()
{
	// TODO: 
}

bool PushButton::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!IWindow::Render(basePos, clipRect, state)) return true;

	if (!m_strLabel.empty())
	{
		Vector2 clientSize = m_Size - m_pBgStyle->GetPaddingLT() - m_pBgStyle->GetPaddingRB() - m_paddingLT - m_paddingRB;
		Vector2 pos = m_Position + m_pBgStyle->GetPaddingLT() + m_paddingLT + (clientSize - m_TextSize) * 0.5f;
		m_pFontStyle->Render(m_strLabel, basePos+pos, clipRect, state);
	}

	return true;
}

void PushButton::AdjustSize()
{
	SetSize(m_TextSize+m_pBgStyle->GetPaddingLT()+m_pBgStyle->GetPaddingRB()+m_paddingLT+m_paddingRB);
}
