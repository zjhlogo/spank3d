/*!
 * \file Label.cpp
 * \date 11-08-2012 14:38:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/Label.h>
#include <Spank3d.h>

Label::Label(IWindow* pParent)
:IWindow(pParent)
{
	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
}

Label::~Label()
{
	// TODO: 
}

void Label::SetLabel(const tstring& strLabel)
{
	m_strLabel = strLabel;
}

const tstring& Label::GetLabel() const
{
	return m_strLabel;
}

bool Label::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!m_strLabel.empty())
	{
		Vector2 textSize = m_pFontStyle->CalcSize(m_strLabel, state);
		Vector2 pos = m_Position + (m_Size - textSize) * 0.5f;
		pos.x = m_Position.x;
		m_pFontStyle->Render(m_strLabel, basePos+pos, clipRect, state);
	}

	return true;
}
