/*!
 * \file ImageListTransformer.cpp
 * \date 2-14-2012 1:02:37
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ImageListTransformer.h"
#include "../ImagePackerFrame.h"
#include "../document/ImagePieceDocument.h"

ImageListTransformer::ImageListTransformer()
{
	m_pListView = NULL;
	m_pPropertyGrid = NULL;
	m_bSkipUpdateProperty = false;
}

ImageListTransformer::~ImageListTransformer()
{
	// TODO: 
}

ImageListTransformer& ImageListTransformer::GetInstance()
{
	static ImageListTransformer s_ImageListTransformer;
	return s_ImageListTransformer;
}

bool ImageListTransformer::Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid)
{
	m_pListView = pTreeCtrl;
	m_pPropertyGrid = pPropertyGrid;
	return true;
}

void ImageListTransformer::UpdateListView()
{
	m_pListView->DeleteAllItems();
	wxTreeItemId rootItem = m_pListView->AddRoot(wxT("ImageList"));

	const ImagePieceDocument::TM_IMAGE_INFO& ImageInfoMap = ImagePieceDocument::GetInstance().GetImageInfoMap();
	for (ImagePieceDocument::TM_IMAGE_INFO::const_iterator it = ImageInfoMap.begin(); it != ImageInfoMap.end(); ++it)
	{
		ImageInfo* pImageInfo = it->second;
		wxTreeItemId itemId = m_pListView->AppendItem(rootItem, pImageInfo->GetId());
		((ImageInfo*)pImageInfo)->SetTreeItemId(itemId);
	}
	m_pListView->ExpandAll();
}

void ImageListTransformer::UpdateProperty(const ImageInfo* pImageInfo)
{
	if (m_bSkipUpdateProperty) return;

	m_pPropertyGrid->Clear();
	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_UNKNOWN);
	if (!pImageInfo) return;

	m_pPropertyGrid->Append(new wxStringProperty(_("id"), wxT("id"), pImageInfo->GetId()));
	m_pPropertyGrid->Append(new wxFileProperty(_("path"), wxT("path"), pImageInfo->GetFileName()))->Enable(false);

	wxSize bitmapSize = ((ImageInfo*)pImageInfo)->GetBitmap()->GetSize();
	m_pPropertyGrid->Append(new wxPropertyCategory(_("size"), wxT("size")));
	m_pPropertyGrid->Append(new wxIntProperty(_("width"), wxT("width"), bitmapSize.x))->Enable(false);
	m_pPropertyGrid->Append(new wxIntProperty(_("height"), wxT("height"), bitmapSize.y))->Enable(false);
	ImagePackerFrame::GetInstance().SetCurrPropertyType(ImagePackerFrame::PT_IMAGE);
}

void ImageListTransformer::PropertyChanged(wxPGProperty* pProperty)
{
	const ImageInfo* pImageInfo = GetSelectedImageInfo();
	if (!pImageInfo) return;

	if (pProperty->GetName() == wxT("id"))
	{
		wxString strNewId = pProperty->GetValueAsString();
		if (ImagePieceDocument::GetInstance().RenameImageInfoId(pImageInfo, strNewId))
		{
			UpdateListView();
			m_bSkipUpdateProperty = true;
			SetSelectedImageInfo(pImageInfo);
			m_bSkipUpdateProperty = false;
		}
	}
}

void ImageListTransformer::SetSelectedImageInfo(const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return;

	m_pListView->SelectItem(pImageInfo->GetTreeItemId(), true);
}

const ImageInfo* ImageListTransformer::GetSelectedImageInfo()
{
	wxString strImageId = m_pListView->GetItemText(m_pListView->GetSelection());
	const ImageInfo* pImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	return pImageInfo;
}
