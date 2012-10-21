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

void PushButton::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	m_pBgStyle->Render(basePos+m_Position, m_Size, clipRect, state);

	Vector2 textSize = m_pFontStyle->CalcSize(_("PushButton"), state);
	Vector2 pos = m_Position + (m_Size - textSize) * 0.5f;
	m_pFontStyle->Render(_("PushButton"), basePos+pos, clipRect, state);
}
