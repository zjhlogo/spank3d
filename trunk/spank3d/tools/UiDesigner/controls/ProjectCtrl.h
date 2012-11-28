/*!
 * \file ProjectCtrl.h
 * \date 11-22-2012 10:09:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PROJECTCTRL_H__
#define __PROJECTCTRL_H__

#include <wx/treectrl.h>
#include "../document/wxDocEvent.h"

class ProjectCtrl : public wxTreeCtrl
{
public:
	ProjectCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxTreeCtrlNameStr);

private:
	void OnDocumentReset(wxDocEvent& event);
	void ResetAllItems();

private:
	wxTreeItemId m_resRoot;

	wxTreeItemId m_styleRoot;
	wxTreeItemId m_styleBitmap;
	wxTreeItemId m_styleNinePatch;
	wxTreeItemId m_styleHorizontalPatch;
	wxTreeItemId m_styleVerticalPatch;
	wxTreeItemId m_styleBitmapFont;

	wxTreeItemId m_dialogRoot;

};
#endif // __PROJECTCTRL_H__
