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
	return true;
}

void UiSystemMgr_Impl::Terminate()
{
	UiInputMgr_Impl::GetInstance().Terminate();
	UiResMgr_Impl::GetInstance().Terminate();
	UiRenderer_Impl::GetInstance().Terminate();
}

void UiSystemMgr_Impl::Update(float dt)
{
	// TODO: 
}

void UiSystemMgr_Impl::Render()
{
	// TODO: 
}
