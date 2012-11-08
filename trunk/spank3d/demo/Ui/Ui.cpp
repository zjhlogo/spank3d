/*!
 * \file Ui.cpp
 * \date 9-11-2012 20:56:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Ui.h"
#include <util/AppUtil.h>
#include <ui/controls/TitledWindow.h>
#include <ui/controls/PushButton.h>

IMPLEMENT_APP(Ui);

Ui::Ui()
{
	// TODO: 
}

Ui::~Ui()
{
	// TODO: 
}

bool Ui::Initialize()
{
	if (!IApp::Initialize()) return false;

	Screen* pMainScreen = g_pUiSystemMgr->GetCurrScreen();
	TitledWindow* pWindow = new TitledWindow(pMainScreen);
	pWindow->SetPosition(200.0f, 200.0f);
	pWindow->SetSize(400.0f, 200.0f);
	pWindow->SetTitle(_T("Title Window"));

	TitledWindow* pWindow2 = new TitledWindow(pWindow);
	pWindow2->SetPosition(50.0f, 50.0f);
	pWindow2->SetSize(200.0f, 100.0f);
	pWindow2->SetTitle(_T("Title Window"));

	PushButton* pButton = new PushButton(pWindow);
	pButton->SetPosition(200.0f, 10.0f);
	pButton->SetSize(100.0f, 30.0f);
	pButton->SetLabel(_T("Push Button"));

	return true;
}

void Ui::Terminate()
{
	IApp::Terminate();
}

void Ui::Update(float dt)
{

}

void Ui::Render()
{

}
