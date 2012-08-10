/*!
 * \file ColorStyleTransformer.cpp
 * \date 2-14-2012 17:01:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ColorStyleTransformer.h"
#include "../ImagePackerFrame.h"
#include "../document/ColorStyleDocument.h"
#include "../editor/ColorStyleEditor.h"

ColorStyleTransformer::ColorStyleTransformer()
{
	m_pListView = NULL;
	m_pPropertyGrid = NULL;
	m_bSkipUpdateProperty = false;
}

ColorStyleTransformer::~ColorStyleTransformer()
{
	// TODO: 
}

ColorStyleTransformer& ColorStyleTransformer::GetInstance()
{
	static ColorStyleTransformer s_ColorStyleTransformer;
	return s_ColorStyleTransformer;
}

bool ColorStyleTransformer::Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid)
{
	m_pListView = pTreeCtrl;
	m_pPropertyGrid = pPropertyGrid;
	return true;
}

void ColorStyleTransformer::UpdateListView()
{
	m_pListView->DeleteAllItems();
	wxTreeItemId rootItem = m_pListView->AddRoot(wxT("ColorStyleList"));

	const ColorStyleDocument::TM_COLOR_STYLE& ColorStyleMap = ColorStyleDocument::GetInstance().GetColorStyleMap();
	for (ColorStyleDocument::TM_COLOR_STYLE::const_iterator it = ColorStyleMap.begin(); it != ColorStyleMap.end(); ++it)
	{
		ColorStyle* pColorStyle = it->second;
		wxTreeItemId itemId = m_pListView->AppendItem(rootItem, pColorStyle->GetId());
		((ColorStyle*)pColorStyle)->SetTreeItemId(itemId);
	}
	m_pListView->ExpandAll();
}

void ColorStyleTransformer::UpdateProperty(const ColorStyle* pColorStyle)
{
	if (m_bSkipUpdateProperty) return;

	m_pPropertyGrid->Clear();
	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_UNKNOWN);
	if (!pColorStyle) return;

	m_pPropertyGrid->Append(new wxStringProperty(_("id"), wxT("id"), pColorStyle->GetId()));

	wxColour color;
	color.SetRGB(pColorStyle->GetStateColor(IStyle::SS_NORMAL));
	m_pPropertyGrid->Append(new wxColourProperty(_("normal"), wxT("normal"), color));
	color.SetRGB(pColorStyle->GetStateColor(IStyle::SS_DOWN));
	m_pPropertyGrid->Append(new wxColourProperty(_("down"), wxT("down"), color));
	color.SetRGB(pColorStyle->GetStateColor(IStyle::SS_HOVER));
	m_pPropertyGrid->Append(new wxColourProperty(_("hover"), wxT("hover"), color));
	color.SetRGB(pColorStyle->GetStateColor(IStyle::SS_DISABLED));
	m_pPropertyGrid->Append(new wxColourProperty(_("disabled"), wxT("disabled"), color));

	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_COLOR_STYLE);
}

void ColorStyleTransformer::PropertyChanged(const wxPGProperty* pProperty)
{
	const ColorStyle* pColorStyle = GetSelectedColorStyle();
	if (!pColorStyle) return;

	bool bRedraw = false;
	if (pProperty->GetName() == wxT("id"))
	{
		wxString strNewId = pProperty->GetValueAsString();
		if (ColorStyleDocument::GetInstance().RenameColorStyleId(pColorStyle, strNewId))
		{
			UpdateListView();
			m_bSkipUpdateProperty = true;
			SetSelectedColorStyle(pColorStyle);
			m_bSkipUpdateProperty = false;
		}
	}
	else if (pProperty->GetName() == wxT("normal"))
	{
		wxColour color;
		color << pProperty->GetValue();
		bRedraw = ColorStyleDocument::GetInstance().SetStateColor(pColorStyle, color.GetRGB(), IStyle::SS_NORMAL);
	}
	else if (pProperty->GetName() == wxT("down"))
	{
		wxColour color;
		color << pProperty->GetValue();
 		bRedraw = ColorStyleDocument::GetInstance().SetStateColor(pColorStyle, color.GetRGB(), IStyle::SS_DOWN);
	}
	else if (pProperty->GetName() == wxT("hover"))
	{
		wxColour color;
		color << pProperty->GetValue();
		bRedraw = ColorStyleDocument::GetInstance().SetStateColor(pColorStyle, color.GetRGB(), IStyle::SS_HOVER);
	}
	else if (pProperty->GetName() == wxT("disabled"))
	{
		wxColour color;
		color << pProperty->GetValue();
		bRedraw = ColorStyleDocument::GetInstance().SetStateColor(pColorStyle, color.GetRGB(), IStyle::SS_DISABLED);
	}

	if (bRedraw)
	{
		ColorStyleEditor::GetInstance().SetColorStyle(NULL);
		ColorStyleEditor::GetInstance().SetColorStyle(pColorStyle);
	}
}

void ColorStyleTransformer::SetSelectedColorStyle(const ColorStyle* pColorStyle)
{
	if (!pColorStyle) return;

	m_pListView->SelectItem(pColorStyle->GetTreeItemId(), true);
}

const ColorStyle* ColorStyleTransformer::GetSelectedColorStyle()
{
	wxString strColorStyleId = m_pListView->GetItemText(m_pListView->GetSelection());
	const ColorStyle* pColorStyle = ColorStyleDocument::GetInstance().FindColorStyle(strColorStyleId);
	return pColorStyle;
}
