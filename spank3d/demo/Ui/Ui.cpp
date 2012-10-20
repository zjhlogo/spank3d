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

IMPLEMENT_APP(Ui);

Ui::Ui()
{
	m_pWindow = NULL;
	m_pWindow2 = NULL;
	m_pButton = NULL;

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
	m_pWindow = new TitledWindow(pMainScreen);
	m_pWindow->SetPosition(200.0f, 200.0f);
	m_pWindow->SetSize(400.0f, 200.0f);

	m_pWindow2 = new TitledWindow(m_pWindow);
	m_pWindow2->SetPosition(50.0f, 50.0f);
	m_pWindow2->SetSize(200.0f, 100.0f);

	m_pButton = new PushButton(m_pWindow);
	m_pButton->SetPosition(200.0f, 10.0f);
	m_pButton->SetSize(100.0f, 30.0f);

	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
	if (!m_pFontStyle) return false;

	return true;
}

void Ui::Terminate()
{
	SAFE_DELETE(m_pWindow);
	SAFE_DELETE(m_pWindow2);
	SAFE_DELETE(m_pButton);
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
