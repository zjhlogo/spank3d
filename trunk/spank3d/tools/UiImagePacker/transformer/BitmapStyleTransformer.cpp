/*!
 * \file BitmapStyleTransformer.cpp
 * \date 2-14-2012 14:46:38
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "BitmapStyleTransformer.h"
#include "../ImagePackerFrame.h"
#include "../document/BitmapStyleDocument.h"
#include "../document/ImagePieceDocument.h"
#include "../editor/BitmapStyleEditor.h"

BitmapStyleTransformer::BitmapStyleTransformer()
{
	m_pListView = NULL;
	m_pPropertyGrid = NULL;
	m_bSkipUpdateProperty = false;
}

BitmapStyleTransformer::~BitmapStyleTransformer()
{
	// TODO: 
}

BitmapStyleTransformer& BitmapStyleTransformer::GetInstance()
{
	static BitmapStyleTransformer s_BitmapStyleTransformer;
	return s_BitmapStyleTransformer;
}

bool BitmapStyleTransformer::Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid)
{
	m_pListView = pTreeCtrl;
	m_pPropertyGrid = pPropertyGrid;
	return true;
}

void BitmapStyleTransformer::UpdateListView()
{
	m_pListView->DeleteAllItems();
	wxTreeItemId rootItem = m_pListView->AddRoot(wxT("BitmapStyleList"));

	const BitmapStyleDocument::TM_BITMAP_STYLE& BitmapStyleMap = BitmapStyleDocument::GetInstance().GetBitmapStyleMap();
	for (BitmapStyleDocument::TM_BITMAP_STYLE::const_iterator it = BitmapStyleMap.begin(); it != BitmapStyleMap.end(); ++it)
	{
		const BitmapStyle* pBitmapStyle = it->second;
		wxTreeItemId itemId = m_pListView->AppendItem(rootItem, pBitmapStyle->GetId());
		((BitmapStyle*)pBitmapStyle)->SetTreeItemId(itemId);
	}
	m_pListView->ExpandAll();
}

void BitmapStyleTransformer::UpdateProperty(const BitmapStyle* pBitmapStyle)
{
	if (m_bSkipUpdateProperty) return;

	m_pPropertyGrid->Clear();
	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_UNKNOWN);
	if (!pBitmapStyle) return;

	m_pPropertyGrid->Append(new wxStringProperty(_("id"), wxT("id"), pBitmapStyle->GetId()));
	m_pPropertyGrid->Append(new wxBoolProperty(_("auto gen bitmap"), wxT("auto_bitmap"), pBitmapStyle->isAutoGenBitmap()))->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);

	const wxArrayString& pieceIds = ImagePieceDocument::GetInstance().GetPieceIds();
	const wxArrayInt& pieceIdsIndex = ImagePieceDocument::GetInstance().GetPieceIdsIndex();
	int value = -1;

	value = ImagePieceDocument::GetInstance().FindPieceIndex(pBitmapStyle->GetStatePiece(IStyle::SS_NORMAL));
	m_pPropertyGrid->Append(new wxEnumProperty(_("normal"), wxT("normal"), pieceIds, pieceIdsIndex, value));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pBitmapStyle->GetStatePiece(IStyle::SS_DOWN));
	m_pPropertyGrid->Append(new wxEnumProperty(_("down"), wxT("down"), pieceIds, pieceIdsIndex, value));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pBitmapStyle->GetStatePiece(IStyle::SS_HOVER));
	m_pPropertyGrid->Append(new wxEnumProperty(_("hover"), wxT("hover"), pieceIds, pieceIdsIndex, value));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pBitmapStyle->GetStatePiece(IStyle::SS_DISABLED));
	m_pPropertyGrid->Append(new wxEnumProperty(_("disabled"), wxT("disabled"), pieceIds, pieceIdsIndex, value));

	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_BITMAP_STYLE);
}

void BitmapStyleTransformer::PropertyChanged(wxPGProperty* pProperty)
{
	const BitmapStyle* pBitmapStyle = GetSelectedBitmapStyle();
	if (!pBitmapStyle) return;

	bool bRedraw = false;
	if (pProperty->GetName() == wxT("id"))
	{
		wxString strNewId = pProperty->GetValueAsString();
		if (BitmapStyleDocument::GetInstance().RenameBitmapStyleId(pBitmapStyle, strNewId))
		{
			UpdateListView();
			m_bSkipUpdateProperty = true;
			SetSelectedBitmapStyle(pBitmapStyle);
			m_bSkipUpdateProperty = false;
		}
	}
	else if (pProperty->GetName() == wxT("auto_bitmap"))
	{
		bool bAutoGenBitmap = pProperty->GetValue().GetBool();
		bRedraw = BitmapStyleDocument::GetInstance().SetAutoGenBitmap(pBitmapStyle, bAutoGenBitmap);
	}
	else if (pProperty->GetName() == wxT("normal"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = BitmapStyleDocument::GetInstance().SetStatePiece(pBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_NORMAL);
	}
	else if (pProperty->GetName() == wxT("down"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = BitmapStyleDocument::GetInstance().SetStatePiece(pBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_DOWN);
	}
	else if (pProperty->GetName() == wxT("hover"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = BitmapStyleDocument::GetInstance().SetStatePiece(pBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_HOVER);
	}
	else if (pProperty->GetName() == wxT("disabled"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bRedraw = BitmapStyleDocument::GetInstance().SetStatePiece(pBitmapStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_DISABLED);
	}

	if (bRedraw)
	{
		BitmapStyleEditor::GetInstance().SetBitmapStyle(NULL);
		BitmapStyleEditor::GetInstance().SetBitmapStyle(pBitmapStyle);
	}
}

void BitmapStyleTransformer::SetSelectedBitmapStyle(const BitmapStyle* pBitmapStyle)
{
	if (!pBitmapStyle) return;

	m_pListView->SelectItem(pBitmapStyle->GetTreeItemId(), true);
}

const BitmapStyle* BitmapStyleTransformer::GetSelectedBitmapStyle()
{
	wxString strBitmapStyleId = m_pListView->GetItemText(m_pListView->GetSelection());
	const BitmapStyle* pBitmapStyle = BitmapStyleDocument::GetInstance().FindBitmapStyle(strBitmapStyleId);
	return pBitmapStyle;
}
