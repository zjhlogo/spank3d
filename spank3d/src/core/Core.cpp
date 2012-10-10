/*!
 * \file Core.cpp
 * \date 4-14-2012 22:16:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/Core.h>
#include <Spank3D.h>

#include "ResMgr_Impl.h"
#include "Device_Impl.h"
#include "../ui/UiSystemMgr_Impl.h"

Core::Core()
{
	g_pCore = this;
}

Core::~Core()
{
	g_pCore = NULL;
}

Core& Core::GetInstance()
{
	static Core s_Core;
	return s_Core;
}

bool Core::Initialize()
{
	if (!ResMgr_Impl::GetInstance().Initialize()) return false;
	if (!Device_Impl::GetInstance().Initialize()) return false;
	if (!UiSystemMgr_Impl::GetInstance().Initialize()) return false;
	if (!g_pApp->Initialize()) return false;

	return true;
}

void Core::Terminate()
{
	g_pApp->Terminate();
	UiSystemMgr_Impl::GetInstance().Terminate();
	Device_Impl::GetInstance().Terminate();
	ResMgr_Impl::GetInstance().Terminate();
}

void Core::Run()
{
	g_pDevice->StartPerform();
}

void Core::End()
{
	g_pDevice->EndPerform();
}
