/*!
 * \file ImagePackerFrame.h
 * \date 2-18-2012 9:43:42
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMAGEPACKERFRAME_H__
#define __IMAGEPACKERFRAME_H__

#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <set>

#include "document/PieceInfo.h"
#include "document/ImageInfo.h"
#include "document/BitmapStyle.h"
#include "document/NineGridStyle.h"
#include "document/ColorStyle.h"
#include "document/ClipBitmapStyle.h"

#include "editor/BaseEditor.h"
#include "editor/ImagePieceEditor.h"

class ImagePackerFrame : public wxFrame
{
public:
	enum CONTROL_ID
	{
		IDM_BASE = 10000,
		IDM_EDIT_UNDO,
		IDM_EDIT_REDO,
		IDM_EDIT_COPY,
		IDM_EDIT_CUT,
		IDM_EDIT_PAST,
		IDM_EDIT_DELETE,

		IDM_ELEMENT_ADD_PIECE,
		IDM_ELEMENT_ADD_BITMAP_STYLE,
		IDM_ELEMENT_ADD_NINE_GRID_STYLE,
		IDM_ELEMENT_ADD_COLOR_STYLE,
		IDM_ELEMENT_ADD_CLIP_BITMAP_STYLE,
		IDM_ELEMENT_REPLACE_PIECE,
		IDM_ELEMENT_MOVE_PIECE,

		IDM_VIEW_GRID,
		IDM_TOOL_EXTRACT_IMAGE,
		IDM_TOOL_PREFERENCES,
		IDM_HELP_SUPPORT,
		IDM_HELP_ABOUT,

 		IDC_TOOLBAR,
		IDC_NOTEBOOK_LIST,
		IDC_NOTEBOOK_EDITOR,
		IDC_IMAGE_LIST,
		IDC_PIECE_LIST,
		IDC_BITMAP_STYLE_LIST,
		IDC_NINE_GRID_STYLE_LIST,
		IDC_COLOR_STYLE_LIST,
		IDC_CLIP_BITMAP_STYLE_LIST,

		IDC_PROPERTY,
		IDC_IMAGE_PIECE_EDITOR_VIEW,
		IDC_BITMAP_STYLE_EDITOR_VIEW,
		IDC_NINE_GRID_STYLE_EDITOR_VIEW,
		IDC_COLOR_STYLE_EDITOR_VIEW,
		IDC_CLIP_BITMAP_STYLE_EDITOR_VIEW,
		IDC_OUTPUT_VIEW,
	};

	enum EDITOR
	{
		EDITOR_IMAGE_PIECE = 0,
		EDITOR_BITMAP_STYLE,
		EDITOR_NINE_GRID_STYLE,
		EDITOR_COLOR_STYLE,
		EDITOR_CLIP_BITMAP_STYLE,
		NUM_EDITOR,
	};

	enum LIST_VIEW
	{
		LIST_VIEW_IMAGE = 0,
		LIST_VIEW_PIECE,
		LIST_VIEW_BITMAP_STYLE,
		LIST_VIEW_NINE_GRID_STYLE,
		LIST_VIEW_COLOR_STYLE,
		LIST_VIEW_CLIP_BITMAP_STYLE,
		NUM_LIST_VIEW,
	};

	enum PROPERTY_TYPE
	{
		PT_UNKNOWN = 0,
		PT_IMAGE,
		PT_PIECE,
		PT_BITMAP_STYLE,
		PT_NINE_GRID_STYLE,
		PT_COLOR_STYLE,
		PT_CLIP_BITMAP_STYLE,
	};

	typedef std::set<wxString> TS_WX_STRING;

public:
	DECLARE_DYNAMIC_CLASS(ImagePackerFrame)
	DECLARE_EVENT_TABLE()

public:
	ImagePackerFrame();
	virtual ~ImagePackerFrame();

	static ImagePackerFrame& GetInstance();

	void SetCurrPropertyType(PROPERTY_TYPE eType);
	PROPERTY_TYPE GetCurrPropertyType() const;

	void UpdateUiModified(bool bModified, bool bForce = false);

private:
	void Init();
	void Release();

	void CreateControls();
	void CreateMenu();
	void CreateToolbar();
	void CreatePropertyView();
	void CreateListView();
	void CreateEditorView();
	void CreateOutputView();

	void OnFileOpenProject(wxCommandEvent& event);
	void DoOpenProject();
	void OnFileSaveProject(wxCommandEvent& event);
	void DoSaveProject();
	void OnFileExit(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	bool IsModified() const;

	void OnEditDelete(wxCommandEvent& event);
	void DoDelete();
	bool FindStyleIdsUsingThePiece(TS_WX_STRING& vBitmapStyleIds, TS_WX_STRING& vNineGridStyleIds, TS_WX_STRING& vClipBitmapStyleIds, const PieceInfo* pPieceInfo);
	bool MakeIdsString(wxString& strOut, const TS_WX_STRING& vIds, const wxString& strHeader);
	void DoDeletePiece(const PieceInfo* pPieceInfo);
	void DoDeleteImage(const ImageInfo* pImageInfo);
	void DoDeleteBitmapStyle(const BitmapStyle* pBitmapStyle);
	void DoDeleteNineGridStyle(const NineGridStyle* pNineGridStyle);
	void DoDeleteColorStyle(const ColorStyle* pColorStyle);
	void DoDeleteClipBitmapStyle(const ClipBitmapStyle* pClipBitmapStyle);

	void OnAddPiece(wxCommandEvent& event);
	void OnAddBitmapStyle(wxCommandEvent& event);
	void OnAddNineGridStyle(wxCommandEvent& event);
	void OnAddColorStyle(wxCommandEvent& event);
	void OnAddClipBitmapStyle(wxCommandEvent& event);
	void OnReplacePiece(wxCommandEvent& event);
	void OnMovePiece(wxCommandEvent& event);

	void OnViewZoom100(wxCommandEvent& event);
	void OnViewZoomIn(wxCommandEvent& event);
	void OnViewZoomOut(wxCommandEvent& event);

	void OnExtractImage(wxCommandEvent& event);
	void OnHelpAbout(wxCommandEvent& event);

	void OnImagePieceListSelected(wxTreeEvent& event);
	void OnImageListSelected(wxTreeEvent& event);
	void OnBitmapStyleListSelected(wxTreeEvent& event);
	void OnNineGridStyleListSelected(wxTreeEvent& event);
	void OnColorStyleListSelected(wxTreeEvent& event);
	void OnClipBitmapStyleListSelected(wxTreeEvent& event);

	void OnPropertyGridChanged(wxPropertyGridEvent& event);

	void OnListKeyDown(wxKeyEvent& event);
	void OnEditorKeyDown(wxKeyEvent& event);

	void SwitchEditor(EDITOR eEditor);
	bool ExtractPieces(const ImagePieceEditor::TV_PIECE_INFO& vPieceInfo);

private:
	static ImagePackerFrame* m_pImagePackerFrame;

	wxAuiManager m_auiManager;

	wxNotebook* m_pListNotebook;
	wxTextCtrl* m_pOutputView;

	PROPERTY_TYPE m_eCurrPropertyType;
	wxPropertyGrid* m_pPropertyGrid;

	wxNotebook* m_pEditorNotebook;
	EDITOR m_eCurrEditor;
	BaseEditor* m_pEditors[NUM_EDITOR];

	bool m_bTitleModifiedFlag;
};
#endif // __IMAGEPACKERFRAME_H__
