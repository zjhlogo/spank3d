/*!
 * \file UiDesignerFrame.cpp
 * \date 11-21-2012 10:49:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "wx/wxprec.h"
#include "UiDesignerFrame.h"
#include "document/DocProject.h"

IMPLEMENT_CLASS(UiDesignerFrame, UiDesigner_ui)

BEGIN_EVENT_TABLE(UiDesignerFrame, UiDesigner_ui)
	EVT_MENU(IDM_FILE_OPEN_PROJECT, UiDesignerFrame::OnFileNewProjectClicked)
END_EVENT_TABLE()

UiDesignerFrame::UiDesignerFrame()
:UiDesigner_ui(NULL)
{
	m_auiManager.GetPane(_("toolbar")).Gripper(false);
	m_auiManager.Update();
}

UiDesignerFrame::~UiDesignerFrame()
{
	// TODO: 
}

void UiDesignerFrame::OnFileNewProjectClicked(wxCommandEvent& event)
{
	wxFileDialog dialog(this, _("Choose a file"), wxEmptyString, wxEmptyString, _("Project files (*.xml)|*.xml"), wxFD_DEFAULT_STYLE);
	if (dialog .ShowModal() == wxID_OK)
	{
		wxString strDir = dialog.GetDirectory();
		wxString strName = dialog.GetFilename();
		DocProject::GetInstance().Open(strDir, strName);
	}
}
