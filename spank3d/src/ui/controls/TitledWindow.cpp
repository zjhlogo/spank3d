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
	m_pTitleBar = new TitleBar(this);
	m_pTitleBar->SetTargetWindow(this);
}

TitledWindow::~TitledWindow()
{
	// TODO: 
}

void TitledWindow::SetSize(float width, float height)
{
	IWindow::SetSize(width, height);
	const Vector2& barSize = m_pTitleBar->GetSize();
	m_pTitleBar->SetSize(width, m_pTitleBar->GetBgStyle()->GetBestSize().y);
}

void TitledWindow::SetTitle(const tstring& strTitle)
{
	m_pTitleBar->SetTitle(strTitle);
}

const tstring& TitledWindow::GetTitle() const
{
	return m_pTitleBar->GetTitle();
}

void TitledWindow::SetTitleStyle(const tstring& styleId)
{
	m_pTitleBar->SetBgStyle(styleId);
}

IGraphicsStyle* TitledWindow::GetTitleStyle()
{
	return m_pTitleBar->GetBgStyle();
}

bool TitledWindow::Render(const Vector2& basePos, const Rect& clipRect, uint state)
{
	if (!IWindow::Render(basePos, clipRect, state)) return true;
	return true;
}
