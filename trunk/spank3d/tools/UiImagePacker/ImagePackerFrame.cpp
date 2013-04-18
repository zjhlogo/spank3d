/*!
 * \file ImagePackerFrame.cpp
 * \date 2-18-2012 9:44:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ImagePackerFrame.h"

#include "resource.h"
#include "Config.h"
#include "document/ProjectDocument.h"
#include "document/ImagePieceDocument.h"
#include "document/BitmapStyleDocument.h"
#include "document/NineGridStyleDocument.h"
#include "document/ColorStyleDocument.h"
#include "document/ClipBitmapStyleDocument.h"

#include "transformer/ImageListTransformer.h"
#include "transformer/PieceListTransformer.h"
#include "transformer/BitmapStyleTransformer.h"
#include "transformer/NineGridStyleTransformer.h"
#include "transformer/ColorStyleTransformer.h"
#include "transformer/ClipBitmapStyleTransformer.h"

#include "editor/ImagePieceEditor.h"
#include "editor/BitmapStyleEditor.h"
#include "editor/NineGridStyleEditor.h"
#include "editor/ColorStyleEditor.h"
#include "editor/ClipBitmapStyleEditor.h"

#include "dialog/DialogAddPiece.h"
#include "dialog/DialogReplacePiece.h"
#include "dialog/DialogMovePiece.h"
#include "utils/FileUtil.h"

#include "images/icon.xpm"

#include "images/ico_clip_16x16.xpm"
#include "images/ico_color_16x16.xpm"
#include "images/ico_image_16x16.xpm"
#include "images/ico_nine_16x16.xpm"
#include "images/ico_open_16x16.xpm"
#include "images/ico_replace_16x16.xpm"
#include "images/ico_save_16x16.xpm"
#include "images/ico_slice_16x16.xpm"
#include "images/ico_zoom_fix_16x16.xpm"
#include "images/ico_zoom_in_16x16.xpm"
#include "images/ico_zoom_out_16x16.xpm"

#include "images/ico_clip_32x32.xpm"
#include "images/ico_color_32x32.xpm"
#include "images/ico_image_32x32.xpm"
#include "images/ico_nine_32x32.xpm"
#include "images/ico_open_32x32.xpm"
#include "images/ico_replace_32x32.xpm"
#include "images/ico_save_32x32.xpm"
#include "images/ico_slice_32x32.xpm"
#include "images/ico_zoom_fix_32x32.xpm"
#include "images/ico_zoom_in_32x32.xpm"
#include "images/ico_zoom_out_32x32.xpm"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

BEGIN_EVENT_TABLE(ImagePackerFrame, wxFrame)
	EVT_CLOSE(ImagePackerFrame::OnClose)

	EVT_MENU(wxID_OPEN, ImagePackerFrame::OnFileOpenProject)
	EVT_MENU(wxID_SAVE, ImagePackerFrame::OnFileSaveProject)
	EVT_MENU(wxID_EXIT, ImagePackerFrame::OnFileExit)

	EVT_MENU(IDM_EDIT_DELETE, ImagePackerFrame::OnEditDelete)

	EVT_MENU(IDM_ELEMENT_ADD_PIECE, ImagePackerFrame::OnAddPiece)
	EVT_MENU(IDM_ELEMENT_ADD_BITMAP_STYLE, ImagePackerFrame::OnAddBitmapStyle)
	EVT_MENU(IDM_ELEMENT_ADD_NINE_GRID_STYLE, ImagePackerFrame::OnAddNineGridStyle)
	EVT_MENU(IDM_ELEMENT_ADD_COLOR_STYLE, ImagePackerFrame::OnAddColorStyle)
	EVT_MENU(IDM_ELEMENT_ADD_CLIP_BITMAP_STYLE, ImagePackerFrame::OnAddClipBitmapStyle)
	EVT_MENU(IDM_ELEMENT_REPLACE_PIECE, ImagePackerFrame::OnReplacePiece)
	EVT_MENU(IDM_ELEMENT_MOVE_PIECE, ImagePackerFrame::OnMovePiece)

	EVT_MENU(wxID_ZOOM_100, ImagePackerFrame::OnViewZoom100)
	EVT_MENU(wxID_ZOOM_IN, ImagePackerFrame::OnViewZoomIn)
	EVT_MENU(wxID_ZOOM_OUT, ImagePackerFrame::OnViewZoomOut)

	EVT_MENU(IDM_TOOL_EXTRACT_IMAGE, ImagePackerFrame::OnExtractImage)

	EVT_MENU(IDM_HELP_ABOUT, ImagePackerFrame::OnHelpAbout)

	EVT_TREE_SEL_CHANGED(IDC_PIECE_LIST, ImagePackerFrame::OnImagePieceListSelected)
	EVT_TREE_SEL_CHANGED(IDC_IMAGE_LIST, ImagePackerFrame::OnImageListSelected)
	EVT_TREE_SEL_CHANGED(IDC_BITMAP_STYLE_LIST, ImagePackerFrame::OnBitmapStyleListSelected)
	EVT_TREE_SEL_CHANGED(IDC_NINE_GRID_STYLE_LIST, ImagePackerFrame::OnNineGridStyleListSelected)
	EVT_TREE_SEL_CHANGED(IDC_COLOR_STYLE_LIST, ImagePackerFrame::OnColorStyleListSelected)
	EVT_TREE_SEL_CHANGED(IDC_CLIP_BITMAP_STYLE_LIST, ImagePackerFrame::OnClipBitmapStyleListSelected)

	EVT_PG_CHANGED(IDC_PROPERTY, ImagePackerFrame::OnPropertyGridChanged)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(ImagePackerFrame, wxFrame)

ImagePackerFrame* ImagePackerFrame::m_pImagePackerFrame = NULL;

ImagePackerFrame::ImagePackerFrame()
:wxFrame(NULL, wxID_ANY, _("Ui Image Packer"), wxDefaultPosition, wxSize(800, 600))
{
	Init();
	CreateControls();
}

ImagePackerFrame::~ImagePackerFrame()
{
	Release();
}

ImagePackerFrame& ImagePackerFrame::GetInstance()
{
	return *m_pImagePackerFrame;
}

void ImagePackerFrame::Init()
{
	m_pImagePackerFrame = this;
	m_pOutputView = NULL;
	m_eCurrPropertyType = PT_UNKNOWN;
	m_pPropertyGrid = NULL;
	m_pListNotebook = NULL;
	m_pEditorNotebook = NULL;
	m_eCurrEditor = NUM_EDITOR;
	memset(m_pEditors, 0, sizeof(m_pEditors));
	m_bTitleModifiedFlag = false;

	SetIcon(wxIcon(icon_xpm));
}

void ImagePackerFrame::Release()
{
	m_auiManager.UnInit();
	m_pImagePackerFrame = NULL;
}

void ImagePackerFrame::SetCurrPropertyType(PROPERTY_TYPE eType)
{
	m_eCurrPropertyType = eType;
}

ImagePackerFrame::PROPERTY_TYPE ImagePackerFrame::GetCurrPropertyType() const
{
	return m_eCurrPropertyType;
}

void ImagePackerFrame::UpdateUiModified(bool bModified, bool bForce /* = false */)
{
	if (!bForce && m_bTitleModifiedFlag == bModified) return;
	m_bTitleModifiedFlag = bModified;
	if (m_bTitleModifiedFlag)
	{
		SetTitle(FileUtil::GetFileName(ProjectDocument::GetInstance().GetFilePath()) + wxT(" *"));
	}
	else
	{
		SetTitle(FileUtil::GetFileName(ProjectDocument::GetInstance().GetFilePath()));
	}
}

void ImagePackerFrame::CreateControls()
{
	m_auiManager.SetManagedWindow(this);

	CreateMenu();
	CreateToolbar();
	CreatePropertyView();
	CreateListView();
	CreateEditorView();
	CreateOutputView();

	m_auiManager.Update();
}

void ImagePackerFrame::CreateMenu()
{
	wxMenuBar* pMenuBar = new wxMenuBar();

	// file
	wxMenu* pMenuItemFile = new wxMenu();
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemFile, wxID_OPEN, _("&Open Project...\tCtrl+O"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_open_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemFile->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemFile, wxID_SAVE, _("&Save Project\tCtrl+S"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_save_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemFile->Append(menuItem);
	}

	pMenuItemFile->AppendSeparator();
	pMenuItemFile->Append(wxID_EXIT, _("E&xit\tAlt+F4"), wxEmptyString, wxITEM_NORMAL);
	pMenuBar->Append(pMenuItemFile, _("&File"));

	// edit
	wxMenu* pMenuItemEdit = new wxMenu();
	pMenuItemEdit->Append(IDM_EDIT_UNDO, _("&Undo\tCtrl+Z"), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuItemEdit->Append(IDM_EDIT_REDO, _("&Redo\tCtrl+Y"), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuItemEdit->AppendSeparator();
	pMenuItemEdit->Append(IDM_EDIT_COPY, _("&Copy\tCtrl+C"), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuItemEdit->Append(IDM_EDIT_CUT, _("Cu&t\tCtrl+X"), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuItemEdit->Append(IDM_EDIT_PAST, _("&Past\tCtrl+V"), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuItemEdit->Append(IDM_EDIT_DELETE, _("&Delete"), wxEmptyString, wxITEM_NORMAL);
	pMenuBar->Append(pMenuItemEdit, _("&Edit"));

	// elements
	wxMenu* pMenuItemElement = new wxMenu();
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_ADD_PIECE, _("Add &Piece..."), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_slice_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemElement->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_ADD_BITMAP_STYLE, _("Add New &Bitmap Style"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_image_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemElement->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_ADD_NINE_GRID_STYLE, _("Add New 9-&Grid Style"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_nine_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemElement->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_ADD_COLOR_STYLE, _("Add New &Color Style"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_color_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemElement->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_ADD_CLIP_BITMAP_STYLE, _("Add New C&lip Bitmap Style"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_clip_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemElement->Append(menuItem);
	}
	pMenuItemElement->AppendSeparator();
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_REPLACE_PIECE, _("&Replace Pieces..."), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_replace_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemElement->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemElement, IDM_ELEMENT_MOVE_PIECE, _("&Move Pieces..."), wxEmptyString, wxITEM_NORMAL);
		pMenuItemElement->Append(menuItem);
	}
	pMenuBar->Append(pMenuItemElement, _("E&lement"));

	// view
	wxMenu* pMenuItemView = new wxMenu();
	pMenuItemView->Append(IDM_VIEW_GRID, _("Show &Grid\tCtrl+G"), wxEmptyString, wxITEM_CHECK)->Enable(false);
	pMenuItemView->Check(IDM_VIEW_GRID, true);
	pMenuItemView->AppendSeparator();
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemView, wxID_ZOOM_100, _("Zoom &100%\tCtrl+1"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_zoom_fix_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemView->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemView, wxID_ZOOM_IN, _("Zoom &In\tCtrl++"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_zoom_in_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemView->Append(menuItem);
	}
	{
		wxMenuItem* menuItem = new wxMenuItem(pMenuItemView, wxID_ZOOM_OUT, _("Zoom &Out\tCtrl+-"), wxEmptyString, wxITEM_NORMAL);
		wxBitmap bitmap(ico_zoom_out_16x16_xpm, wxBITMAP_TYPE_XPM);
		menuItem->SetBitmap(bitmap);
		pMenuItemView->Append(menuItem);
	}
	pMenuBar->Append(pMenuItemView, _("&View"));

	// tool
	wxMenu* pMenuItemTool = new wxMenu();
	pMenuItemTool->Append(IDM_TOOL_EXTRACT_IMAGE, _("&Extract Image Pieces..."), wxEmptyString, wxITEM_NORMAL);
	pMenuItemTool->Append(IDM_TOOL_PREFERENCES, _("&Preferences..."), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuBar->Append(pMenuItemTool, _("&Tools"));

	// help
	wxMenu* pMenuItemHelp = new wxMenu();
	pMenuItemHelp->Append(IDM_HELP_SUPPORT, _("&Support"), wxEmptyString, wxITEM_NORMAL)->Enable(false);
	pMenuItemHelp->AppendSeparator();
	pMenuItemHelp->Append(IDM_HELP_ABOUT, _("&About\tF1"), wxEmptyString, wxITEM_NORMAL);
	pMenuBar->Append(pMenuItemHelp, _("&Help"));

	SetMenuBar(pMenuBar);
}

void ImagePackerFrame::CreateToolbar()
{
	wxAuiToolBar* pAuiToolBar = new wxAuiToolBar(this, IDC_TOOLBAR, wxDefaultPosition, wxDefaultSize, 0);

	{
		wxBitmap bitmap(ico_open_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(wxID_OPEN, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("open project file ( Ctrl+O )"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_save_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(wxID_SAVE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("save project file ( Ctrl+S )"), wxEmptyString, NULL);
	}

	pAuiToolBar->AddSeparator();

	{
		wxBitmap bitmap(ico_slice_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(IDM_ELEMENT_ADD_PIECE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("add new piece"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_image_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(IDM_ELEMENT_ADD_BITMAP_STYLE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("add new bitmap style"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_nine_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(IDM_ELEMENT_ADD_NINE_GRID_STYLE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("add new 9-grid style"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_color_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(IDM_ELEMENT_ADD_COLOR_STYLE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("add new color style"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_clip_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(IDM_ELEMENT_ADD_CLIP_BITMAP_STYLE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("add new clip bitmap style"), wxEmptyString, NULL);
	}

	pAuiToolBar->AddSeparator();

	{
		wxBitmap bitmap(ico_replace_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(IDM_ELEMENT_REPLACE_PIECE, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("replace pieces"), wxEmptyString, NULL);
	}

	pAuiToolBar->AddSeparator();

	{
		wxBitmap bitmap(ico_zoom_fix_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(wxID_ZOOM_100, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("zoom 100% ( Ctrl+1 )"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_zoom_in_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(wxID_ZOOM_IN, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("zoom in ( Ctrl++ )"), wxEmptyString, NULL);
	}

	{
		wxBitmap bitmap(ico_zoom_out_32x32_xpm, wxBITMAP_TYPE_XPM);
		wxBitmap bitmapDisabled;
		pAuiToolBar->AddTool(wxID_ZOOM_OUT, wxEmptyString, bitmap, bitmapDisabled, wxITEM_NORMAL, _("zoom out ( Ctrl+- )"), wxEmptyString, NULL);
	}

	pAuiToolBar->Realize();

	m_auiManager.AddPane(pAuiToolBar, wxAuiPaneInfo()
		.Name(_("Toolbar"))
		.Top()
		.Layer(1)
		.Dockable(false)
		.CaptionVisible(false)
		.CloseButton(false)
		.DestroyOnClose(false)
		.Resizable(true)
		.Floatable(false)
		.Movable(false)
		.PaneBorder(false));
}

void ImagePackerFrame::CreatePropertyView()
{
	m_pPropertyGrid = new wxPropertyGrid(this, IDC_PROPERTY, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER|wxNO_BORDER);
	m_auiManager.AddPane(m_pPropertyGrid, wxAuiPaneInfo()
		.Name(_("Property"))
		.Caption(_("Property"))
		.Right()
		.BestSize(wxSize(300, 300))
		.Position(1)
		.CloseButton(false)
		.DestroyOnClose(false)
		.Resizable(true)
		.Floatable(false)
		.FloatingSize(wxSize(300, 500))
		.Movable(false));
}

void ImagePackerFrame::CreateListView()
{
	m_pListNotebook = new wxNotebook(this, IDC_NOTEBOOK_EDITOR, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNO_BORDER);
	m_auiManager.AddPane(m_pListNotebook, wxAuiPaneInfo()
		.Name(_("List"))
		.Caption(_("List"))
		.Left()
		.Layer(1)
		.BestSize(wxSize(400, 500))
		.CaptionVisible(true)
		.CloseButton(false)
		.DestroyOnClose(false)
		.Resizable(true)
		.Floatable(false)
		.FloatingSize(wxSize(400, 500))
		.Movable(false));

	wxTreeCtrl* pImageListView = new wxTreeCtrl(m_pListNotebook, IDC_IMAGE_LIST, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	pImageListView->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnListKeyDown), NULL, this);
	m_pListNotebook->AddPage(pImageListView, _("Images"));
	ImageListTransformer::GetInstance().Initialize(pImageListView, m_pPropertyGrid);

	wxTreeCtrl* pPieceListView = new wxTreeCtrl(m_pListNotebook, IDC_PIECE_LIST, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	pPieceListView->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnListKeyDown), NULL, this);
	m_pListNotebook->AddPage(pPieceListView, _("Pieces"));
	PieceListTransformer::GetInstance().Initialize(pPieceListView, m_pPropertyGrid);

	wxTreeCtrl* pBitmapStyleListView = new wxTreeCtrl(m_pListNotebook, IDC_BITMAP_STYLE_LIST, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	pBitmapStyleListView->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnListKeyDown), NULL, this);
	m_pListNotebook->AddPage(pBitmapStyleListView, _("Bitmap Styles"));
	BitmapStyleTransformer::GetInstance().Initialize(pBitmapStyleListView, m_pPropertyGrid);

	wxTreeCtrl* pNineGridStyleListView = new wxTreeCtrl(m_pListNotebook, IDC_NINE_GRID_STYLE_LIST, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	pNineGridStyleListView->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnListKeyDown), NULL, this);
	m_pListNotebook->AddPage(pNineGridStyleListView, _("9-Grid Styles"));
	NineGridStyleTransformer::GetInstance().Initialize(pNineGridStyleListView, m_pPropertyGrid);

	wxTreeCtrl* pColorStyleListView = new wxTreeCtrl(m_pListNotebook, IDC_COLOR_STYLE_LIST, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	pColorStyleListView->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnListKeyDown), NULL, this);
	m_pListNotebook->AddPage(pColorStyleListView, _("Color Styles"));
	ColorStyleTransformer::GetInstance().Initialize(pColorStyleListView, m_pPropertyGrid);

	wxTreeCtrl* pClipBitmapStyleListView = new wxTreeCtrl(m_pListNotebook, IDC_CLIP_BITMAP_STYLE_LIST, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	pClipBitmapStyleListView->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnListKeyDown), NULL, this);
	m_pListNotebook->AddPage(pClipBitmapStyleListView, _("Clip Bitmap Styles"));
	ClipBitmapStyleTransformer::GetInstance().Initialize(pClipBitmapStyleListView, m_pPropertyGrid);
}

void ImagePackerFrame::CreateEditorView()
{
	m_pEditorNotebook = new wxNotebook(this, IDC_NOTEBOOK_EDITOR, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	m_auiManager.AddPane(m_pEditorNotebook, wxAuiPaneInfo()
		.Name(_("Editor"))
		.Caption(_("Editor"))
		.Centre()
		.CaptionVisible(false)
		.CloseButton(false)
		.DestroyOnClose(false)
		.Resizable(true)
		.Floatable(false)
		.Movable(false));

	ImagePieceEditor* pImagePieceEditor = new ImagePieceEditor(m_pEditorNotebook, IDC_IMAGE_PIECE_EDITOR_VIEW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFRAME_NO_TASKBAR);
	pImagePieceEditor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnEditorKeyDown), NULL, this);
	m_pEditorNotebook->AddPage(pImagePieceEditor, _("Pieces Editor"));
	m_pEditors[EDITOR_IMAGE_PIECE] = pImagePieceEditor;

	BitmapStyleEditor* pBitmapStyleEditor = new BitmapStyleEditor(m_pEditorNotebook, IDC_BITMAP_STYLE_EDITOR_VIEW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFRAME_NO_TASKBAR);
	pBitmapStyleEditor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnEditorKeyDown), NULL, this);
	m_pEditorNotebook->AddPage(pBitmapStyleEditor, _("Bitmap Style Editor"));
	m_pEditors[EDITOR_BITMAP_STYLE] = pBitmapStyleEditor;

	NineGridStyleEditor* pNineGridStyleEditor = new NineGridStyleEditor(m_pEditorNotebook, IDC_NINE_GRID_STYLE_EDITOR_VIEW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFRAME_NO_TASKBAR);
	pNineGridStyleEditor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnEditorKeyDown), NULL, this);
	m_pEditorNotebook->AddPage(pNineGridStyleEditor, _("Nine Grid Style Editor"));
	m_pEditors[EDITOR_NINE_GRID_STYLE] = pNineGridStyleEditor;

	ColorStyleEditor* pColorStyleEditor = new ColorStyleEditor(m_pEditorNotebook, IDC_COLOR_STYLE_EDITOR_VIEW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFRAME_NO_TASKBAR);
	pColorStyleEditor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnEditorKeyDown), NULL, this);
	m_pEditorNotebook->AddPage(pColorStyleEditor, _("Color Style Editor"));
	m_pEditors[EDITOR_COLOR_STYLE] = pColorStyleEditor;

	ClipBitmapStyleEditor* pClipBitmapStyleEditor = new ClipBitmapStyleEditor(m_pEditorNotebook, IDC_CLIP_BITMAP_STYLE_EDITOR_VIEW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxFRAME_NO_TASKBAR);
	pClipBitmapStyleEditor->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ImagePackerFrame::OnEditorKeyDown), NULL, this);
	m_pEditorNotebook->AddPage(pClipBitmapStyleEditor, _("Clip Bitmap Style Editor"));
	m_pEditors[EDITOR_CLIP_BITMAP_STYLE] = pClipBitmapStyleEditor;
}

void ImagePackerFrame::CreateOutputView()
{
	m_pOutputView = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_MULTILINE|wxNO_BORDER);
	m_pOutputView->AppendText(_("This is output view\n"));
	m_pOutputView->AppendText(_("line 1\n"));
	m_pOutputView->AppendText(_("line 2\n"));

	m_auiManager.AddPane(m_pOutputView, wxAuiPaneInfo()
		.Name(_("Output"))
		.Caption(_("Output"))
		.Bottom()
		.BestSize(wxSize(300, 300))
		.CloseButton(false)
		.DestroyOnClose(false)
		.Resizable(true)
		.Floatable(false)
		.FloatingSize(wxSize(500, 200))
		.Movable(false));
}

void ImagePackerFrame::OnFileOpenProject(wxCommandEvent& event)
{
	if (IsModified())
	{
		wxMessageDialog dialog(this, _("Do you want to save the Project ?"), _("Save Project"), wxYES_NO|wxCANCEL);
		switch (dialog .ShowModal())
		{
		case wxID_YES:
			DoSaveProject();
			break;
		case wxID_NO:
			DoOpenProject();
			break;
		case wxID_CANCEL:
			break;
		}
	}
	else
	{
		DoOpenProject();
	}
}

void ImagePackerFrame::DoOpenProject()
{
	wxString strCurrDir;
	FileUtil::GetCurrDir(strCurrDir);

	wxFileDialog dialog(this, _("Choose a file"), strCurrDir, wxEmptyString, _("Project files (*.ipproj)|*.ipproj"), wxFD_DEFAULT_STYLE);
	if (dialog .ShowModal() == wxID_OK)
	{
		SwitchEditor(NUM_EDITOR);

		wxString strPath = dialog.GetPath();
		wxString strDir = dialog.GetDirectory();
		ProjectDocument::GetInstance().SetProjectDir(strDir);
		if (ProjectDocument::GetInstance().OpenFile(strPath))
		{
			ImagePieceDocument::GetInstance().OpenFile(ProjectDocument::GetInstance().GetImagePieceFilePath());
			BitmapStyleDocument::GetInstance().OpenFile(ProjectDocument::GetInstance().GetBitmapStyleFilePath());
			NineGridStyleDocument::GetInstance().OpenFile(ProjectDocument::GetInstance().GetNineGridStyleFilePath());
			ColorStyleDocument::GetInstance().OpenFile(ProjectDocument::GetInstance().GetColorStyleFilePath());
			ClipBitmapStyleDocument::GetInstance().OpenFile(ProjectDocument::GetInstance().GetClipBitmapStyleFilePath());

			ImageListTransformer::GetInstance().UpdateListView();
			PieceListTransformer::GetInstance().UpdateListView();
			BitmapStyleTransformer::GetInstance().UpdateListView();
			NineGridStyleTransformer::GetInstance().UpdateListView();
			ColorStyleTransformer::GetInstance().UpdateListView();
			ClipBitmapStyleTransformer::GetInstance().UpdateListView();

			UpdateUiModified(IsModified(), true);
		}
	}
}

void ImagePackerFrame::OnFileSaveProject(wxCommandEvent& event)
{
	DoSaveProject();
}

void ImagePackerFrame::DoSaveProject()
{
	ProjectDocument::GetInstance().SaveFile(ProjectDocument::GetInstance().GetFilePath());
	ImagePieceDocument::GetInstance().SaveFile(ProjectDocument::GetInstance().GetImagePieceFilePath());
	BitmapStyleDocument::GetInstance().SaveFile(ProjectDocument::GetInstance().GetBitmapStyleFilePath());
	NineGridStyleDocument::GetInstance().SaveFile(ProjectDocument::GetInstance().GetNineGridStyleFilePath());
	ColorStyleDocument::GetInstance().SaveFile(ProjectDocument::GetInstance().GetColorStyleFilePath());
	ClipBitmapStyleDocument::GetInstance().SaveFile(ProjectDocument::GetInstance().GetClipBitmapStyleFilePath());
	UpdateUiModified(false);
}

void ImagePackerFrame::OnFileExit(wxCommandEvent& event)
{
	Close();
}

void ImagePackerFrame::OnClose(wxCloseEvent& event)
{
	if (IsModified())
	{
		wxMessageDialog dialog(this, _("Do you want to save the Project ?"), _("Save Project"), wxYES_NO|wxCANCEL);
		switch (dialog .ShowModal())
		{
		case wxID_YES:
			DoSaveProject();
			event.Skip();
			break;
		case wxID_NO:
			event.Skip();
			break;
		case wxID_CANCEL:
			break;
		}
	}
	else
	{
		event.Skip();
	}
}

bool ImagePackerFrame::IsModified() const
{
	if (ProjectDocument::GetInstance().IsModified()) return true;
	if (ImagePieceDocument::GetInstance().IsModified()) return true;
	if (BitmapStyleDocument::GetInstance().IsModified()) return true;
	if (NineGridStyleDocument::GetInstance().IsModified()) return true;
	if (ColorStyleDocument::GetInstance().IsModified()) return true;
	if (ClipBitmapStyleDocument::GetInstance().IsModified()) return true;

	return false;
}

void ImagePackerFrame::OnEditDelete(wxCommandEvent& event)
{
	DoDelete();
}

void ImagePackerFrame::DoDelete()
{
	if (ProjectDocument::GetInstance().GetFilePath().empty())
	{
		wxMessageDialog msg(this, _("Please open a project first"));
		msg.ShowModal();
		return;
	}

	switch (m_eCurrEditor)
	{
	case EDITOR_IMAGE_PIECE:
		{
			const PieceInfo* pPieceInfo = ImagePieceEditor::GetInstance().GetSelPieceInfo();
			const ImageInfo* pImageInfo = ImagePieceEditor::GetInstance().GetImageInfo();
			if (pPieceInfo) DoDeletePiece(pPieceInfo);
			else if (pImageInfo) DoDeleteImage(pImageInfo);
		}
		break;
	case EDITOR_BITMAP_STYLE:
		{
			const BitmapStyle* pBitmapStyle = BitmapStyleEditor::GetInstance().GetBitmapStyle();
			if (pBitmapStyle) DoDeleteBitmapStyle(pBitmapStyle);
		}
		break;
	case EDITOR_NINE_GRID_STYLE:
		{
			const NineGridStyle* pNineGridStyle = NineGridStyleEditor::GetInstance().GetNineGridStyle();
			if (pNineGridStyle) DoDeleteNineGridStyle(pNineGridStyle);
		}
		break;
	case EDITOR_COLOR_STYLE:
		{
			const ColorStyle* pColorStyle = ColorStyleEditor::GetInstance().GetColorStyle();
			if (pColorStyle) DoDeleteColorStyle(pColorStyle);
		}
		break;
	case EDITOR_CLIP_BITMAP_STYLE:
		{
			const ClipBitmapStyle* pClipBitmapStyle = ClipBitmapStyleEditor::GetInstance().GetClipBitmapStyle();
			if (pClipBitmapStyle) DoDeleteClipBitmapStyle(pClipBitmapStyle);
		}
		break;
	}
}

bool ImagePackerFrame::FindStyleIdsUsingThePiece(TS_WX_STRING& vBitmapStyleIds, TS_WX_STRING& vNineGridStyleIds, TS_WX_STRING& vClipBitmapStyleIds, const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return false;

	BitmapStyleDocument::TV_BITMAP_STYLE vBitmapStyles;
	if (BitmapStyleDocument::GetInstance().EnumBitmapStyles(vBitmapStyles, pPieceInfo) > 0)
	{
		for (BitmapStyleDocument::TV_BITMAP_STYLE::iterator it = vBitmapStyles.begin(); it != vBitmapStyles.end(); ++it)
		{
			vBitmapStyleIds.insert((*it)->GetId());
		}
	}

	NineGridStyleDocument::TV_NINE_GRID_STYLE vNineGridStyles;
	if (NineGridStyleDocument::GetInstance().EnumNineGridStyles(vNineGridStyles, pPieceInfo) > 0)
	{
		for (NineGridStyleDocument::TV_NINE_GRID_STYLE::iterator it = vNineGridStyles.begin(); it != vNineGridStyles.end(); ++it)
		{
			vNineGridStyleIds.insert((*it)->GetId());
		}
	}

	ClipBitmapStyleDocument::TV_CLIP_BITMAP_STYLE vClipBitmapStyles;
	if (ClipBitmapStyleDocument::GetInstance().EnumClipBitmapStyles(vClipBitmapStyles, pPieceInfo) > 0)
	{
		for (ClipBitmapStyleDocument::TV_CLIP_BITMAP_STYLE::iterator it = vClipBitmapStyles.begin(); it != vClipBitmapStyles.end(); ++it)
		{
			vClipBitmapStyleIds.insert((*it)->GetId());
		}
	}

	return true;
}

bool ImagePackerFrame::MakeIdsString(wxString& strOut, const TS_WX_STRING& vIds, const wxString& strHeader)
{
	if (vIds.size() <= 0) return false;

	strOut += wxString::Format(wxT("\n\n%s\n"), strHeader);
	for (TS_WX_STRING::const_iterator it = vIds.begin(); it != vIds.end(); ++it)
	{
		strOut += wxString::Format(wxT("\"%s\" "), (*it));
	}

	return true;
}

void ImagePackerFrame::DoDeletePiece(const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return;

	// find styles using the piece
	TS_WX_STRING vBitmapStyleIds;
	TS_WX_STRING vNineGridStyleIds;
	TS_WX_STRING vClipBitmapStyleIds;
	FindStyleIdsUsingThePiece(vBitmapStyleIds, vNineGridStyleIds, vClipBitmapStyleIds, pPieceInfo);

	// make prompt string
	wxString strDeleteStyles;
	MakeIdsString(strDeleteStyles, vBitmapStyleIds, _("also delete bitmap styles:"));
	MakeIdsString(strDeleteStyles, vNineGridStyleIds, _("also delete 9-grid styles:"));
	MakeIdsString(strDeleteStyles, vClipBitmapStyleIds, _("also delete clip bitmap styles:"));

	// prompt
	wxString wxMsg = wxString::Format(_("Do you want to delete the piece: %s ?%s"), pPieceInfo->GetId(), strDeleteStyles);
	wxMessageDialog msgPrompt(this, wxMsg, _("Delete Piece"), wxYES_NO|wxCENTER);
	if (msgPrompt.ShowModal() != wxID_YES) return;

	// delete bitmap styles
	for (TS_WX_STRING::const_iterator it = vBitmapStyleIds.begin(); it != vBitmapStyleIds.end(); ++it)
	{
		BitmapStyleDocument::GetInstance().RemoveBitmapStyle((*it));
	}
	if (vBitmapStyleIds.size() > 0) BitmapStyleTransformer::GetInstance().UpdateListView();

	// delete nine grid styles
	for (TS_WX_STRING::const_iterator it = vNineGridStyleIds.begin(); it != vNineGridStyleIds.end(); ++it)
	{
		NineGridStyleDocument::GetInstance().RemoveNineGridStyle((*it));
	}
	if (vNineGridStyleIds.size() > 0) NineGridStyleTransformer::GetInstance().UpdateListView();

	// delete clip bitmap styles
	for (TS_WX_STRING::const_iterator it = vClipBitmapStyleIds.begin(); it != vClipBitmapStyleIds.end(); ++it)
	{
		ClipBitmapStyleDocument::GetInstance().RemoveClipBitmapStyle((*it));
	}
	if (vClipBitmapStyleIds.size() > 0) ClipBitmapStyleTransformer::GetInstance().UpdateListView();

	// delete the piece
	ImagePieceEditor::GetInstance().SetSelPieceInfo(NULL);
	ImagePieceDocument::GetInstance().RemovePiece(pPieceInfo->GetId());
	PieceListTransformer::GetInstance().UpdateListView();
	PieceListTransformer::GetInstance().UpdateProperty(NULL);
}

void ImagePackerFrame::DoDeleteImage(const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return;

	TS_WX_STRING vPieceInfoIds;
	TS_WX_STRING vBitmapStyleIds;
	TS_WX_STRING vNineGridStyleIds;
	TS_WX_STRING vClipBitmapStyleIds;

	// find pieces and styles using the image
	ImagePieceDocument::TV_PIECE_INFO vPieceInfo;
	if (ImagePieceDocument::GetInstance().EnumImagePieces(vPieceInfo, pImageInfo) > 0)
	{
		for (ImagePieceDocument::TV_PIECE_INFO::iterator it = vPieceInfo.begin(); it != vPieceInfo.end(); ++it)
		{
			const PieceInfo* pPieceInfo = (*it);
			vPieceInfoIds.insert(pPieceInfo->GetId());
			FindStyleIdsUsingThePiece(vBitmapStyleIds, vNineGridStyleIds, vClipBitmapStyleIds, pPieceInfo);
		}
	}

	// make prompt string
	wxString strDeletePiecesAndStyles;
	MakeIdsString(strDeletePiecesAndStyles, vPieceInfoIds, _("also delete pieces:"));
	MakeIdsString(strDeletePiecesAndStyles, vBitmapStyleIds, _("also delete bitmap styles:"));
	MakeIdsString(strDeletePiecesAndStyles, vNineGridStyleIds, _("also delete 9-grid styles:"));
	MakeIdsString(strDeletePiecesAndStyles, vClipBitmapStyleIds, _("also delete clip bitmap styles:"));

	// prompt
	wxString wxMsg = wxString::Format(_("Do you want to delete the image: %s ?%s"), pImageInfo->GetId(), strDeletePiecesAndStyles);
	wxMessageDialog msgPrompt(this, wxMsg, _("Delete Image"), wxYES_NO|wxCENTER);
	if (msgPrompt.ShowModal() != wxID_YES) return;

	// delete bitmap styles
	for (TS_WX_STRING::const_iterator it = vBitmapStyleIds.begin(); it != vBitmapStyleIds.end(); ++it)
	{
		BitmapStyleDocument::GetInstance().RemoveBitmapStyle((*it));
	}
	if (vBitmapStyleIds.size() > 0) BitmapStyleTransformer::GetInstance().UpdateListView();

	// delete nine grid styles
	for (TS_WX_STRING::const_iterator it = vNineGridStyleIds.begin(); it != vNineGridStyleIds.end(); ++it)
	{
		NineGridStyleDocument::GetInstance().RemoveNineGridStyle((*it));
	}
	if (vNineGridStyleIds.size() > 0) NineGridStyleTransformer::GetInstance().UpdateListView();

	// delete clip bitmap styles
	for (TS_WX_STRING::const_iterator it = vClipBitmapStyleIds.begin(); it != vClipBitmapStyleIds.end(); ++it)
	{
		ClipBitmapStyleDocument::GetInstance().RemoveClipBitmapStyle((*it));
	}
	if (vClipBitmapStyleIds.size() > 0) ClipBitmapStyleTransformer::GetInstance().UpdateListView();

	// delete the pieces
	for (TS_WX_STRING::const_iterator it = vPieceInfoIds.begin(); it != vPieceInfoIds.end(); ++it)
	{
		ImagePieceDocument::GetInstance().RemovePiece((*it));
	}
	if (vPieceInfoIds.size() > 0) PieceListTransformer::GetInstance().UpdateListView();

	// delete the image
	ImagePieceEditor::GetInstance().SetImageInfo(NULL);
	ImagePieceDocument::GetInstance().RemoveImage(pImageInfo->GetId());
	ImageListTransformer::GetInstance().UpdateListView();
	ImageListTransformer::GetInstance().UpdateProperty(NULL);
}

void ImagePackerFrame::DoDeleteBitmapStyle(const BitmapStyle* pBitmapStyle)
{
	if (!pBitmapStyle) return;
	wxString wxMsg = wxString::Format(_("Do you want to delete the bitmap style: %s"), pBitmapStyle->GetId());
	wxMessageDialog msgPrompt(this, wxMsg, _("Delete Bitmap Style"), wxYES_NO|wxCENTER);
	if (msgPrompt.ShowModal() == wxID_YES)
	{
		// delete bitmap style
		BitmapStyleEditor::GetInstance().SetBitmapStyle(NULL);
		BitmapStyleDocument::GetInstance().RemoveBitmapStyle(pBitmapStyle->GetId());
		BitmapStyleTransformer::GetInstance().UpdateListView();
		BitmapStyleTransformer::GetInstance().UpdateProperty(NULL);
	}
}

void ImagePackerFrame::DoDeleteNineGridStyle(const NineGridStyle* pNineGridStyle)
{
	if (!pNineGridStyle) return;
	wxString wxMsg = wxString::Format(_("Do you want to delete the 9-grid style: %s"), pNineGridStyle->GetId());
	wxMessageDialog msgPrompt(this, wxMsg, _("Delete 9-Grid Style"), wxYES_NO|wxCENTER);
	if (msgPrompt.ShowModal() == wxID_YES)
	{
		// delete nine grid style
		NineGridStyleEditor::GetInstance().SetNineGridStyle(NULL);
		NineGridStyleDocument::GetInstance().RemoveNineGridStyle(pNineGridStyle->GetId());
		NineGridStyleTransformer::GetInstance().UpdateListView();
		NineGridStyleTransformer::GetInstance().UpdateProperty(NULL);
	}
}

void ImagePackerFrame::DoDeleteColorStyle(const ColorStyle* pColorStyle)
{
	if (!pColorStyle) return;
	wxString wxMsg = wxString::Format(_("Do you want to delete the color style: %s"), pColorStyle->GetId());
	wxMessageDialog msgPrompt(this, wxMsg, _("Delete Color Style"), wxYES_NO|wxCENTER);
	if (msgPrompt.ShowModal() == wxID_YES)
	{
		// delete color style
		ColorStyleEditor::GetInstance().SetColorStyle(NULL);
		ColorStyleDocument::GetInstance().RemoveColorStyle(pColorStyle->GetId());
		ColorStyleTransformer::GetInstance().UpdateListView();
		ColorStyleTransformer::GetInstance().UpdateProperty(NULL);
	}
}

void ImagePackerFrame::DoDeleteClipBitmapStyle(const ClipBitmapStyle* pClipBitmapStyle)
{
	if (!pClipBitmapStyle) return;
	wxString wxMsg = wxString::Format(_("Do you want to delete the clip bitmap style: %s"), pClipBitmapStyle->GetId());
	wxMessageDialog msgPrompt(this, wxMsg, _("Delete Clip Bitmap Style"), wxYES_NO|wxCENTER);
	if (msgPrompt.ShowModal() == wxID_YES)
	{
		// delete clip bitmap style
		ClipBitmapStyleEditor::GetInstance().SetClipBitmapStyle(NULL);
		ClipBitmapStyleDocument::GetInstance().RemoveClipBitmapStyle(pClipBitmapStyle->GetId());
		ClipBitmapStyleTransformer::GetInstance().UpdateListView();
		ClipBitmapStyleTransformer::GetInstance().UpdateProperty(NULL);
	}
}

void ImagePackerFrame::OnAddPiece(wxCommandEvent& event)
{
	if (ProjectDocument::GetInstance().GetFilePath().empty())
	{
		wxMessageDialog msg(this, _("Please open a project first"));
		msg.ShowModal();
		return;
	}

	DialogAddPiece dialog(this);
	dialog.SetDefaultImageInfo(ImagePieceEditor::GetInstance().GetImageInfo());
	dialog.ShowModal();

	const ImageInfo* pImageInfo = dialog.GetCurrImageInfo();
	if (pImageInfo)
	{
		m_pListNotebook->SetSelection(LIST_VIEW_IMAGE);
		SwitchEditor(EDITOR_IMAGE_PIECE);
		ImagePieceEditor::GetInstance().SetImageInfo(pImageInfo);
	}
}

void ImagePackerFrame::OnAddBitmapStyle(wxCommandEvent& event)
{
	if (ProjectDocument::GetInstance().GetFilePath().empty())
	{
		wxMessageDialog msg(this, _("Please open a project first"));
		msg.ShowModal();
		return;
	}

	const BitmapStyle* pNewBitmapStyle = BitmapStyleDocument::GetInstance().AddBitmapStyle(wxT("bs_bitmap_style"));
	BitmapStyleTransformer::GetInstance().UpdateListView();
	BitmapStyleTransformer::GetInstance().SetSelectedBitmapStyle(pNewBitmapStyle);
	m_pListNotebook->SetSelection(LIST_VIEW_BITMAP_STYLE);
}

void ImagePackerFrame::OnAddNineGridStyle(wxCommandEvent& event)
{
	if (ProjectDocument::GetInstance().GetFilePath().empty())
	{
		wxMessageDialog msg(this, _("Please open a project first"));
		msg.ShowModal();
		return;
	}

	const NineGridStyle* pNewNineGridStyle = NineGridStyleDocument::GetInstance().AddNineGridStyle(wxT("ngs_nine_grid_style"));
	NineGridStyleTransformer::GetInstance().UpdateListView();
	NineGridStyleTransformer::GetInstance().SetSelectedNineGridStyle(pNewNineGridStyle);
	m_pListNotebook->SetSelection(LIST_VIEW_NINE_GRID_STYLE);
}

void ImagePackerFrame::OnAddColorStyle(wxCommandEvent& event)
{
	if (ProjectDocument::GetInstance().GetFilePath().empty())
	{
		wxMessageDialog msg(this, _("Please open a project first"));
		msg.ShowModal();
		return;
	}

	const ColorStyle* pNewColorStyle = ColorStyleDocument::GetInstance().AddColorStyle(wxT("cs_color_style"));
	ColorStyleTransformer::GetInstance().UpdateListView();
	ColorStyleTransformer::GetInstance().SetSelectedColorStyle(pNewColorStyle);
	m_pListNotebook->SetSelection(LIST_VIEW_COLOR_STYLE);
}

void ImagePackerFrame::OnAddClipBitmapStyle(wxCommandEvent& event)
{
	if (ProjectDocument::GetInstance().GetFilePath().empty())
	{
		wxMessageDialog msg(this, _("Please open a project first"));
		msg.ShowModal();
		return;
	}

	const ClipBitmapStyle* pNewClipBitmapStyle = ClipBitmapStyleDocument::GetInstance().AddClipBitmapStyle(wxT("cbs_clip_bitmap_style"));
	ClipBitmapStyleTransformer::GetInstance().UpdateListView();
	ClipBitmapStyleTransformer::GetInstance().SetSelectedClipBitmapStyle(pNewClipBitmapStyle);
	m_pListNotebook->SetSelection(LIST_VIEW_CLIP_BITMAP_STYLE);
}

void ImagePackerFrame::OnReplacePiece(wxCommandEvent& event)
{
	const ImageInfo* pImageInfo = ImagePieceEditor::GetInstance().GetImageInfo();
	if (!pImageInfo)
	{
		wxMessageDialog msg(this, _("Please select a image first"));
		msg.ShowModal();
		return;
	}

	DialogReplacePiece dialog(this);
	dialog.SetDefaultImageInfo(pImageInfo);
	dialog.ShowModal();

	const ImageInfo* pCurrImageInfo = dialog.GetCurrImageInfo();
	if (pCurrImageInfo)
	{
		m_pListNotebook->SetSelection(LIST_VIEW_IMAGE);
		SwitchEditor(EDITOR_IMAGE_PIECE);
		ImagePieceEditor::GetInstance().SetImageInfo(pCurrImageInfo);
	}
}

void ImagePackerFrame::OnMovePiece(wxCommandEvent& event)
{
	const ImageInfo* pImageInfo = ImagePieceEditor::GetInstance().GetImageInfo();
	if (!pImageInfo)
	{
		wxMessageDialog msg(this, _("Please select a image first"));
		msg.ShowModal();
		return;
	}

	const ImagePieceEditor::TV_PIECE_INFO& vPieces = ImagePieceEditor::GetInstance().GetSelections();
	if (vPieces.size() <= 0)
	{
		wxMessageDialog msg(this, _("Please select pieces to move to"));
		msg.ShowModal();
		return;
	}

	DialogMovePiece dialog(this);
	if (!dialog.InitializeData(pImageInfo, vPieces)) return;
	dialog.ShowModal();

	const ImageInfo* pToImageInfo = dialog.GetToImageInfo();
	if (pToImageInfo)
	{
		DialogAddPiece dialogRepack(this);
		dialogRepack.RepackImagePiece(dialog.GetFromImageInfo());

		m_pListNotebook->SetSelection(LIST_VIEW_IMAGE);
		SwitchEditor(EDITOR_IMAGE_PIECE);
		ImagePieceEditor::GetInstance().SetImageInfo(pToImageInfo);
	}
}

void ImagePackerFrame::OnViewZoom100(wxCommandEvent& event)
{
	m_pEditors[m_pEditorNotebook->GetSelection()]->Zoom(BaseEditor::ZOOM_MIN);
}

void ImagePackerFrame::OnViewZoomIn(wxCommandEvent& event)
{
	m_pEditors[m_pEditorNotebook->GetSelection()]->ZoomIn();
}

void ImagePackerFrame::OnViewZoomOut(wxCommandEvent& event)
{
	m_pEditors[m_pEditorNotebook->GetSelection()]->ZoomOut();
}

void ImagePackerFrame::OnExtractImage(wxCommandEvent& event)
{
	// extrace the selection pieces
	const ImagePieceEditor::TV_PIECE_INFO& vSelPieceInfo = ImagePieceEditor::GetInstance().GetSelections();
	if (vSelPieceInfo.size() > 0)
	{
		ExtractPieces(vSelPieceInfo);
		return;
	}

	// extrace the pieces in the image
	ImagePieceDocument::TV_PIECE_INFO vPiecesInTheImage;
	const ImageInfo* pImageInfo = ImagePieceEditor::GetInstance().GetImageInfo();
	if (ImagePieceDocument::GetInstance().EnumImagePieces(vPiecesInTheImage, pImageInfo) > 0)
	{
		ExtractPieces(vPiecesInTheImage);
		return;
	}

	// show error message
	wxMessageDialog msg(this, _("Please selected a image or pieces"));
	msg.ShowModal();
}

void ImagePackerFrame::OnHelpAbout(wxCommandEvent& event)
{
	wxMessageDialog about(this, Config::GetInstance().GetAbout(), _("About"), wxOK|wxCENTRE|wxICON_INFORMATION);
	about.ShowModal();
}

void ImagePackerFrame::OnImagePieceListSelected(wxTreeEvent& event)
{
	SwitchEditor(EDITOR_IMAGE_PIECE);
	const PieceInfo* pPieceInfo = PieceListTransformer::GetInstance().GetSelectedPieceInfo();
	if (pPieceInfo)
	{
		const ImageInfo* pImageInfo = pPieceInfo->GetImageInfo();
		ImagePieceEditor::GetInstance().SetImageInfo(pImageInfo);
		ImageListTransformer::GetInstance().SetSelectedImageInfo(pImageInfo);
	}
	ImagePieceEditor::GetInstance().SetSelPieceInfo(pPieceInfo);
	PieceListTransformer::GetInstance().UpdateProperty(pPieceInfo);
}

void ImagePackerFrame::OnImageListSelected(wxTreeEvent& event)
{
	SwitchEditor(EDITOR_IMAGE_PIECE);
	const ImageInfo* pImageInfo = ImageListTransformer::GetInstance().GetSelectedImageInfo();
	ImagePieceEditor::GetInstance().SetImageInfo(pImageInfo);
	ImagePieceEditor::GetInstance().SetSelPieceInfo(NULL);
	ImageListTransformer::GetInstance().UpdateProperty(pImageInfo);
}

void ImagePackerFrame::OnBitmapStyleListSelected(wxTreeEvent& event)
{
	SwitchEditor(EDITOR_BITMAP_STYLE);
	const BitmapStyle* pBitmapStyle = BitmapStyleTransformer::GetInstance().GetSelectedBitmapStyle();
	BitmapStyleEditor::GetInstance().SetBitmapStyle(pBitmapStyle);
	BitmapStyleTransformer::GetInstance().UpdateProperty(pBitmapStyle);
}

void ImagePackerFrame::OnNineGridStyleListSelected(wxTreeEvent& event)
{
	SwitchEditor(EDITOR_NINE_GRID_STYLE);
	const NineGridStyle* pNineGridStyle = NineGridStyleTransformer::GetInstance().GetSelectedNineGridStyle();
	NineGridStyleEditor::GetInstance().SetNineGridStyle(pNineGridStyle);
	NineGridStyleTransformer::GetInstance().UpdateProperty(pNineGridStyle);
}

void ImagePackerFrame::OnColorStyleListSelected(wxTreeEvent& event)
{
	SwitchEditor(EDITOR_COLOR_STYLE);
	const ColorStyle* pColorStyle = ColorStyleTransformer::GetInstance().GetSelectedColorStyle();
	ColorStyleEditor::GetInstance().SetColorStyle(pColorStyle);
	ColorStyleTransformer::GetInstance().UpdateProperty(pColorStyle);
}

void ImagePackerFrame::OnClipBitmapStyleListSelected(wxTreeEvent& event)
{
	SwitchEditor(EDITOR_CLIP_BITMAP_STYLE);
	const ClipBitmapStyle* pClipBitmapStyle = ClipBitmapStyleTransformer::GetInstance().GetSelectedClipBitmapStyle();
	ClipBitmapStyleEditor::GetInstance().SetClipBitmapStyle(pClipBitmapStyle);
	ClipBitmapStyleTransformer::GetInstance().UpdateProperty(pClipBitmapStyle);
}

void ImagePackerFrame::OnPropertyGridChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* pProperty = event.GetProperty();

	switch (m_eCurrPropertyType)
	{
	case PT_IMAGE:
		ImageListTransformer::GetInstance().PropertyChanged(pProperty);
		break;
	case PT_PIECE:
		PieceListTransformer::GetInstance().PropertyChanged(pProperty);
		break;
	case PT_BITMAP_STYLE:
		BitmapStyleTransformer::GetInstance().PropertyChanged(pProperty);
		break;
	case PT_NINE_GRID_STYLE:
		NineGridStyleTransformer::GetInstance().PropertyChanged(pProperty);
		break;
	case PT_COLOR_STYLE:
		ColorStyleTransformer::GetInstance().PropertyChanged(pProperty);
		break;
	case PT_CLIP_BITMAP_STYLE:
		ClipBitmapStyleTransformer::GetInstance().PropertyChanged(pProperty);
		break;
	}
}

void ImagePackerFrame::OnListKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_DELETE)
	{
		DoDelete();
	}

	event.Skip();
}

void ImagePackerFrame::OnEditorKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_DELETE)
	{
		DoDelete();
	}

	event.Skip();
}

void ImagePackerFrame::SwitchEditor(EDITOR eEditor)
{
	for (int i = 0; i < NUM_EDITOR; ++i)
	{
		if (i == eEditor)
		{
			m_pEditorNotebook->SetSelection(eEditor);
			m_eCurrEditor = eEditor;
		}
		else
		{
			m_pEditors[i]->Reset();
		}
	}
}

bool ImagePackerFrame::ExtractPieces(const ImagePieceEditor::TV_PIECE_INFO& vPieceInfo)
{
	wxDirDialog dialog(this, _("Choose a directory to extract"), wxEmptyString, wxDD_NEW_DIR_BUTTON);
	if (dialog.ShowModal() != wxID_OK) return false;

	for (ImagePieceDocument::TV_PIECE_INFO::const_iterator it = vPieceInfo.begin(); it != vPieceInfo.end(); ++it)
	{
		const PieceInfo* pPieceInfo = (*it);

		const ImageInfo* pImageInfo = pPieceInfo->GetImageInfo();
		if (!pImageInfo) continue;

		const wxBitmap* pImageBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
		if (!pImageBitmap) continue;

		wxBitmap bitmap = pImageBitmap->GetSubBitmap(pPieceInfo->GetRect());
		bitmap.SaveFile(dialog.GetPath() + wxT("/") + pPieceInfo->GetId() + wxT(".png"), wxBITMAP_TYPE_PNG);
	}

	return true;
}
