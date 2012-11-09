/*!
 * \file Screen.cpp
 * \date 10-10-2012 10:31:22
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <ui/controls/Screen.h>

Screen::Screen(uint screenIndex)
:IWindow(NULL)
{
	m_nScreenIndex = screenIndex;
}

Screen::~Screen()
{
	// TODO: 
}

uint Screen::GetScreenIndex() const
{
	return m_nScreenIndex;
}

bool Screen::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	// TODO: 
	return true;
}
