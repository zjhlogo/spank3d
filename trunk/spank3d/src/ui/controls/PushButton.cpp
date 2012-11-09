/*!
 * \file PushButton.cpp
 * \date 10-18-2012 15:13:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/PushButton.h>
#include <Spank3d.h>

PushButton::PushButton(IWindow* pParent)
:IWindow(pParent)
{
	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
}

PushButton::~PushButton()
{
	// TODO: 
}

void PushButton::SetLabel(const tstring& strLabel)
{
	m_strLabel = strLabel;
}

const tstring& PushButton::GetLabel() const
{
	return m_strLabel;
}

bool PushButton::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!IWindow::Render(basePos, clipRect, state)) return true;

	if (!m_strLabel.empty())
	{
		Vector2 textSize = m_pFontStyle->CalcSize(m_strLabel, state);
		Vector2 pos = m_Position + (m_Size - textSize) * 0.5f;
		m_pFontStyle->Render(m_strLabel, basePos+pos, clipRect, state);
	}

	return true;
}
