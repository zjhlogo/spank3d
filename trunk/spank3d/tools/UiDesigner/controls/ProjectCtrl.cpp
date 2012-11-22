/*!
 * \file ProjectCtrl.cpp
 * \date 11-22-2012 10:09:36
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ProjectCtrl.h"
#include "../document/DocProject.h"

ProjectCtrl::ProjectCtrl(wxWindow *parent, wxWindowID id /*= wxID_ANY*/, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/, long style /*= wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT*/, const wxValidator& validator /*= wxDefaultValidator*/, const wxString& name /*= wxTreeCtrlNameStr*/)
:wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
	DocProject::GetInstance().Connect(wxEVT_DOCUMENT_RESET, wxDocEventHandler(ProjectCtrl::OnDocumentReset), NULL, this);

	wxTreeItemId rootItem = AddRoot(_T("root"));
	m_resRoot = AppendItem(rootItem, _T("resources"));

	m_styleRoot = AppendItem(rootItem, _T("styles"));
	m_styleBitmap = AppendItem(m_styleRoot, _T("bitmap"));
	m_styleNinePatch = AppendItem(m_styleRoot, _T("nine patch"));
	m_styleVerticalPatch = AppendItem(m_styleRoot, _T("vertical patch"));
	m_styleHorizontalPatch = AppendItem(m_styleRoot, _T("horizontal patch"));
	m_styleBitmapFont = AppendItem(m_styleRoot, _T("bitmap font"));

	m_dialogRoot = AppendItem(rootItem, _T("dialogs"));
}

void ProjectCtrl::OnDocumentReset(wxDocEvent& event)
{
	DeleteChildren(m_resRoot);
	DeleteChildren(m_styleBitmap);
	DeleteChildren(m_styleNinePatch);
	DeleteChildren(m_styleVerticalPatch);
	DeleteChildren(m_styleHorizontalPatch);
	DeleteChildren(m_styleBitmapFont);

	const DocProject::TM_PIECE_INFO& pieceInfoMap = DocProject::GetInstance().GetPieceInfoMap();
	for (DocProject::TM_PIECE_INFO::const_iterator it = pieceInfoMap.begin(); it != pieceInfoMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	const DocProject::TM_BITMAP_FONT& bitmapFontMap = DocProject::GetInstance().GetBitmapFontMap();
	for (DocProject::TM_BITMAP_FONT::const_iterator it = bitmapFontMap.begin(); it != bitmapFontMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	const DocProject::TM_BITMAP_STYLE& bitmapStyleMap = DocProject::GetInstance().GetBitmapStyleMap();
	for (DocProject::TM_BITMAP_STYLE::const_iterator it = bitmapStyleMap.begin(); it != bitmapStyleMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	const DocProject::TM_NINE_PATCH_STYLE& ninePatchStyleMap = DocProject::GetInstance().GetNinePatchStyleMap();
	for (DocProject::TM_NINE_PATCH_STYLE::const_iterator it = ninePatchStyleMap.begin(); it != ninePatchStyleMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	const DocProject::TM_VERTICAL_PATCH_STYLE& verticalPatchStyleMap = DocProject::GetInstance().GetVerticalPatchStyleMap();
	for (DocProject::TM_VERTICAL_PATCH_STYLE::const_iterator it = verticalPatchStyleMap.begin(); it != verticalPatchStyleMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	const DocProject::TM_HORIZONTAL_PATCH_STYLE& horizontalPatchStyleMap = DocProject::GetInstance().GetHorizontalPatchStyleMap();
	for (DocProject::TM_HORIZONTAL_PATCH_STYLE::const_iterator it = horizontalPatchStyleMap.begin(); it != horizontalPatchStyleMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	const DocProject::TM_BITMAP_FONT_STYLE& bitmapFontStyleMap = DocProject::GetInstance().GetBitmapFontStyleMap();
	for (DocProject::TM_BITMAP_FONT_STYLE::const_iterator it = bitmapFontStyleMap.begin(); it != bitmapFontStyleMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	ExpandAll();
}
