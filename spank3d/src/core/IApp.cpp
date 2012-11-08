/*!
 * \file IApp.cpp
 * \date 4-14-2012 22:13:50
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/IApp.h>
#include <Spank3d.h>
#include <ui/UiState.h>
#include <util/StringUtil.h>

IApp::IApp()
{
	g_pApp = this;

	m_pFontStyle = NULL;
	m_nFPSFrame = 0;
	m_fFPSTime = 0.0f;
}

IApp::~IApp()
{
	g_pApp = NULL;
}

bool IApp::Initialize()
{
	m_pFontStyle = g_pUiResMgr->FindFontStyle(_("12px_Tahoma"));
	if (!m_pFontStyle) return false;

	return true;
}

void IApp::Terminate()
{
	// TODO: 
}

void IApp::SystemUpdate(float dt)
{
	Update(dt);

	UpdateFps(dt);
	g_pUiSystemMgr->Update(dt);

	Render();

	g_pUiSystemMgr->Render();
	m_pFontStyle->Render(m_strFPS, Math::VEC2_ZERO, g_pUiSystemMgr->GetScreenRect(), UiState::STATE_DEFAULT);
	g_pUiRenderer->FlushAll();
}

void IApp::Run()
{
	// initialize core
	if (!Core::GetInstance().Initialize()) return;
	g_pCore->Run();
	Core::GetInstance().Terminate();
}

void IApp::UpdateFps(float dt)
{
	m_nFPSFrame++;
	m_fFPSTime += dt;
	if (m_fFPSTime > 1.0f)
	{
		m_fFPSTime -= 1.0f;
		StringUtil::strformat(m_strFPS, _T("FPS: %d"), m_nFPSFrame);
		m_nFPSFrame = 0;
	}
}
