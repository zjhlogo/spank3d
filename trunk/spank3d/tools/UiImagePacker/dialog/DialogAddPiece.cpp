/*!
 * \file DialogAddPiece.cpp
 * \date 2-16-2012 9:14:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DialogAddPiece.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dcgraph.h>
#include <algorithm>

#include "../Config.h"
#include "../document/ImagePieceDocument.h"
#include "../document/ProjectDocument.h"
#include "../transformer/ImageListTransformer.h"
#include "../transformer/PieceListTransformer.h"
#include "../utils/FileUtil.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

IMPLEMENT_DYNAMIC_CLASS(DialogAddPiece, wxDialog)

BEGIN_EVENT_TABLE(DialogAddPiece, wxDialog)
	EVT_BUTTON(ID_BUTTON, DialogAddPiece::OnBtnAddPieceClicked)
	EVT_BUTTON(ID_BUTTON1, DialogAddPiece::OnBtnRemovePieceClicked)
	EVT_LISTBOX(ID_LISTBOX, DialogAddPiece::OnLstPiecesSelected)
	EVT_RADIOBUTTON(ID_RADIOBUTTON, DialogAddPiece::OnRadAddImageSelected)
	EVT_CHOICE(ID_CHOICE, DialogAddPiece::OnChoImageSelected)
	EVT_RADIOBUTTON(ID_RADIOBUTTON1, DialogAddPiece::OnRadAddImageSelected)
	EVT_BUTTON(wxID_OK, DialogAddPiece::OnOkClicked)
	EVT_BUTTON(wxID_CANCEL, DialogAddPiece::OnCancelClicked)
END_EVENT_TABLE()

static bool ComparePackingInfo(DialogAddPiece::PACKING_PIECE_INFO* p1, DialogAddPiece::PACKING_PIECE_INFO* p2)
{
	wxSize size1 = p1->subBitmap.GetSize();
	wxSize size2 = p2->subBitmap.GetSize();

// 	return (size1.x*size1.y) > (size2.x*size2.y);
	return std::max(size1.x, size1.y) > std::max(size2.x, size2.y);
}

DialogAddPiece::DialogAddPiece()
{
	Init();
}

DialogAddPiece::DialogAddPiece(wxWindow* parent, wxWindowID id /* = wxID_ANY */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxDEFAULT_DIALOG_STYLE */)
{
	Init();
	Create(parent, id, _("Add Piece"), pos, size, style);
}

DialogAddPiece::~DialogAddPiece()
{
	// TODO: 
}

bool DialogAddPiece::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxDEFAULT_DIALOG_STYLE */)
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create(parent, id, caption, pos, size, style);

	CreateControls();
	InitializeData();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();

	return true;
}

void DialogAddPiece::Init()
{
	m_BtnAddPiece = NULL;
	m_BtnRemovePiece = NULL;
	m_LstPieces = NULL;
	m_RadAddExisting = NULL;
	m_ChoImage = NULL;
	m_RadCreateNew = NULL;
	m_EdtImageName = NULL;
	m_EdtImageWidth = NULL;
	m_EdtImageHeight = NULL;
	m_pCurrImageInfo = NULL;
}

void DialogAddPiece::CreateControls()
{    
	DialogAddPiece* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Add Piece"));
	wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
	itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_RIGHT, 5);

	m_BtnAddPiece = new wxButton(itemStaticBoxSizer3->GetStaticBox(), ID_BUTTON, _("+"), wxDefaultPosition, wxSize(24, 24), 0);
	itemBoxSizer4->Add(m_BtnAddPiece, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_BtnRemovePiece = new wxButton(itemStaticBoxSizer3->GetStaticBox(), ID_BUTTON1, _("-"), wxDefaultPosition, wxSize(24, 24), 0);
	m_BtnRemovePiece->Enable(false);
	itemBoxSizer4->Add(m_BtnRemovePiece, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxArrayString m_LstPiecesStrings;
	m_LstPieces = new wxListBox(itemStaticBoxSizer3->GetStaticBox(), ID_LISTBOX, wxDefaultPosition, wxSize(-1, 200), m_LstPiecesStrings, wxLB_EXTENDED);
	itemStaticBoxSizer3->Add(m_LstPieces, 0, wxGROW, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL, 5);

	wxStaticBox* itemStaticBoxSizer9Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Add Image"));
	wxStaticBoxSizer* itemStaticBoxSizer9 = new wxStaticBoxSizer(itemStaticBoxSizer9Static, wxVERTICAL);
	itemBoxSizer8->Add(itemStaticBoxSizer9, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxFlexGridSizer* itemFlexGridSizer10 = new wxFlexGridSizer(0, 2, 2, 0);
	itemStaticBoxSizer9->Add(itemFlexGridSizer10, 0, wxALIGN_CENTER_HORIZONTAL, 5);

	m_RadAddExisting = new wxRadioButton(itemStaticBoxSizer9->GetStaticBox(), ID_RADIOBUTTON, _("Add Existing"), wxDefaultPosition, wxDefaultSize, 0);
	m_RadAddExisting->SetValue(true);
	itemFlexGridSizer10->Add(m_RadAddExisting, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	wxArrayString m_ChoImageStrings;
	m_ChoImage = new wxChoice(itemStaticBoxSizer9->GetStaticBox(), ID_CHOICE, wxDefaultPosition, wxDefaultSize, m_ChoImageStrings, 0);
	itemFlexGridSizer10->Add(m_ChoImage, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	m_RadCreateNew = new wxRadioButton(itemStaticBoxSizer9->GetStaticBox(), ID_RADIOBUTTON1, _("Create New"), wxDefaultPosition, wxDefaultSize, 0);
	m_RadCreateNew->SetValue(false);
	itemFlexGridSizer10->Add(m_RadCreateNew, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageName = new wxTextCtrl(itemStaticBoxSizer9->GetStaticBox(), ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_EdtImageName->Enable(false);
	itemFlexGridSizer10->Add(m_EdtImageName, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticBox* itemStaticBoxSizer15Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Image Info"));
	wxStaticBoxSizer* itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxVERTICAL);
	itemBoxSizer8->Add(itemStaticBoxSizer15, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

	wxFlexGridSizer* itemFlexGridSizer16 = new wxFlexGridSizer(0, 2, 2, 2);
	itemStaticBoxSizer15->Add(itemFlexGridSizer16, 0, wxALIGN_CENTER_HORIZONTAL, 5);

	wxStaticText* itemStaticText17 = new wxStaticText(itemStaticBoxSizer15->GetStaticBox(), wxID_STATIC, _("Width"), wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageWidth = new wxTextCtrl(itemStaticBoxSizer15->GetStaticBox(), ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer16->Add(m_EdtImageWidth, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* itemStaticText19 = new wxStaticText(itemStaticBoxSizer15->GetStaticBox(), wxID_STATIC, _("Height"), wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer16->Add(itemStaticText19, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageHeight = new wxTextCtrl(itemStaticBoxSizer15->GetStaticBox(), ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer16->Add(m_EdtImageHeight, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer21 = new wxStdDialogButtonSizer;

	itemBoxSizer2->Add(itemStdDialogButtonSizer21, 0, wxALIGN_RIGHT|wxALL, 5);
	wxButton* itemButton22 = new wxButton(itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0);
	itemStdDialogButtonSizer21->AddButton(itemButton22);

	wxButton* itemButton23 = new wxButton(itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	itemStdDialogButtonSizer21->AddButton(itemButton23);

	itemStdDialogButtonSizer21->Realize();
}

void DialogAddPiece::InitializeData()
{
	m_ChoImage->Clear();
	const wxArrayString& arrayImageIds = ImagePieceDocument::GetInstance().GetImageIds();
	for (wxArrayString::const_iterator it = arrayImageIds.begin(); it != arrayImageIds.end(); ++it)
	{
		m_ChoImage->Append((*it));
	}
}

void DialogAddPiece::OnBtnAddPieceClicked(wxCommandEvent& event)
{
	wxFileDialog dialog(this,
		_("Choose a file"),
		ProjectDocument::GetInstance().GetRootPath(),
		wxEmptyString,
		_("Image files (*.png;*.jpg)|*.png;*.jpg"),
		wxFD_OPEN|wxFD_MULTIPLE);

	if (dialog .ShowModal() == wxID_OK)
	{
		wxArrayString arrayString;
		dialog.GetPaths(arrayString);
		AddListItems(arrayString);
	}

	event.Skip();
}

void DialogAddPiece::OnBtnRemovePieceClicked(wxCommandEvent& event)
{
	RemoveListSelectedItems();
	m_BtnRemovePiece->Enable(false);
	event.Skip();
}

void DialogAddPiece::OnLstPiecesSelected(wxCommandEvent& event)
{
	wxArrayInt arrayInt;
	if (m_LstPieces->GetSelections(arrayInt) <= 0)
	{
		m_BtnRemovePiece->Enable(false);
	}
	else
	{
		m_BtnRemovePiece->Enable(true);
	}
	event.Skip();
}

void DialogAddPiece::OnRadAddImageSelected(wxCommandEvent& event)
{
	m_ChoImage->Enable(m_RadAddExisting->GetValue());

	if (m_RadAddExisting->GetValue())
	{
		wxString strImageId = m_ChoImage->GetStringSelection();
		const ImageInfo* pImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
		InitImageInfo(pImageInfo);
	}
	else
	{
		InitImageInfo(NULL);
	}

	event.Skip();
}

void DialogAddPiece::OnChoImageSelected(wxCommandEvent& event)
{
	wxString strImageId = m_ChoImage->GetStringSelection();
	const ImageInfo* pImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	InitImageInfo(pImageInfo);
	event.Skip();
}

bool DialogAddPiece::RepackImagePiece(const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return false;

	const wxBitmap* pBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
	wxSize newSize(pBitmap->GetWidth(), pBitmap->GetHeight());

	TV_PACKING_PIECE_INFO vPackingInfo;

	// get packing info from images
	if (!GetPieceFromImage(vPackingInfo, pImageInfo))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// sort the pieces
	std::sort(vPackingInfo.begin(), vPackingInfo.end(), ComparePackingInfo);

	// generate packing info, try packing
	if (!GeneratePackingInfo(vPackingInfo, newSize))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// packing
	wxBitmap* pNewBitmap = PackImage(newSize, vPackingInfo);
	if (!pNewBitmap)
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// apply new image and piece info to documents
	ImagePieceDocument::GetInstance().SetImageBitmap(pImageInfo, pNewBitmap);
	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);
		wxRect rect(pPackingInfo->pNode->x, pPackingInfo->pNode->y, pPackingInfo->pNode->width, pPackingInfo->pNode->height);
		ImagePieceDocument::GetInstance().SetPieceRect(pPackingInfo->pPieceInfo, rect);
	}

	FreePackingPiecesInfo(vPackingInfo);

	return true;
}

void DialogAddPiece::OnOkClicked(wxCommandEvent& event)
{
	bool bOk = false;

	long width = 0;
	long height = 0;
	if (!m_EdtImageWidth->GetValue().ToLong(&width)) return;
	if (!m_EdtImageHeight->GetValue().ToLong(&height)) return;
	if (width <= 0 || height <= 0)
	{
		wxMessageDialog msg(this, _("width and height must greater than 0"), _("Error"));
		msg.ShowModal();
		return;
	}

	wxSize newSize(width, height);

	if (m_RadAddExisting->GetValue())
	{
		bOk = AddPieceToExistingImage(newSize);
	}
	else
	{
		wxFileDialog dialog(this,
			_("Save to file"),
			ProjectDocument::GetInstance().GetRootPath(),
			_("Untitled.png"),
			_("Image files (*.png)|*.png"),
			wxFD_SAVE);

		if (dialog .ShowModal() == wxID_OK)
		{
			m_EdtImageName->SetValue(dialog.GetPath());
			bOk = AddPieceIntoNewImage(newSize);
		}
		else
		{
			return;
		}
	}

	if (!bOk)
	{
		wxMessageDialog msg(this, m_strError);
		msg.ShowModal();
		return;
	}
	else
	{
		event.Skip();
	}
}

void DialogAddPiece::OnCancelClicked(wxCommandEvent& event)
{
	event.Skip();
}

void DialogAddPiece::InitImageInfo(const ImageInfo* pImageInfo)
{
	if (!pImageInfo)
	{
		m_EdtImageName->SetValue(wxEmptyString);
		m_EdtImageWidth->SetValue(wxString::Format(wxT("%d"), Config::DEFAULT_IMAGE_SIZE));
		m_EdtImageHeight->SetValue(wxString::Format(wxT("%d"), Config::DEFAULT_IMAGE_SIZE));
	}
	else
	{
		m_EdtImageName->SetValue(pImageInfo->GetId());
		const wxBitmap* pBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
		m_EdtImageWidth->SetValue(wxString::Format(wxT("%d"), pBitmap->GetWidth()));
		m_EdtImageHeight->SetValue(wxString::Format(wxT("%d"), pBitmap->GetHeight()));
	}
}

void DialogAddPiece::AddListItems(const wxArrayString& arrayString)
{
	if (arrayString.size() <= 0) return;

	for (int i = 0; i < (int)arrayString.size(); ++i)
	{
		wxString strPath = arrayString[i];
		m_LstPieces->Append(strPath);
	}
}

void DialogAddPiece::RemoveListSelectedItems()
{
	wxArrayInt arrayInt;
	m_LstPieces->GetSelections(arrayInt);

	wxArrayString arrayString;
	for (int i = 0; i < (int)arrayInt.size(); ++i)
	{
		arrayString.Add(m_LstPieces->GetString(arrayInt[i]));
	}

	for (int i = 0; i < (int)arrayString.size(); ++i)
	{
		for (int j = 0; j < (int)m_LstPieces->GetCount(); ++j)
		{
			if (m_LstPieces->GetString(j) == arrayString[i])
			{
				// remove item
				m_LstPieces->Delete(j);
				break;
			}
		}
	}
}

bool DialogAddPiece::AddPieceToExistingImage(const wxSize& newSize)
{
	wxString strImageId = m_ChoImage->GetStringSelection();
	const ImageInfo* pImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	if (!pImageInfo) return false;

	TV_PACKING_PIECE_INFO vPackingInfo;

	// get packing info from list
	if (!GetPieceFromList(vPackingInfo))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// get packing info from images
	if (!GetPieceFromImage(vPackingInfo, pImageInfo))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// sort the pieces
	std::sort(vPackingInfo.begin(), vPackingInfo.end(), ComparePackingInfo);

	// generate packing info, try packing
	if (!GeneratePackingInfo(vPackingInfo, newSize))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// packing
	wxBitmap* pNewBitmap = PackImage(newSize, vPackingInfo);
	if (!pNewBitmap)
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// apply new image and piece info to documents
	ImagePieceDocument::GetInstance().SetImageBitmap(pImageInfo, pNewBitmap);
	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);
		wxRect rect(pPackingInfo->pNode->x, pPackingInfo->pNode->y, pPackingInfo->pNode->width, pPackingInfo->pNode->height);
		if (pPackingInfo->pPieceInfo)
		{
			ImagePieceDocument::GetInstance().SetPieceRect(pPackingInfo->pPieceInfo, rect);
		}
		else
		{
			wxString strId = FileUtil::GetFileName(pPackingInfo->strId);
			FileUtil::FormatId(strId);
			ImagePieceDocument::GetInstance().AddPiece(strId, rect, pImageInfo);
		}
	}

	PieceListTransformer::GetInstance().UpdateListView();
	FreePackingPiecesInfo(vPackingInfo);

	m_pCurrImageInfo = pImageInfo;
	return true;
}

bool DialogAddPiece::AddPieceIntoNewImage(const wxSize& newSize)
{
	wxString strPath = m_EdtImageName->GetValue();
	if (strPath.empty()) return false;

	TV_PACKING_PIECE_INFO vPackingInfo;

	// get packing info from list
	if (!GetPieceFromList(vPackingInfo))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// sort the pieces
	std::sort(vPackingInfo.begin(), vPackingInfo.end(), ComparePackingInfo);

	// generate packing info, try packing
	if (!GeneratePackingInfo(vPackingInfo, newSize))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// packing
	wxBitmap* pNewBitmap = PackImage(newSize, vPackingInfo);
	if (!pNewBitmap)
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// apply new image and piece info to documents
	wxString strImageId = FileUtil::GetFileName(strPath);
	FileUtil::FormatId(strImageId);
	wxString strRelativePath = FileUtil::RemoveRootDir(strPath, ProjectDocument::GetInstance().GetRootPath() + wxT("/"));
	const ImageInfo* pImageInfo = ImagePieceDocument::GetInstance().AddImage(strImageId, strRelativePath, pNewBitmap);

	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);
		wxRect rect(pPackingInfo->pNode->x, pPackingInfo->pNode->y, pPackingInfo->pNode->width, pPackingInfo->pNode->height);
		wxString strId = FileUtil::GetFileName(pPackingInfo->strId);
		FileUtil::FormatId(strId);
		ImagePieceDocument::GetInstance().AddPiece(strId, rect, pImageInfo);
	}

	ImageListTransformer::GetInstance().UpdateListView();
	PieceListTransformer::GetInstance().UpdateListView();
	FreePackingPiecesInfo(vPackingInfo);
	m_pCurrImageInfo = pImageInfo;
	return true;
}

void DialogAddPiece::FreePackingPiecesInfo(TV_PACKING_PIECE_INFO& vPackingInfo)
{
	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);
		SAFE_DELETE(pPackingInfo);
	}

	vPackingInfo.clear();
}

wxBitmap* DialogAddPiece::PackImage(const wxSize& bmpSize, const TV_PACKING_PIECE_INFO& vPackingInfo)
{
	wxBitmap* pNewBitmap = new wxBitmap(bmpSize);
	pNewBitmap->UseAlpha();

	wxMemoryDC memDC(*pNewBitmap);
	wxGCDC dc(memDC);

	dc.SetBackground(*wxTRANSPARENT_BRUSH);
	dc.Clear();

	for (TV_PACKING_PIECE_INFO::const_iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		const PACKING_PIECE_INFO* pPackingInfo = (*it);
		wxPoint destPos(pPackingInfo->pNode->x, pPackingInfo->pNode->y);

		wxBitmap& subBitmap = (wxBitmap)pPackingInfo->subBitmap;
		wxMemoryDC memSubDC(subBitmap);
		dc.Blit(destPos, pPackingInfo->subBitmap.GetSize(), &memSubDC, wxPoint(0, 0));
	}

	return pNewBitmap;
}

bool DialogAddPiece::GeneratePackingInfo(TV_PACKING_PIECE_INFO& vPackingInfo, const wxSize& bmpSize)
{
	m_Packer.Init(bmpSize.x, bmpSize.y);

	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);

		// Pack the next rectangle in the input list.
		pPackingInfo->pNode = m_Packer.Insert(pPackingInfo->bmpSize.x, pPackingInfo->bmpSize.y);
		if (!pPackingInfo->pNode)
		{
			m_strError = wxString::Format(_("Not enough space to pack sub bitmaps %s"), pPackingInfo->strId);
			return false;
		}
	}

	return true;
}

bool DialogAddPiece::GetPieceFromList(TV_PACKING_PIECE_INFO& vPackingInfo)
{
	wxArrayString arrayString = m_LstPieces->GetStrings();
	if (arrayString.size() <= 0) return true;

	for (wxArrayString::iterator it = arrayString.begin(); it != arrayString.end(); ++it)
	{
		const wxString& strPath = (*it);

		PACKING_PIECE_INFO* pPackingInfo = new PACKING_PIECE_INFO();
		pPackingInfo->pNode = NULL;

		if (!pPackingInfo->subBitmap.LoadFile(strPath, wxBITMAP_TYPE_ANY))
		{
			SAFE_DELETE(pPackingInfo);
			m_strError = wxString::Format(_("Load Bimap Failed: %s"), strPath);
			return false;
		}
		pPackingInfo->bmpSize = pPackingInfo->subBitmap.GetSize();
		pPackingInfo->strId = strPath;
		pPackingInfo->pPieceInfo = NULL;

		vPackingInfo.push_back(pPackingInfo);
	}

	return true;
}

bool DialogAddPiece::GetPieceFromImage(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo)
{
	ImagePieceDocument::TV_PIECE_INFO PiecesInImage;
	int numPieces = ImagePieceDocument::GetInstance().EnumImagePieces(PiecesInImage, pImageInfo);
	for (ImagePieceDocument::TV_PIECE_INFO::const_iterator it = PiecesInImage.begin(); it != PiecesInImage.end(); ++it)
	{
		const PieceInfo* pPieceInfo = (*it);
		const wxRect& pieceRect = pPieceInfo->GetRect();

		PACKING_PIECE_INFO* pPackingInfo = new PACKING_PIECE_INFO();
		pPackingInfo->pNode = NULL;
		pPackingInfo->subBitmap = ((ImageInfo*)pImageInfo)->GetBitmap()->GetSubBitmap(pieceRect);
		pPackingInfo->bmpSize = pPackingInfo->subBitmap.GetSize();
		pPackingInfo->strId = pPieceInfo->GetId();
		pPackingInfo->pPieceInfo = pPieceInfo;

		vPackingInfo.push_back(pPackingInfo);
	}

	return true;
}

void DialogAddPiece::SetDefaultImageInfo(const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return;

	m_ChoImage->SetStringSelection(pImageInfo->GetId());
	InitImageInfo(pImageInfo);
}

const ImageInfo* DialogAddPiece::GetCurrImageInfo()
{
	return m_pCurrImageInfo;
}