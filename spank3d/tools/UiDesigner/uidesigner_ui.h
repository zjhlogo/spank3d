/////////////////////////////////////////////////////////////////////////////
// Name:        uidesigner_ui.h
// Purpose:     
// Author:      zjhlogo
// Modified by: 
// Created:     21/11/2012 17:43:55
// RCS-ID:      
// Copyright:   zjhlogo (C) 2011-2012
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _UIDESIGNER_UI_H_
#define _UIDESIGNER_UI_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/aui/framemanager.h"
#include "wx/frame.h"
#include "wx/toolbar.h"
#include "wx/notebook.h"
#include "wx/treectrl.h"
#include "wx/aui/auibook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
class wxAuiNotebook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_UIDESIGNER_UI_STYLE wxDEFAULT_FRAME_STYLE
#define SYMBOL_UIDESIGNER_UI_TITLE _("Ui Designer")
#define SYMBOL_UIDESIGNER_UI_IDNAME ID_UIDESIGNER
#define SYMBOL_UIDESIGNER_UI_SIZE wxSize(682, 342)
#define SYMBOL_UIDESIGNER_UI_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * UiDesigner_ui class declaration
 */

class UiDesigner_ui: public wxFrame
{    
	DECLARE_CLASS( UiDesigner_ui )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	UiDesigner_ui();
	UiDesigner_ui( wxWindow* parent, wxWindowID id = SYMBOL_UIDESIGNER_UI_IDNAME, const wxString& caption = SYMBOL_UIDESIGNER_UI_TITLE, const wxPoint& pos = SYMBOL_UIDESIGNER_UI_POSITION, const wxSize& size = SYMBOL_UIDESIGNER_UI_SIZE, long style = SYMBOL_UIDESIGNER_UI_STYLE );

	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_UIDESIGNER_UI_IDNAME, const wxString& caption = SYMBOL_UIDESIGNER_UI_TITLE, const wxPoint& pos = SYMBOL_UIDESIGNER_UI_POSITION, const wxSize& size = SYMBOL_UIDESIGNER_UI_SIZE, long style = SYMBOL_UIDESIGNER_UI_STYLE );

	/// Destructor
	~UiDesigner_ui();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin UiDesigner_ui event handler declarations

////@end UiDesigner_ui event handler declarations

////@begin UiDesigner_ui member function declarations

	/// Returns the AUI manager object
	wxAuiManager& GetAuiManager() { return m_auiManager; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end UiDesigner_ui member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin UiDesigner_ui member variables
	wxAuiManager m_auiManager;
	wxNotebook* m_noteSolution;
	wxNotebook* m_noteOutput;
	wxAuiNotebook* m_noteCanvas;
	wxPanel* m_panelProperty;
	/// Control identifiers
	enum {
		ID_UIDESIGNER = 10000,
		IDM_FILE_NEW_PROJECT = 10002,
		IDM_FILE_OPEN_PROJECT = 10003,
		IDM_FILE_CLOSE_PROJECT = 10004,
		IDM_FILE_CLOSE_ALL = 10005,
		IDM_FILE_SAVE_PROJECT = 10006,
		IDM_FILE_SAVE_PROJECT_AS = 10007,
		IDM_FILE_EXIT = 10008,
		IDM_HELP_ABOUT = 10009,
		IDT_MAIN_TOOLBAR = 10014,
		IDC_SOLUTION = 10001,
		IDC_PROJECT = 10016,
		IDC_ELEMENTS = 10017,
		IDC_RESOURCES = 10018,
		IDC_OUTPUT = 10027,
		IDC_OUTPUT1 = 10028,
		IDC_OUTPUT2 = 10029,
		IDC_CANVAS = 10010,
		IDC_DIALOG1 = 10012,
		IDC_RESOURCE1 = 10013,
		IDC_PROPERTY = 10011
	};
////@end UiDesigner_ui member variables
};

#endif
	// _UIDESIGNER_UI_H_
