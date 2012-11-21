/*!
 * \file ImagePackerApp.cpp
 * \date 2-18-2012 9:43:02
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ImagePackerApp.h"
#include "ImagePackerFrame.h"

IMPLEMENT_APP(ImagePackerApp)

ImagePackerApp::ImagePackerApp()
{
	// TODO: 
}

ImagePackerApp::~ImagePackerApp()
{
	// TODO: 
}

bool ImagePackerApp::OnInit()
{
	wxImage::AddHandler(new wxICOHandler());
	wxImage::AddHandler(new wxXPMHandler());
	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxJPEGHandler());
	wxImage::AddHandler(new wxCURHandler());

	m_Local.Init();
	wxLocale::AddCatalogLookupPathPrefix(wxT("."));
	m_Local.AddCatalog(wxT("zh_cn"));

	ImagePackerFrame* mainWindow = new ImagePackerFrame();
	mainWindow->Maximize(true);
	mainWindow->Show(true);

	return true;
}
