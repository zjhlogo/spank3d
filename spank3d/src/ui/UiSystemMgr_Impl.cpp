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
