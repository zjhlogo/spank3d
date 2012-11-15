/*!
 * \file Label.cpp
 * \date 11-08-2012 14:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/Label.h>
#include <ui/UiState.h>
#include <Spank3d.h>

Label::Label(IWindow* pParent, const tstring& strLabel /* = EMPTY_STRING */)
:IWindow(pParent)
{
	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
	SetWindowState(WS_SOLID, false);
	SetLabel(strLabel);
}

Label::~Label()
{
	// TODO: 
}

void Label::SetLabel(const tstring& strLabel)
{
	m_strLabel = strLabel;
	m_TextSize = m_pFontStyle->CalcSize(m_strLabel, UiState::STATE_DEFAULT);
	SetSize(m_TextSize);
}

const tstring& Label::GetLabel() const
{
	return m_strLabel;
}

bool Label::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!m_strLabel.empty())
	{
		Vector2 pos = m_Position + (m_Size - m_TextSize) * 0.5f;
		pos.x = m_Position.x;
		m_pFontStyle->Render(m_strLabel, basePos+pos, clipRect, state);
	}

	return true;
}
