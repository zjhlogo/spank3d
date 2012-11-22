/*!
 * \file UiDesignerFrame.h
 * \date 11-21-2012 10:48:05
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIDESIGNERFRAME_H__
#define __UIDESIGNERFRAME_H__

#include <wx/wx.h>
#include "uidesigner_ui.h"

class UiDesignerFrame : public UiDesigner_ui
{
	DECLARE_CLASS(UiDesignerFrame)
	DECLARE_EVENT_TABLE()

public:
	UiDesignerFrame();
	virtual ~UiDesignerFrame();

private:
	void OnFileNewProjectClicked(wxCommandEvent& event);

};
#endif // __UIDESIGNERFRAME_H__
