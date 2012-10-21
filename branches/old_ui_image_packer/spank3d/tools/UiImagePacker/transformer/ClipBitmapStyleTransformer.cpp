/*!
 * \file ClipClipBitmapStyleTransformer.cpp
 * \date 2-14-2012 17:05:44
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ClipBitmapStyleTransformer.h"
#include "../ImagePackerFrame.h"
#include "../document/ClipBitmapStyleDocument.h"
#include "../document/ImagePieceDocument.h"
#include "../editor/ClipBitmapStyleEditor.h"

ClipBitmapStyleTransformer::ClipBitmapStyleTransformer()
{
	m_pListView = NULL;
	m_pPropertyGrid = NULL;
	m_bSkipUpdateProperty = false;
}

ClipBitmapStyleTransformer::~ClipBitmapStyleTransformer()
{
	// TODO: 
}

ClipBitmapStyleTransformer& ClipBitmapStyleTransformer::GetInstance()
{
	static ClipBitmapStyleTransformer s_ClipBitmapStyleTransformer;
	return s_ClipBitmapStyleTransformer;
}

bool ClipBitmapStyleTransformer::Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid)
{
	m_pListView = pTreeCtrl;
	m_pPropertyGrid = pPropertyGrid;
	return true;
}

void ClipBitmapStyleTransformer::UpdateListView()
{
	m_pListView->DeleteAllItems();
	wxTreeItemId rootItem = m_pListView->AddRoot(wxT("ClipBitmapStyleList"));

	const ClipBitmapStyleDocument::TM_CLIP_BITMAP_STYLE& ClipBitmapStyleMap = ClipBitmapStyleDocument::GetInstance().GetClipBitmapStyleMap();
	for (ClipBitmapStyleDocument::TM_CLIP_BITMAP_STYLE::const_iterator it = ClipBitmapStyleMap.begin(); it != ClipBitmapStyleMap.end(); ++it)
	{
		ClipBitmapStyle* pClipBitmapStyle = it->second;
		wxTreeItemId itemId = m_pListView->AppendItem(rootItem, pClipBitmapStyle->GetId());
		((ClipBitmapStyle*)pClipBitmapStyle)->SetTreeItemId(itemId);
	}
	m_pListView->ExpandAll();
}

void ClipBitmapStyleTransformer::UpdateProperty(const ClipBitmapStyle* pClipBitmapStyle)
{
	if (m_bSkipUpdateProperty) return;

	m_pPropertyGrid->Clear();
	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_UNKNOWN);
	if (!pClipBitmapStyle) return;

	m_pPropertyGrid->Append(new wxStringProperty(_("id"), wxT("id"), pClipBitmapStyle->GetId()));
	m_pPropertyGrid->Append(new wxBoolProperty(_("auto gen bitmap"), wxT("auto_bitmap"), pClipBitmapStyle->isAutoGenBitmap()))->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);

	const wxArrayString& pieceIds = ImagePieceDocument::GetInstance().GetPieceIds();
	const wxArrayInt& pieceIdsIndex = ImagePieceDocument::GetInstance().GetPieceIdsIndex();
	int value = -1;

	value = ImagePieceDocument::GetInstance().FindPieceIndex(pClipBitmapStyle->GetStatePiece(IStyle::SS_NORMAL));
	m_pPropertyGrid->Append(new wxEnumProperty(_("normal"), wxT("normal"), pieceIds, pieceIdsIndex, value));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pClipBitmapStyle->GetStatePiece(IStyle::SS_DOWN));
	m_pPropertyGrid->Append(new wxEnumProperty(_("down"), wxT("down"), pieceIds, pieceIdsIndex, value));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pClipBitmapStyle->GetStatePiece(IStyle::SS_HOVER));
	m_pPropertyGrid->Append(new wxEnumProperty(_("hover"), wxT("hover"), pieceIds, pieceIdsIndex, value));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pClipBitmapStyle->GetStatePiece(IStyle::SS_DISABLED));
	m_pPropertyGrid->Append(new wxEnumProperty(_("disabled"), wxT("disabled"), pieceIds, pieceIdsIndex, value));

	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_CLIP_BITMAP_STYLE);
}

void ClipBitmapStyleTransformer::PropertyChanged(const wxPGProperty* pProperty)
{
	const ClipBitmapStyle* pClipBitmapStyle = GetSelectedClipBitmapStyle();
	if (!pClipBitmapStyle) return;

	bool bRedraw = false;
	if (pProperty->GetName() == wxT("id"))
	{
		wxString strNewId = pProperty->GetValueAsString();
		if (ClipBitmapStyleDocument::GetInstance().RenameClipBitmapStyleId(pClipBitmapStyle, strNewId))
		{
			UpdateListView();
			m_bSkipUpdateProperty = true;
			SetSelectedClipBitmapStyle(pClipBitmapStyle);
			m_bSkipUpdateProperty = false;
		}
	}
	else if (pProperty->GetName() == wxT("auto_bitmap"))
	{
		bool bAutoGenBitmap = pProperty->GetValue().GetBool();
		bRedraw = ClipBitmapStyleDocument::GetInstance().SetAutoGenBitmap(pClipBitmapStyle, bAutoGenBitmap);
	}
	else if (pProperty->GetName() == wxT("normal"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = ClipBitmapStyleDocument::GetInstance().SetStatePiece(pClipBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_NORMAL);
	}
	else if (pProperty->GetName() == wxT("down"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = ClipBitmapStyleDocument::GetInstance().SetStatePiece(pClipBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_DOWN);
	}
	else if (pProperty->GetName() == wxT("hover"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = ClipBitmapStyleDocument::GetInstance().SetStatePiece(pClipBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_HOVER);
	}
	else if (pProperty->GetName() == wxT("disabled"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = ClipBitmapStyleDocument::GetInstance().SetStatePiece(pClipBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_DISABLED);
	}

	if (bRedraw)
	{
		ClipBitmapStyleEditor::GetInstance().SetClipBitmapStyle(NULL);
		ClipBitmapStyleEditor::GetInstance().SetClipBitmapStyle(pClipBitmapStyle);
	}
}

void ClipBitmapStyleTransformer::SetSelectedClipBitmapStyle(const ClipBitmapStyle* pClipBitmapStyle)
{
	if (!pClipBitmapStyle) return;

	m_pListView->SelectItem(pClipBitmapStyle->GetTreeItemId(), true);
}

const ClipBitmapStyle* ClipBitmapStyleTransformer::GetSelectedClipBitmapStyle()
{
	wxString strClipBitmapStyleId = m_pListView->GetItemText(m_pListView->GetSelection());
	const ClipBitmapStyle* pClipBitmapStyle = ClipBitmapStyleDocument::GetInstance().FindClipBitmapStyle(strClipBitmapStyleId);
	return pClipBitmapStyle;
}
