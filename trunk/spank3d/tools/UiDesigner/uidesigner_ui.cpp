/////////////////////////////////////////////////////////////////////////////
// Name:        uidesigner_ui.cpp
// Purpose:     
// Author:      zjhlogo
// Modified by: 
// Created:     21/11/2012 14:12:52
// RCS-ID:      
// Copyright:   zjhlogo (C) 2011-2012
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "uidesigner_ui.h"

////@begin XPM images
#include "images/doc_new.xpm"
#include "images/folder_open.xpm"
#include "images/save.xpm"
#include "images/save_as.xpm"
////@end XPM images


/*
 * UiDesigner_ui type definition
 */

IMPLEMENT_CLASS( UiDesigner_ui, wxFrame )


/*
 * UiDesigner_ui event table definition
 */

BEGIN_EVENT_TABLE( UiDesigner_ui, wxFrame )

////@begin UiDesigner_ui event table entries
////@end UiDesigner_ui event table entries

END_EVENT_TABLE()


/*
 * UiDesigner_ui constructors
 */

UiDesigner_ui::UiDesigner_ui()
{
	Init();
}

UiDesigner_ui::UiDesigner_ui( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create( parent, id, caption, pos, size, style );
}


/*
 * UiDesigner_ui creator
 */

bool UiDesigner_ui::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin UiDesigner_ui creation
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	Centre();
////@end UiDesigner_ui creation
	return true;
}


/*
 * UiDesigner_ui destructor
 */

UiDesigner_ui::~UiDesigner_ui()
{
////@begin UiDesigner_ui destruction
	GetAuiManager().UnInit();
////@end UiDesigner_ui destruction
}


/*
 * Member initialisation
 */

void UiDesigner_ui::Init()
{
////@begin UiDesigner_ui member initialisation
	m_noteSolution = NULL;
	m_noteOutput = NULL;
	m_noteCanvas = NULL;
	m_panelProperty = NULL;
////@end UiDesigner_ui member initialisation
}


/*
 * Control creation for UiDesigner_ui
 */

void UiDesigner_ui::CreateControls()
{    
////@begin UiDesigner_ui content construction
	UiDesigner_ui* itemFrame1 = this;

	GetAuiManager().SetManagedWindow(this);

	wxMenuBar* menuBar = new wxMenuBar;
	wxMenu* itemMenu3 = new wxMenu;
	{
		wxMenuItem* menuItem = new wxMenuItem(itemMenu3, IDM_FILE_NEW_PROJECT, _("&New Project...\tCtrl+N"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("images/doc_new.xpm")));
		menuItem->SetBitmap(bitmap);
		itemMenu3->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(itemMenu3, IDM_FILE_OPEN_PROJECT, _("&Open Project...\tCtrl+O"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("images/folder_open.xpm")));
		menuItem->SetBitmap(bitmap);
		itemMenu3->Append(menuItem);
	}
	itemMenu3->AppendSeparator();
	itemMenu3->Append(IDM_FILE_CLOSE_PROJECT, _("&Close Project"), wxEmptyString, wxITEM_NORMAL);
	itemMenu3->Append(IDM_FILE_CLOSE_ALL, _("C&lose All"), wxEmptyString, wxITEM_NORMAL);
	itemMenu3->AppendSeparator();
	{
		wxMenuItem* menuItem = new wxMenuItem(itemMenu3, IDM_FILE_SAVE_PROJECT, _("&Save Project...\tCtrl+S"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("images/save.xpm")));
		menuItem->SetBitmap(bitmap);
		itemMenu3->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(itemMenu3, IDM_FILE_SAVE_PROJECT_AS, _("Save Project &As...\tCtrl+Shift+S"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(itemFrame1->GetBitmapResource(wxT("images/save_as.xpm")));
		menuItem->SetBitmap(bitmap);
		itemMenu3->Append(menuItem);
	}
	itemMenu3->AppendSeparator();
	itemMenu3->Append(IDM_FILE_EXIT, _("E&xit\tAlt+F4"), wxEmptyString, wxITEM_NORMAL);
	menuBar->Append(itemMenu3, _("&File"));
	wxMenu* itemMenu14 = new wxMenu;
	itemMenu14->Append(IDM_HELP_ABOUT, _("&About\tF1"), wxEmptyString, wxITEM_NORMAL);
	menuBar->Append(itemMenu14, _("&Help"));
	itemFrame1->SetMenuBar(menuBar);

	wxToolBar* itemToolBar16 = new wxToolBar( itemFrame1, IDT_MAIN_TOOLBAR, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
	itemToolBar16->SetToolBitmapSize(wxSize(16, 16));
	wxBitmap itemtool17Bitmap(itemFrame1->GetBitmapResource(wxT("images/doc_new.xpm")));
	wxBitmap itemtool17BitmapDisabled;
	itemToolBar16->AddTool(IDM_FILE_NEW_PROJECT, wxEmptyString, itemtool17Bitmap, itemtool17BitmapDisabled, wxITEM_NORMAL, _("New Project (Ctrl+N)"), wxEmptyString);
	wxBitmap itemtool18Bitmap(itemFrame1->GetBitmapResource(wxT("images/folder_open.xpm")));
	wxBitmap itemtool18BitmapDisabled;
	itemToolBar16->AddTool(IDM_FILE_OPEN_PROJECT, wxEmptyString, itemtool18Bitmap, itemtool18BitmapDisabled, wxITEM_NORMAL, _("Open Project (Ctrl+O)"), wxEmptyString);
	itemToolBar16->AddSeparator();
	wxBitmap itemtool20Bitmap(itemFrame1->GetBitmapResource(wxT("images/save.xpm")));
	wxBitmap itemtool20BitmapDisabled;
	itemToolBar16->AddTool(IDM_FILE_SAVE_PROJECT, wxEmptyString, itemtool20Bitmap, itemtool20BitmapDisabled, wxITEM_NORMAL, _("Save Project (Ctrl+S)"), wxEmptyString);
	wxBitmap itemtool21Bitmap(itemFrame1->GetBitmapResource(wxT("images/save_as.xpm")));
	wxBitmap itemtool21BitmapDisabled;
	itemToolBar16->AddTool(IDM_FILE_SAVE_PROJECT_AS, wxEmptyString, itemtool21Bitmap, itemtool21BitmapDisabled, wxITEM_NORMAL, _("Save Project As (Ctrl+Shift+S)"), wxEmptyString);
	itemToolBar16->Realize();
	itemFrame1->GetAuiManager().AddPane(itemToolBar16, wxAuiPaneInfo()
		.ToolbarPane().Name(_T("toolbar")).Top().Layer(10).CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(false).Floatable(false).Movable(false).PaneBorder(false));

	m_noteSolution = new wxNotebook( itemFrame1, IDC_SOLUTION, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	m_noteSolution->SetName(_T("solution"));

	wxTreeCtrl* itemTreeCtrl23 = new wxTreeCtrl( m_noteSolution, IDC_PROJECT, wxDefaultPosition, m_noteSolution->ConvertDialogToPixels(wxSize(66, 57)), wxTR_SINGLE );

	m_noteSolution->AddPage(itemTreeCtrl23, _("Project"));

	wxTreeCtrl* itemTreeCtrl24 = new wxTreeCtrl( m_noteSolution, IDC_ELEMENTS, wxDefaultPosition, m_noteSolution->ConvertDialogToPixels(wxSize(66, 57)), wxTR_SINGLE );

	m_noteSolution->AddPage(itemTreeCtrl24, _("Elements"));

	wxTreeCtrl* itemTreeCtrl25 = new wxTreeCtrl( m_noteSolution, IDC_RESOURCES, wxDefaultPosition, m_noteSolution->ConvertDialogToPixels(wxSize(66, 57)), wxTR_SINGLE );

	m_noteSolution->AddPage(itemTreeCtrl25, _("Resources"));

	itemFrame1->GetAuiManager().AddPane(m_noteSolution, wxAuiPaneInfo()
		.Name(_T("solution")).Caption(_("Solution")).MinSize(wxSize(300, 800)).Layer(1).DestroyOnClose(false).Resizable(true).PaneBorder(false).PinButton(true));

	m_noteOutput = new wxNotebook( itemFrame1, IDC_OUTPUT, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	m_noteOutput->SetName(_T("output"));

	wxArrayString itemListBox27Strings;
	wxListBox* itemListBox27 = new wxListBox( m_noteOutput, IDC_OUTPUT1, wxDefaultPosition, wxDefaultSize, itemListBox27Strings, wxLB_SINGLE );

	m_noteOutput->AddPage(itemListBox27, _("Output 1"));

	wxArrayString itemListBox28Strings;
	wxListBox* itemListBox28 = new wxListBox( m_noteOutput, IDC_OUTPUT2, wxDefaultPosition, wxDefaultSize, itemListBox28Strings, wxLB_SINGLE );

	m_noteOutput->AddPage(itemListBox28, _("Output 2"));

	itemFrame1->GetAuiManager().AddPane(m_noteOutput, wxAuiPaneInfo()
		.Name(_T("output")).Caption(_("Output")).Bottom().MinSize(wxSize(500, 200)).DestroyOnClose(false).Resizable(true).PaneBorder(false).PinButton(true));

	m_noteCanvas = new wxAuiNotebook( itemFrame1, IDC_CANVAS, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE|wxNO_BORDER );
	m_noteCanvas->SetName(_T("canvas"));

	wxPanel* itemPanel30 = new wxPanel( m_noteCanvas, IDC_DIALOG1, wxDefaultPosition, wxDefaultSize, 0 );

	m_noteCanvas->AddPage(itemPanel30, _("Dialog 1 *"), false);

	wxPanel* itemPanel31 = new wxPanel( m_noteCanvas, IDC_RESOURCE1, wxDefaultPosition, wxDefaultSize, 0 );

	m_noteCanvas->AddPage(itemPanel31, _("Resource 1 *"), false);

	itemFrame1->GetAuiManager().AddPane(m_noteCanvas, wxAuiPaneInfo()
		.Name(_T("ID_AUINOTEBOOK")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false).PaneBorder(false));

	m_panelProperty = new wxPanel( itemFrame1, IDC_PROPERTY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_panelProperty->SetName(_T("property"));
	itemFrame1->GetAuiManager().AddPane(m_panelProperty, wxAuiPaneInfo()
		.Name(_T("property")).Caption(_("Property")).Right().MinSize(wxSize(300, 800)).DestroyOnClose(false).Resizable(true).Floatable(false).PaneBorder(false).PinButton(true));

	GetAuiManager().Update();

////@end UiDesigner_ui content construction
}


/*
 * Should we show tooltips?
 */

bool UiDesigner_ui::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap UiDesigner_ui::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin UiDesigner_ui bitmap retrieval
	wxUnusedVar(name);
	if (name == _T("images/doc_new.xpm"))
	{
		wxBitmap bitmap(doc_new_xpm);
		return bitmap;
	}
	else if (name == _T("images/folder_open.xpm"))
	{
		wxBitmap bitmap(folder_open_xpm);
		return bitmap;
	}
	else if (name == _T("images/save.xpm"))
	{
		wxBitmap bitmap(save_xpm);
		return bitmap;
	}
	else if (name == _T("images/save_as.xpm"))
	{
		wxBitmap bitmap(save_as_xpm);
		return bitmap;
	}
	return wxNullBitmap;
////@end UiDesigner_ui bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon UiDesigner_ui::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin UiDesigner_ui icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end UiDesigner_ui icon retrieval
}
