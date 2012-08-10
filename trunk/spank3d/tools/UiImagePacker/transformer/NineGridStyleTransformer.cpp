/*!
 * \file NineGridStyleTransformer.cpp
 * \date 2-14-2012 16:08:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "NineGridStyleTransformer.h"
#include "../ImagePackerFrame.h"
#include "../document/NineGridStyleDocument.h"
#include "../document/ImagePieceDocument.h"
#include "../editor/NineGridStyleEditor.h"

NineGridStyleTransformer::NineGridStyleTransformer()
{
	m_pListView = NULL;
	m_pPropertyGrid = NULL;
	m_bSkipUpdateProperty = false;
}

NineGridStyleTransformer::~NineGridStyleTransformer()
{
	// TODO: 
}

NineGridStyleTransformer& NineGridStyleTransformer::GetInstance()
{
	static NineGridStyleTransformer s_NineGridStyleTransformer;
	return s_NineGridStyleTransformer;
}

bool NineGridStyleTransformer::Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid)
{
	m_pListView = pTreeCtrl;
	m_pPropertyGrid = pPropertyGrid;
	return true;
}

void NineGridStyleTransformer::UpdateListView()
{
	m_pListView->DeleteAllItems();
	wxTreeItemId rootItem = m_pListView->AddRoot(wxT("NineGridStyleList"));

	const NineGridStyleDocument::TM_NINE_GRID_STYLE& NineGridStyleMap = NineGridStyleDocument::GetInstance().GetNineGridStyleMap();
	for (NineGridStyleDocument::TM_NINE_GRID_STYLE::const_iterator it = NineGridStyleMap.begin(); it != NineGridStyleMap.end(); ++it)
	{
		NineGridStyle* pNineGridStyle = it->second;
		wxTreeItemId itemId = m_pListView->AppendItem(rootItem, pNineGridStyle->GetId());
		((NineGridStyle*)pNineGridStyle)->SetTreeItemId(itemId);
	}
	m_pListView->ExpandAll();
}

void NineGridStyleTransformer::UpdateProperty(const NineGridStyle* pNineGrieStyle)
{
	if (m_bSkipUpdateProperty) return;

	m_pPropertyGrid->Clear();
	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_UNKNOWN);
	if (!pNineGrieStyle) return;

	m_pPropertyGrid->Append(new wxStringProperty(_("id"), wxT("id"), pNineGrieStyle->GetId()));
	m_pPropertyGrid->Append(new wxBoolProperty(_("auto gen bitmap"), wxT("auto_bitmap"), pNineGrieStyle->isAutoGenBitmap()))->SetAttribute(wxPG_BOOL_USE_CHECKBOX, true);

	const wxArrayString& pieceIds = ImagePieceDocument::GetInstance().GetPieceIds();
	const wxArrayInt& pieceIdsIndex = ImagePieceDocument::GetInstance().GetPieceIdsIndex();
	int value = -1;

	m_pPropertyGrid->Append(new wxPropertyCategory(_("normal"), wxT("normal")));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pNineGrieStyle->GetStateGridInfo(IStyle::SS_NORMAL)->pPieceInfo);
	m_pPropertyGrid->Append(new wxEnumProperty(_("piece_id"), wxT("normal_piece_id"), pieceIds, pieceIdsIndex, value));
	m_pPropertyGrid->Append(new wxIntProperty(_("min_x"), wxT("normal_min_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_NORMAL)->min_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("min_y"), wxT("normal_min_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_NORMAL)->min_y))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_x"), wxT("normal_max_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_NORMAL)->max_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_y"), wxT("normal_max_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_NORMAL)->max_y))->SetEditor(wxPGEditor_SpinCtrl);

	m_pPropertyGrid->Append(new wxPropertyCategory(_("down"), wxT("down")));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pNineGrieStyle->GetStateGridInfo(IStyle::SS_DOWN)->pPieceInfo);
	m_pPropertyGrid->Append(new wxEnumProperty(_("piece_id"), wxT("down_piece_id"), pieceIds, pieceIdsIndex, value));
	m_pPropertyGrid->Append(new wxIntProperty(_("min_x"), wxT("down_min_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DOWN)->min_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("min_y"), wxT("down_min_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DOWN)->min_y))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_x"), wxT("down_max_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DOWN)->max_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_y"), wxT("down_max_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DOWN)->max_y))->SetEditor(wxPGEditor_SpinCtrl);

	m_pPropertyGrid->Append(new wxPropertyCategory(_("hover"), wxT("hover")));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pNineGrieStyle->GetStateGridInfo(IStyle::SS_HOVER)->pPieceInfo);
	m_pPropertyGrid->Append(new wxEnumProperty(_("piece_id"), wxT("hover_piece_id"), pieceIds, pieceIdsIndex, value));
	m_pPropertyGrid->Append(new wxIntProperty(_("min_x"), wxT("hover_min_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_HOVER)->min_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("min_y"), wxT("hover_min_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_HOVER)->min_y))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_x"), wxT("hover_max_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_HOVER)->max_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_y"), wxT("hover_max_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_HOVER)->max_y))->SetEditor(wxPGEditor_SpinCtrl);

	m_pPropertyGrid->Append(new wxPropertyCategory(_("disabled"), wxT("disabled")));
	value = ImagePieceDocument::GetInstance().FindPieceIndex(pNineGrieStyle->GetStateGridInfo(IStyle::SS_DISABLED)->pPieceInfo);
	m_pPropertyGrid->Append(new wxEnumProperty(_("piece_id"), wxT("disabled_piece_id"), pieceIds, pieceIdsIndex, value));
	m_pPropertyGrid->Append(new wxIntProperty(_("min_x"), wxT("disabled_min_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DISABLED)->min_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("min_y"), wxT("disabled_min_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DISABLED)->min_y))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_x"), wxT("disabled_max_x"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DISABLED)->max_x))->SetEditor(wxPGEditor_SpinCtrl);
	m_pPropertyGrid->Append(new wxIntProperty(_("max_y"), wxT("disabled_max_y"), pNineGrieStyle->GetStateGridInfo(IStyle::SS_DISABLED)->max_y))->SetEditor(wxPGEditor_SpinCtrl);

	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_NINE_GRID_STYLE);
}

void NineGridStyleTransformer::PropertyChanged(wxPGProperty* pProperty)
{
	const NineGridStyle* pNineGridStyle = GetSelectedNineGridStyle();
	if (!pNineGridStyle) return;

	bool bResetStyle = false;
	bool bRefresh = false;

	if (pProperty->GetName() == wxT("id"))
	{
		wxString strNewId = pProperty->GetValueAsString();
		if (NineGridStyleDocument::GetInstance().RenameNineGridStyleId(pNineGridStyle, strNewId))
		{
			UpdateListView();
			m_bSkipUpdateProperty = true;
			SetSelectedNineGridStyle(pNineGridStyle);
			m_bSkipUpdateProperty = false;
		}
	}
	else if (pProperty->GetName() == wxT("auto_bitmap"))
	{
		bool bAutoGenBitmap = pProperty->GetValue().GetBool();
		bRefresh = NineGridStyleDocument::GetInstance().SetAutoGenBitmap(pNineGridStyle, bAutoGenBitmap);
	}
	else if (pProperty->GetName() == wxT("normal_piece_id"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bResetStyle = NineGridStyleDocument::GetInstance().SetStatePiece(pNineGridStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_NORMAL);
	}
	else if (pProperty->GetName() == wxT("normal_min_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinX(pNineGridStyle, value, IStyle::SS_NORMAL);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL)->min_x);
	}
	else if (pProperty->GetName() == wxT("normal_min_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinY(pNineGridStyle, value, IStyle::SS_NORMAL);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL)->min_y);
	}
	else if (pProperty->GetName() == wxT("normal_max_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxX(pNineGridStyle, value, IStyle::SS_NORMAL);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL)->max_x);
	}
	else if (pProperty->GetName() == wxT("normal_max_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxY(pNineGridStyle, value, IStyle::SS_NORMAL);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL)->max_y);
	}
	else if (pProperty->GetName() == wxT("down_piece_id"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bResetStyle = NineGridStyleDocument::GetInstance().SetStatePiece(pNineGridStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_DOWN);
	}
	else if (pProperty->GetName() == wxT("down_min_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinX(pNineGridStyle, value, IStyle::SS_DOWN);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DOWN)->min_x);
	}
	else if (pProperty->GetName() == wxT("down_min_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinY(pNineGridStyle, value, IStyle::SS_DOWN);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DOWN)->min_y);
	}
	else if (pProperty->GetName() == wxT("down_max_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxX(pNineGridStyle, value, IStyle::SS_DOWN);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DOWN)->max_x);
	}
	else if (pProperty->GetName() == wxT("down_max_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxY(pNineGridStyle, value, IStyle::SS_DOWN);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DOWN)->max_y);
	}
	else if (pProperty->GetName() == wxT("hover_piece_id"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bResetStyle = NineGridStyleDocument::GetInstance().SetStatePiece(pNineGridStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_HOVER);
	}
	else if (pProperty->GetName() == wxT("hover_min_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinX(pNineGridStyle, value, IStyle::SS_HOVER);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_HOVER)->min_x);
	}
	else if (pProperty->GetName() == wxT("hover_min_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinY(pNineGridStyle, value, IStyle::SS_HOVER);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_HOVER)->min_y);
	}
	else if (pProperty->GetName() == wxT("hover_max_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxX(pNineGridStyle, value, IStyle::SS_HOVER);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_HOVER)->max_x);
	}
	else if (pProperty->GetName() == wxT("hover_max_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxY(pNineGridStyle, value, IStyle::SS_HOVER);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_HOVER)->max_y);
	}
	else if (pProperty->GetName() == wxT("disabled_piece_id"))
	{
		wxString strNewPieceId = pProperty->GetValueAsString();
		bResetStyle = NineGridStyleDocument::GetInstance().SetStatePiece(pNineGridStyle, ImagePieceDocument::GetInstance().FindPieceInfo(strNewPieceId), IStyle::SS_DISABLED);
	}
	else if (pProperty->GetName() == wxT("disabled_min_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinX(pNineGridStyle, value, IStyle::SS_DISABLED);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DISABLED)->min_x);
	}
	else if (pProperty->GetName() == wxT("disabled_min_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMinY(pNineGridStyle, value, IStyle::SS_DISABLED);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DISABLED)->min_y);
	}
	else if (pProperty->GetName() == wxT("disabled_max_x"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxX(pNineGridStyle, value, IStyle::SS_DISABLED);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DISABLED)->max_x);
	}
	else if (pProperty->GetName() == wxT("disabled_max_y"))
	{
		int value = pProperty->GetValue().GetInteger();
		bRefresh = NineGridStyleDocument::GetInstance().SetStateMaxY(pNineGridStyle, value, IStyle::SS_DISABLED);
		pProperty->SetValueFromInt(pNineGridStyle->GetStateGridInfo(IStyle::SS_DISABLED)->max_y);
	}

	if (bResetStyle)
	{
		NineGridStyleEditor::GetInstance().SetNineGridStyle(NULL);
		NineGridStyleEditor::GetInstance().SetNineGridStyle(pNineGridStyle);
	}

	if (bRefresh)
	{
		NineGridStyleEditor::GetInstance().Refresh(false);
	}
}

void NineGridStyleTransformer::SetSelectedNineGridStyle(const NineGridStyle* pNineGrieStyle)
{
	if (!pNineGrieStyle) return;

	m_pListView->SelectItem(pNineGrieStyle->GetTreeItemId(), true);
}

const NineGridStyle* NineGridStyleTransformer::GetSelectedNineGridStyle()
{
	wxString strNineGridStyleId = m_pListView->GetItemText(m_pListView->GetSelection());
	const NineGridStyle* pNineGridStyle = NineGridStyleDocument::GetInstance().FindNineGridStyle(strNineGridStyleId);
	return pNineGridStyle;
}
