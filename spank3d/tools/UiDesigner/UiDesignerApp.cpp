/*!
 * \file UiDesignerApp.cpp
 * \date 11-21-2012 10:47:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "wx/wxprec.h"
#include "UiDesignerApp.h"
#include "UiDesignerFrame.h"

IMPLEMENT_APP(UiDesignerApp)

UiDesignerApp::UiDesignerApp()
{
	// TODO: 
}

UiDesignerApp::~UiDesignerApp()
{
	// TODO: 
}

bool UiDesignerApp::OnInit()
{
	wxInitAllImageHandlers();

	m_Local.Init();
	wxLocale::AddCatalogLookupPathPrefix(wxT("."));
	m_Local.AddCatalog(wxT("zh_cn"));

	UiDesignerFrame* mainWindow = new UiDesignerFrame();
	mainWindow->Maximize(true);
	mainWindow->Show(true);

	return true;
}
