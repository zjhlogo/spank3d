/*!
 * \file IApp.cpp
 * \date 4-14-2012 22:13:50
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <core/IApp.h>
#include <Spank3D.h>

IApp::IApp()
{
	g_pApp = this;
}

IApp::~IApp()
{
	g_pApp = NULL;
}

void IApp::Run()
{
	// initialize core
	if (!Core::GetInstance().Initialize()) return;
	g_pCore->Run();
	Core::GetInstance().Terminate();
}
