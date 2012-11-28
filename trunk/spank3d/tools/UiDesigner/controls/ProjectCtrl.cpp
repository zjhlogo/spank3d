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
}

void ProjectCtrl::OnDocumentReset(wxDocEvent& event)
{
	ResetAllItems();

	// update piece info view
	DocProject::TM_DOC_PIECE_INFO& docPieceInfoMap = DocProject::GetInstance().GetPieceInfoMap();
	for (DocProject::TM_DOC_PIECE_INFO::iterator it = docPieceInfoMap.begin(); it != docPieceInfoMap.end(); ++it)
	{
		DocPieceInfo* pDocPieceInfo = it->second;
		wxTreeItemId docPieceInfoItemId = AppendItem(m_resRoot, pDocPieceInfo->GetFileName());

		DocPieceInfo::TM_PIECE_INFO& pieceInfoMap = pDocPieceInfo->GetPieceInfoMap();
		for (DocPieceInfo::TM_PIECE_INFO::iterator itPieceInfo = pieceInfoMap.begin(); itPieceInfo != pieceInfoMap.end(); ++itPieceInfo)
		{
			AppendItem(docPieceInfoItemId, itPieceInfo->first);
		}
	}

	const DocProject::TM_DOC_BITMAP_FONT& bitmapFontMap = DocProject::GetInstance().GetBitmapFontMap();
	for (DocProject::TM_DOC_BITMAP_FONT::const_iterator it = bitmapFontMap.begin(); it != bitmapFontMap.end(); ++it)
	{
		AppendItem(m_resRoot, it->first);
	}

	// update bitmap style view
	DocProject::TM_DOC_BITMAP_STYLE& bitmapStyleMap = DocProject::GetInstance().GetBitmapStyleMap();
	for (DocProject::TM_DOC_BITMAP_STYLE::iterator it = bitmapStyleMap.begin(); it != bitmapStyleMap.end(); ++it)
	{
		DocBitmapStyle* pDocBitmapStyle = it->second;
		wxTreeItemId docBitmapStyleItemId = AppendItem(m_resRoot, pDocBitmapStyle->GetFileName());

		DocBitmapStyle::TM_BITMAP_STYLE& bitmapStyleMap = pDocBitmapStyle->GetBitmapStyleMap();
		for (DocBitmapStyle::TM_BITMAP_STYLE::iterator itBitmapStyle = bitmapStyleMap.begin(); itBitmapStyle != bitmapStyleMap.end(); ++itBitmapStyle)
		{
			AppendItem(docBitmapStyleItemId, itBitmapStyle->first);
			AppendItem(m_styleBitmap, itBitmapStyle->first);
		}
	}

	// update nine patch style view
	DocProject::TM_DOC_NINE_PATCH_STYLE& ninePatchStyleMap = DocProject::GetInstance().GetNinePatchStyleMap();
	for (DocProject::TM_DOC_NINE_PATCH_STYLE::iterator it = ninePatchStyleMap.begin(); it != ninePatchStyleMap.end(); ++it)
	{
		DocNinePatchStyle* pDocNinePatchStyle = it->second;
		wxTreeItemId docNinePatchStyleItemId = AppendItem(m_resRoot, pDocNinePatchStyle->GetFileName());

		DocNinePatchStyle::TM_NINE_PATCH_STYLE& ninePatchStyleMap = pDocNinePatchStyle->GetNinePatchStyleMap();
		for (DocNinePatchStyle::TM_NINE_PATCH_STYLE::iterator itNinePatchStyle = ninePatchStyleMap.begin(); itNinePatchStyle != ninePatchStyleMap.end(); ++itNinePatchStyle)
		{
			AppendItem(docNinePatchStyleItemId, itNinePatchStyle->first);
			AppendItem(m_styleNinePatch, itNinePatchStyle->first);
		}
	}

	// update horizontal patch style view
	DocProject::TM_DOC_HORIZONTAL_PATCH_STYLE& horizontalPatchStyleMap = DocProject::GetInstance().GetHorizontalPatchStyleMap();
	for (DocProject::TM_DOC_HORIZONTAL_PATCH_STYLE::iterator it = horizontalPatchStyleMap.begin(); it != horizontalPatchStyleMap.end(); ++it)
	{
		DocHorizontalPatchStyle* pDocHorizontalPatchStyle = it->second;
		wxTreeItemId docHorizontalPatchStyleItemId = AppendItem(m_resRoot, pDocHorizontalPatchStyle->GetFileName());

		DocHorizontalPatchStyle::TM_HORIZONTAL_PATCH_STYLE& horizontalPatchStyleMap = pDocHorizontalPatchStyle->GetHorizontalPatchStyleMap();
		for (DocHorizontalPatchStyle::TM_HORIZONTAL_PATCH_STYLE::iterator itHorizontalPatchStyle = horizontalPatchStyleMap.begin(); itHorizontalPatchStyle != horizontalPatchStyleMap.end(); ++itHorizontalPatchStyle)
		{
			AppendItem(docHorizontalPatchStyleItemId, itHorizontalPatchStyle->first);
			AppendItem(m_styleHorizontalPatch, itHorizontalPatchStyle->first);
		}
	}

	// update vertical patch style view
	DocProject::TM_DOC_VERTICAL_PATCH_STYLE& verticalPatchStyleMap = DocProject::GetInstance().GetVerticalPatchStyleMap();
	for (DocProject::TM_DOC_VERTICAL_PATCH_STYLE::iterator it = verticalPatchStyleMap.begin(); it != verticalPatchStyleMap.end(); ++it)
	{
		DocVerticalPatchStyle* pDocVerticalPatchStyle = it->second;
		wxTreeItemId docVerticalPatchStyleItemId = AppendItem(m_resRoot, pDocVerticalPatchStyle->GetFileName());

		DocVerticalPatchStyle::TM_VERTICAL_PATCH_STYLE& verticalPatchStyleMap = pDocVerticalPatchStyle->GetVerticalPatchStyleMap();
		for (DocVerticalPatchStyle::TM_VERTICAL_PATCH_STYLE::iterator itVerticalPatchStyle = verticalPatchStyleMap.begin(); itVerticalPatchStyle != verticalPatchStyleMap.end(); ++itVerticalPatchStyle)
		{
			AppendItem(docVerticalPatchStyleItemId, itVerticalPatchStyle->first);
			AppendItem(m_styleVerticalPatch, itVerticalPatchStyle->first);
		}
	}

	// update bitmap font style view
	DocProject::TM_DOC_BITMAP_FONT_STYLE& bitmapFontStyleMap = DocProject::GetInstance().GetBitmapFontStyleMap();
	for (DocProject::TM_DOC_BITMAP_FONT_STYLE::iterator it = bitmapFontStyleMap.begin(); it != bitmapFontStyleMap.end(); ++it)
	{
		DocBitmapFontStyle* pDocBitmapFontStyle = it->second;
		wxTreeItemId docBitmapFontStyleItemId = AppendItem(m_resRoot, pDocBitmapFontStyle->GetFileName());

		DocBitmapFontStyle::TM_BITMAP_FONT_STYLE& bitmapFontStyleMap = pDocBitmapFontStyle->GetBitmapFontStyleMap();
		for (DocBitmapFontStyle::TM_BITMAP_FONT_STYLE::iterator itBitmapFontStyle = bitmapFontStyleMap.begin(); itBitmapFontStyle != bitmapFontStyleMap.end(); ++itBitmapFontStyle)
		{
			AppendItem(docBitmapFontStyleItemId, itBitmapFontStyle->first);
			AppendItem(m_styleBitmapFont, itBitmapFontStyle->first);
		}
	}

	Expand(m_resRoot);
	Expand(m_styleRoot);
	Expand(m_dialogRoot);
}

void ProjectCtrl::ResetAllItems()
{
	DeleteAllItems();

	wxTreeItemId rootItem = AddRoot(_T("root"));
	m_resRoot = AppendItem(rootItem, _T("resources"));

	m_styleRoot = AppendItem(rootItem, _T("styles"));
	m_styleBitmap = AppendItem(m_styleRoot, _T("bitmap"));
	m_styleNinePatch = AppendItem(m_styleRoot, _T("nine patch"));
	m_styleHorizontalPatch = AppendItem(m_styleRoot, _T("horizontal patch"));
	m_styleVerticalPatch = AppendItem(m_styleRoot, _T("vertical patch"));
	m_styleBitmapFont = AppendItem(m_styleRoot, _T("bitmap font"));

	m_dialogRoot = AppendItem(rootItem, _T("dialogs"));
}
