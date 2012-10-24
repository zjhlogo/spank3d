/*!
 * \file Ui.cpp
 * \date 9-11-2012 20:56:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Ui.h"
#include <util/AppUtil.h>
#include <util/StringUtil.h>
#include <event/EventIds.h>
#include <ui/UiState.h>
#include <ui/controls/TitledWindow.h>
#include <ui/controls/PushButton.h>

IMPLEMENT_APP(Ui);

Ui::Ui()
{
	m_pFontStyle = NULL;
	m_nFPSFrame = 0;
	m_fFPSTime = 0.0f;
}

Ui::~Ui()
{
	// TODO: 
}

bool Ui::Initialize()
{
	Screen* pMainScreen = g_pUiSystemMgr->GetCurrScreen();
	TitledWindow* pWindow = new TitledWindow(pMainScreen);
	pWindow->SetPosition(200.0f, 200.0f);
	pWindow->SetSize(400.0f, 200.0f);

	TitledWindow* pWindow2 = new TitledWindow(pWindow);
	pWindow2->SetPosition(50.0f, 50.0f);
	pWindow2->SetSize(200.0f, 100.0f);

	PushButton* pButton = new PushButton(pWindow);
	pButton->SetPosition(200.0f, 10.0f);
	pButton->SetSize(100.0f, 30.0f);

	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
	if (!m_pFontStyle) return false;

//	g_pUiSystemMgr->LoadWindowFromFile(_("countdown.xml"), g_pUiSystemMgr->GetCurrScreen());
	return true;
}

void Ui::Terminate()
{
	// TODO: 
}

void Ui::Update(float dt)
{
	m_nFPSFrame++;
	m_fFPSTime += dt;
	if (m_fFPSTime > 1.0f)
	{
		m_fFPSTime -= 1.0f;
		StringUtil::strformat(m_strFPS, _T("%d FPS"), m_nFPSFrame);
		m_nFPSFrame = 0;
	}

	g_pUiSystemMgr->Update(dt);
}

void Ui::Render()
{
	g_pUiSystemMgr->Render();
	m_pFontStyle->Render(m_strFPS, Math::VEC2_ZERO, g_pUiSystemMgr->GetScreenRect(), UiState::STATE_DEFAULT);

	g_pUiRenderer->FlushAll();
}
