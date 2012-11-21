/*!
 * \file UiDesignerFrame.cpp
 * \date 11-21-2012 10:49:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "wx/wxprec.h"
#include "UiDesignerFrame.h"

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
