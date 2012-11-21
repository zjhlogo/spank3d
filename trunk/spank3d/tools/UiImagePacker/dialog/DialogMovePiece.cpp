/*!
 * \file DialogMovePiece.cpp
 * \date 3-22-2012 17:26:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include <wx/dcgraph.h>
#include <algorithm>

#include "DialogMovePiece.h"
#include "../Config.h"
#include "../document/ImagePieceDocument.h"
#include "../document/ProjectDocument.h"
#include "../transformer/ImageListTransformer.h"
#include "../transformer/PieceListTransformer.h"
#include "../utils/FileUtil.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

IMPLEMENT_DYNAMIC_CLASS(DialogMovePiece, wxDialog)

BEGIN_EVENT_TABLE(DialogMovePiece, wxDialog)
	EVT_CHOICE(ID_CHOICE1, DialogMovePiece::OnChoImageIdsSelected)
	EVT_BUTTON(wxID_OK, DialogMovePiece::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, DialogMovePiece::OnCancelClick)
END_EVENT_TABLE()

static bool ComparePackingInfo(DialogMovePiece::PACKING_PIECE_INFO* p1, DialogMovePiece::PACKING_PIECE_INFO* p2)
{
	wxSize size1 = p1->subBitmap.GetSize();
	wxSize size2 = p2->subBitmap.GetSize();

// 	return (size1.x*size1.y) > (size2.x*size2.y);
	return std::max(size1.x, size1.y) > std::max(size2.x, size2.y);
}

DialogMovePiece::DialogMovePiece()
{
	Init();
}

DialogMovePiece::DialogMovePiece(wxWindow* parent, wxWindowID id /* = wxID_ANY */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxDEFAULT_DIALOG_STYLE */)
{
	Init();
	Create(parent, id, _("Move Piece"), pos, size, style);
}

DialogMovePiece::~DialogMovePiece()
{
	// TODO: 
}

bool DialogMovePiece::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxDEFAULT_DIALOG_STYLE */)
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create(parent, id, caption, pos, size, style);

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();

	return true;
}

const ImageInfo* DialogMovePiece::GetFromImageInfo() const
{
	return m_pFromImageInfo;
}

const ImageInfo* DialogMovePiece::GetToImageInfo() const
{
	return m_pToImageInfo;
}

void DialogMovePiece::Init()
{
	m_LstPieces = NULL;
	m_EdtFromImage = NULL;
	m_ChoToImage = NULL;
	m_EdtImageWidth = NULL;
	m_EdtImageHeight = NULL;

	m_pFromImageInfo = NULL;
	m_pToImageInfo = NULL;
}

void DialogMovePiece::CreateControls()
{    
	DialogMovePiece* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Pieces to move"));
	wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
	itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxArrayString m_LstPieceStrings;
	m_LstPieces = new wxListBox(itemStaticBoxSizer3->GetStaticBox(), ID_LISTBOX, wxDefaultPosition, wxSize(400, 200), m_LstPieceStrings, wxLB_SINGLE);
	itemStaticBoxSizer3->Add(m_LstPieces, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer5Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Image Info"));
	wxStaticBoxSizer* itemStaticBoxSizer5 = new wxStaticBoxSizer(itemStaticBoxSizer5Static, wxVERTICAL);
	itemBoxSizer2->Add(itemStaticBoxSizer5, 0, wxGROW|wxALL, 5);

	wxFlexGridSizer* itemFlexGridSizer6 = new wxFlexGridSizer(0, 2, 2, 2);
	itemStaticBoxSizer5->Add(itemFlexGridSizer6, 0, wxGROW, 5);

	wxStaticText* itemStaticText7 = new wxStaticText(itemStaticBoxSizer5->GetStaticBox(), wxID_STATIC, _("From Image"), wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtFromImage = new wxTextCtrl(itemStaticBoxSizer5->GetStaticBox(), ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_EdtFromImage->Enable(false);
	itemFlexGridSizer6->Add(m_EdtFromImage, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* itemStaticText9 = new wxStaticText(itemStaticBoxSizer5->GetStaticBox(), wxID_STATIC, _("To Image"), wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer6->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	wxArrayString m_ChoImageIdsStrings;
	m_ChoToImage = new wxChoice(itemStaticBoxSizer5->GetStaticBox(), ID_CHOICE1, wxDefaultPosition, wxDefaultSize, m_ChoImageIdsStrings, 0);
	itemFlexGridSizer6->Add(m_ChoToImage, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* itemStaticText11 = new wxStaticText(itemStaticBoxSizer5->GetStaticBox(), wxID_STATIC, _("Width"), wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer6->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageWidth = new wxTextCtrl(itemStaticBoxSizer5->GetStaticBox(), ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer6->Add(m_EdtImageWidth, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* itemStaticText13 = new wxStaticText(itemStaticBoxSizer5->GetStaticBox(), wxID_STATIC, _("Height"), wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer6->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageHeight = new wxTextCtrl(itemStaticBoxSizer5->GetStaticBox(), ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	itemFlexGridSizer6->Add(m_EdtImageHeight, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	itemFlexGridSizer6->AddGrowableCol(1);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer15 = new wxStdDialogButtonSizer;

	itemBoxSizer2->Add(itemStdDialogButtonSizer15, 0, wxALIGN_RIGHT|wxALL, 5);
	wxButton* itemButton16 = new wxButton(itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0);
	itemStdDialogButtonSizer15->AddButton(itemButton16);

	wxButton* itemButton17 = new wxButton(itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	itemStdDialogButtonSizer15->AddButton(itemButton17);

	itemStdDialogButtonSizer15->Realize();
}

bool DialogMovePiece::InitializeData(const ImageInfo* pImageInfo, const ImagePieceEditor::TV_PIECE_INFO& vPieces)
{
	if (!pImageInfo) return false;
	if (vPieces.size() <= 0) return false;

	// initialize image list
	m_ChoToImage->Clear();
	const wxArrayString& arrayImageIds = ImagePieceDocument::GetInstance().GetImageIds();
	for (wxArrayString::const_iterator it = arrayImageIds.begin(); it != arrayImageIds.end(); ++it)
	{
		if ((*it) != pImageInfo->GetId()) m_ChoToImage->Append((*it));
	}
	m_ChoToImage->SetSelection(0);

	// initialize piece list
	m_LstPieces->Clear();
	for (ImagePieceEditor::TV_PIECE_INFO::const_iterator it = vPieces.begin(); it != vPieces.end(); ++it)
	{
		const PieceInfo* pPieceInfo = (*it);
		m_LstPieces->Append(pPieceInfo->GetId());
	}

	// setup from image info
	m_pFromImageInfo = pImageInfo;
	m_EdtFromImage->SetValue(m_pFromImageInfo->GetId());

	// setup to image info
	m_pToImageInfo = NULL;
	InitToImageInfo(m_pToImageInfo);

	return true;
}

void DialogMovePiece::OnChoImageIdsSelected(wxCommandEvent& event)
{
	wxString strImageId = m_ChoToImage->GetStringSelection();
	const ImageInfo* pToImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	InitToImageInfo(pToImageInfo);
	event.Skip();
}

void DialogMovePiece::OnOkClick(wxCommandEvent& event)
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
	bOk = AddPieceToExistingImage(newSize);

	if (!bOk)
	{
		m_pToImageInfo = NULL;
		wxMessageDialog msg(this, m_strError);
		msg.ShowModal();
		return;
	}
	else
	{
		event.Skip();
	}
}

void DialogMovePiece::OnCancelClick(wxCommandEvent& event)
{
	event.Skip();
}

void DialogMovePiece::InitToImageInfo(const ImageInfo* pImageInfo)
{
	if (!pImageInfo)
	{
		m_EdtImageWidth->SetValue("");
		m_EdtImageHeight->SetValue("");
		m_EdtImageWidth->Enable(false);
		m_EdtImageHeight->Enable(false);
	}
	else
	{
		const wxBitmap* pBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
		m_EdtImageWidth->SetValue(wxString::Format(wxT("%d"), pBitmap->GetWidth()));
		m_EdtImageHeight->SetValue(wxString::Format(wxT("%d"), pBitmap->GetHeight()));
		m_EdtImageWidth->Enable(true);
		m_EdtImageHeight->Enable(true);
	}
}

bool DialogMovePiece::AddPieceToExistingImage(const wxSize& newSize)
{
	wxString strImageId = m_ChoToImage->GetStringSelection();
	m_pToImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	if (!m_pToImageInfo) return false;

	TV_PACKING_PIECE_INFO vPackingInfo;

	// get packing info from list
	if (!GetPieceFromList(vPackingInfo))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// get packing info from images
	if (!GetPieceFromImage(vPackingInfo, m_pToImageInfo))
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
	ImagePieceDocument::GetInstance().SetImageBitmap(m_pToImageInfo, pNewBitmap);
	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);

		// clear old bitmap info
		const PieceInfo* pPieceInfo = pPackingInfo->pPieceInfo;
		if (pPieceInfo->GetImageInfo() == m_pFromImageInfo)
		{
			ImagePieceDocument::GetInstance().ClearImageBitmapArea(m_pFromImageInfo, pPieceInfo->GetRect());
			ImagePieceDocument::GetInstance().SetPieceImageInfo(pPieceInfo, m_pToImageInfo);
		}

		// set new bitmap info
		wxRect rect(pPackingInfo->pNode->x, pPackingInfo->pNode->y, pPackingInfo->pNode->width, pPackingInfo->pNode->height);
		ImagePieceDocument::GetInstance().SetPieceRect(pPieceInfo, rect);
	}

	FreePackingPiecesInfo(vPackingInfo);

	return true;
}

bool DialogMovePiece::GetPieceFromList(TV_PACKING_PIECE_INFO& vPackingInfo)
{
	wxArrayString arrayString = m_LstPieces->GetStrings();
	if (arrayString.size() <= 0) return true;

	for (wxArrayString::iterator it = arrayString.begin(); it != arrayString.end(); ++it)
	{
		const PieceInfo* pPieceInfo = ImagePieceDocument::GetInstance().FindPieceInfo(*it);
		if (!pPieceInfo || pPieceInfo->GetImageInfo() != m_pFromImageInfo)
		{
			m_strError = wxString::Format(_("Can not find piece %s from image %s"), (*it), m_pFromImageInfo->GetId());
			return false;
		}

		const wxRect& pieceRect = pPieceInfo->GetRect();

		PACKING_PIECE_INFO* pPackingInfo = new PACKING_PIECE_INFO();
		pPackingInfo->pNode = NULL;
		pPackingInfo->subBitmap = ((ImageInfo*)m_pFromImageInfo)->GetBitmap()->GetSubBitmap(pieceRect);
		pPackingInfo->bmpSize = pPackingInfo->subBitmap.GetSize();
		pPackingInfo->strId = pPieceInfo->GetId();
		pPackingInfo->pPieceInfo = pPieceInfo;

		vPackingInfo.push_back(pPackingInfo);
	}

	return true;
}

bool DialogMovePiece::GetPieceFromImage(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo)
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

bool DialogMovePiece::GeneratePackingInfo(TV_PACKING_PIECE_INFO& vPackingInfo, const wxSize& bmpSize)
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

void DialogMovePiece::FreePackingPiecesInfo(TV_PACKING_PIECE_INFO& vPackingInfo)
{
	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);
		SAFE_DELETE(pPackingInfo);
	}

	vPackingInfo.clear();
}

wxBitmap* DialogMovePiece::PackImage(const wxSize& bmpSize, const TV_PACKING_PIECE_INFO& vPackingInfo)
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
