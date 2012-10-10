/*!
 * \file UiInputMgr_Impl.cpp
 * \date 10-10-2012 9:32:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiInputMgr_Impl.h"
#include <Spank3d.h>

UiInputMgr_Impl::UiInputMgr_Impl()
{
	g_pUiInputMgr = this;
}

UiInputMgr_Impl::~UiInputMgr_Impl()
{
	g_pUiInputMgr = NULL;
}

UiInputMgr_Impl& UiInputMgr_Impl::GetInstance()
{
	static UiInputMgr_Impl s_UiInputMgr_Impl;
	return s_UiInputMgr_Impl;
}

bool UiInputMgr_Impl::Initialize()
{
	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	return true;
}

void UiInputMgr_Impl::Terminate()
{
	// TODO: 
}

bool UiInputMgr_Impl::OnMouseEvent(MouseEvent& event)
{
	// TODO: 
	return true;
}
