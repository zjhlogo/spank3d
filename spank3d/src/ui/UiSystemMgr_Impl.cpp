/*!
 * \file UiSystemMgr_Impl.cpp
 * \date 10-10-2012 9:20:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiSystemMgr_Impl.h"
#include "UiRenderer_Impl.h"
#include "UiResMgr_Impl.h"
#include "UiInputMgr_Impl.h"
#include <Spank3d.h>

UiSystemMgr_Impl::UiSystemMgr_Impl()
{
	g_pUiSystemMgr = this;
	m_pCurrScreen = NULL;

	m_pDownWindow = NULL;
	m_pHoverWindow = NULL;
	m_pFocusWindow = NULL;
}

UiSystemMgr_Impl::~UiSystemMgr_Impl()
{
	g_pUiSystemMgr = NULL;
}

UiSystemMgr_Impl& UiSystemMgr_Impl::GetInstance()
{
	static UiSystemMgr_Impl s_UiSystemMgr_Impl;
	return s_UiSystemMgr_Impl;
}

bool UiSystemMgr_Impl::Initialize()
{
	if (!UiRenderer_Impl::GetInstance().Initialize()) return false;
	if (!UiResMgr_Impl::GetInstance().Initialize()) return false;
	if (!UiInputMgr_Impl::GetInstance().Initialize()) return false;

	SwitchScreen(CreateScreen()->GetScreenIndex());
	return true;
}

void UiSystemMgr_Impl::Terminate()
{
	DestroyScreens();
	UiInputMgr_Impl::GetInstance().Terminate();
	UiResMgr_Impl::GetInstance().Terminate();
	UiRenderer_Impl::GetInstance().Terminate();
}

Screen* UiSystemMgr_Impl::CreateScreen()
{
	Screen* pScreen = new Screen(m_vScreen.size());
	m_vScreen.push_back(pScreen);
	return pScreen;
}

Screen* UiSystemMgr_Impl::GetScreen(uint index)
{
	if (index >= m_vScreen.size()) return NULL;
	return m_vScreen[index];
}

bool UiSystemMgr_Impl::SwitchScreen(uint index)
{
	Screen* pScreen = GetScreen(index);
	if (!pScreen) return false;

	m_pCurrScreen = pScreen;
	return true;
}

Screen* UiSystemMgr_Impl::GetCurrScreen()
{
	return m_pCurrScreen;
}

void UiSystemMgr_Impl::Update(float dt)
{
	// TODO: 
}

void UiSystemMgr_Impl::Render()
{
	// TODO: 
}

void UiSystemMgr_Impl::DestroyScreens()
{
	for (TV_SCREEN::iterator it = m_vScreen.begin(); it != m_vScreen.end(); ++it)
	{
		Screen* pScreen = (*it);
		SAFE_DELETE(pScreen);
	}
	m_vScreen.clear();
	m_pCurrScreen = NULL;
}

void UiSystemMgr_Impl::SetWindowDownState(IWindow* pWindow)
{
	if (m_pDownWindow == pWindow) return;

	if (m_pDownWindow)
	{
		m_pDownWindow->SetWindowState(IWindow::WS_MOUSE_DOWN, false);
		m_pDownWindow->UnregisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnDownWindowDestroyed);
	}

	m_pDownWindow = pWindow;

	if (m_pDownWindow)
	{
		m_pDownWindow->SetWindowState(IWindow::WS_MOUSE_DOWN, true);
		m_pDownWindow->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnDownWindowDestroyed);
	}
}

void UiSystemMgr_Impl::SetWindowHoverState(IWindow* pWindow)
{
	if (m_pHoverWindow == pWindow) return;

	if (m_pHoverWindow)
	{
		m_pHoverWindow->SetWindowState(IWindow::WS_MOUSE_HOVER, false);
		m_pHoverWindow->UnregisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnHoverWindowDestroyed);
	}

	m_pHoverWindow = pWindow;

	if (m_pHoverWindow)
	{
		m_pHoverWindow->SetWindowState(IWindow::WS_MOUSE_HOVER, true);
		m_pHoverWindow->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnHoverWindowDestroyed);
	}
}

void UiSystemMgr_Impl::SetWindowFocusState(IWindow* pWindow)
{
	if (m_pFocusWindow == pWindow) return;

	if (m_pFocusWindow)
	{
		m_pFocusWindow->SetWindowState(IWindow::WS_FOCUS, false);
		m_pFocusWindow->UnregisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnFocusWindowDestroyed);
	}

	m_pFocusWindow = pWindow;
	if (m_pFocusWindow)
	{
		m_pFocusWindow->SetWindowState(IWindow::WS_FOCUS, true);
		m_pFocusWindow->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnFocusWindowDestroyed);
	}
}

IWindow* UiSystemMgr_Impl::GetDownWindow()
{
	return m_pDownWindow;
}

IWindow* UiSystemMgr_Impl::GetHoverWindow()
{
	return m_pHoverWindow;
}

IWindow* UiSystemMgr_Impl::GetFocusWindow()
{
	return m_pFocusWindow;
}

bool UiSystemMgr_Impl::OnDownWindowDestroyed(Event& event)
{
	m_pDownWindow = NULL;
	return true;
}

bool UiSystemMgr_Impl::OnHoverWindowDestroyed(Event& event)
{
	m_pHoverWindow = NULL;
	return true;
}

bool UiSystemMgr_Impl::OnFocusWindowDestroyed(Event& event)
{
	m_pFocusWindow = NULL;
	return true;
}
