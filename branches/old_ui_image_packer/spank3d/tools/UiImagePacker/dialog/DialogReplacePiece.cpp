/*!
 * \file DialogReplacePiece.cpp
 * \date 2-21-2012 13:55:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DialogReplacePiece.h"
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

IMPLEMENT_DYNAMIC_CLASS(DialogReplacePiece, wxDialog)

BEGIN_EVENT_TABLE(DialogReplacePiece, wxDialog)
	EVT_BUTTON(ID_BUTTON, DialogReplacePiece::OnBtnAddPieceClicked)
	EVT_BUTTON(ID_BUTTON1, DialogReplacePiece::OnBtnRemovePieceClicked)
	EVT_LISTBOX(ID_LISTBOX, DialogReplacePiece::OnLstPiecesSelected)
	EVT_BUTTON(wxID_OK, DialogReplacePiece::OnOkClicked)
	EVT_BUTTON(wxID_CANCEL, DialogReplacePiece::OnCancelClicked)
END_EVENT_TABLE()

static bool ComparePackingInfo(DialogReplacePiece::PACKING_PIECE_INFO* p1, DialogReplacePiece::PACKING_PIECE_INFO* p2)
{
	wxSize size1 = p1->subBitmap.GetSize();
	wxSize size2 = p2->subBitmap.GetSize();

// 	return (size1.x*size1.y) > (size2.x*size2.y);
	return std::max(size1.x, size1.y) > std::max(size2.x, size2.y);
}

DialogReplacePiece::DialogReplacePiece()
{
	Init();
}

DialogReplacePiece::DialogReplacePiece(wxWindow* parent, wxWindowID id /* = wxID_ANY */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxDEFAULT_DIALOG_STYLE */)
{
	Init();
	Create(parent, id, _("Replace Piece"), pos, size, style);
}

DialogReplacePiece::~DialogReplacePiece()
{
	// TODO: 
}

bool DialogReplacePiece::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxDEFAULT_DIALOG_STYLE */)
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

void DialogReplacePiece::Init()
{
	m_BtnAddPiece = NULL;
	m_BtnRemovePiece = NULL;
	m_LstPieces = NULL;
	m_EdtImageId = NULL;
	m_EdtImageWidth = NULL;
	m_EdtImageHeight = NULL;
	m_pCurrImageInfo = NULL;
}

void DialogReplacePiece::CreateControls()
{    
	DialogReplacePiece* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Replace Piece"));
	wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
	itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemStaticBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_RIGHT, 5);

	m_BtnAddPiece = new wxButton( itemStaticBoxSizer3->GetStaticBox(), ID_BUTTON, _("+"), wxDefaultPosition, wxSize(24, 24), 0 );
	itemBoxSizer4->Add(m_BtnAddPiece, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_BtnRemovePiece = new wxButton( itemStaticBoxSizer3->GetStaticBox(), ID_BUTTON1, _("-"), wxDefaultPosition, wxSize(24, 24), 0 );
	m_BtnRemovePiece->Enable(false);
	itemBoxSizer4->Add(m_BtnRemovePiece, 0, wxALIGN_CENTER_VERTICAL, 5);

	wxArrayString m_LstPiecesStrings;
	m_LstPieces = new wxListBox( itemStaticBoxSizer3->GetStaticBox(), ID_LISTBOX, wxDefaultPosition, wxSize(400, 200), m_LstPiecesStrings, wxLB_EXTENDED );
	itemStaticBoxSizer3->Add(m_LstPieces, 0, wxGROW, 5);

	wxStaticBox* itemStaticBoxSizer8Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Image Info"));
	wxStaticBoxSizer* itemStaticBoxSizer8 = new wxStaticBoxSizer(itemStaticBoxSizer8Static, wxVERTICAL);
	itemBoxSizer2->Add(itemStaticBoxSizer8, 0, wxGROW|wxRIGHT, 5);

	wxFlexGridSizer* itemFlexGridSizer9 = new wxFlexGridSizer(0, 2, 2, 2);
	itemStaticBoxSizer8->Add(itemFlexGridSizer9, 0, wxGROW, 5);

	wxStaticText* itemStaticText10 = new wxStaticText( itemStaticBoxSizer8->GetStaticBox(), wxID_STATIC, _("Image Id"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageId = new wxTextCtrl( itemStaticBoxSizer8->GetStaticBox(), ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_EdtImageId->Enable(false);
	itemFlexGridSizer9->Add(m_EdtImageId, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* itemStaticText12 = new wxStaticText( itemStaticBoxSizer8->GetStaticBox(), wxID_STATIC, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer9->Add(itemStaticText12, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageWidth = new wxTextCtrl( itemStaticBoxSizer8->GetStaticBox(), ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer9->Add(m_EdtImageWidth, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	wxStaticText* itemStaticText14 = new wxStaticText( itemStaticBoxSizer8->GetStaticBox(), wxID_STATIC, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer9->Add(itemStaticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	m_EdtImageHeight = new wxTextCtrl( itemStaticBoxSizer8->GetStaticBox(), ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer9->Add(m_EdtImageHeight, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);

	itemFlexGridSizer9->AddGrowableCol(1);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer16 = new wxStdDialogButtonSizer;

	itemBoxSizer2->Add(itemStdDialogButtonSizer16, 0, wxALIGN_RIGHT|wxALL, 5);
	wxButton* itemButton17 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer16->AddButton(itemButton17);

	wxButton* itemButton18 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer16->AddButton(itemButton18);

	itemStdDialogButtonSizer16->Realize();
}

void DialogReplacePiece::OnBtnAddPieceClicked(wxCommandEvent& event)
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

void DialogReplacePiece::OnBtnRemovePieceClicked(wxCommandEvent& event)
{
	RemoveListSelectedItems();
	m_BtnRemovePiece->Enable(false);
	event.Skip();
}

void DialogReplacePiece::OnLstPiecesSelected(wxCommandEvent& event)
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

void DialogReplacePiece::OnOkClicked(wxCommandEvent& event)
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
	bOk = ReplacePieceFromExistingImage(newSize);

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

void DialogReplacePiece::OnCancelClicked(wxCommandEvent& event)
{
	event.Skip();
}

void DialogReplacePiece::InitImageInfo(const ImageInfo* pImageInfo)
{
	if (!pImageInfo)
	{
		m_EdtImageId->SetValue(wxEmptyString);
		m_EdtImageWidth->SetValue(wxString::Format(wxT("%d"), Config::DEFAULT_IMAGE_SIZE));
		m_EdtImageHeight->SetValue(wxString::Format(wxT("%d"), Config::DEFAULT_IMAGE_SIZE));
	}
	else
	{
		m_EdtImageId->SetValue(pImageInfo->GetId());
		const wxBitmap* pBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
		m_EdtImageWidth->SetValue(wxString::Format(wxT("%d"), pBitmap->GetWidth()));
		m_EdtImageHeight->SetValue(wxString::Format(wxT("%d"), pBitmap->GetHeight()));
	}
}

void DialogReplacePiece::AddListItems(const wxArrayString& arrayString)
{
	if (arrayString.size() <= 0) return;

	for (int i = 0; i < (int)arrayString.size(); ++i)
	{
		wxString strPath = arrayString[i];
		m_LstPieces->Append(strPath);
	}
}

void DialogReplacePiece::RemoveListSelectedItems()
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

bool DialogReplacePiece::ReplacePieceFromExistingImage(const wxSize& newSize)
{
	wxString strImageId = m_EdtImageId->GetValue();
	const ImageInfo* pImageInfo = ImagePieceDocument::GetInstance().FindImageInfo(strImageId);
	if (!pImageInfo) return false;

	TV_PACKING_PIECE_INFO vPackingInfo;

	// get packing info from list
	if (!GetPieceFromList(vPackingInfo, pImageInfo))
	{
		FreePackingPiecesInfo(vPackingInfo);
		return false;
	}

	// get packing info from images
	if (!GetRestPieceFromImage(vPackingInfo, pImageInfo))
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

	PieceListTransformer::GetInstance().UpdateListView();
	FreePackingPiecesInfo(vPackingInfo);

	m_pCurrImageInfo = pImageInfo;
	return true;
}

void DialogReplacePiece::FreePackingPiecesInfo(TV_PACKING_PIECE_INFO& vPackingInfo)
{
	for (TV_PACKING_PIECE_INFO::iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		PACKING_PIECE_INFO* pPackingInfo = (*it);
		SAFE_DELETE(pPackingInfo);
	}

	vPackingInfo.clear();
}

wxBitmap* DialogReplacePiece::PackImage(const wxSize& bmpSize, const TV_PACKING_PIECE_INFO& vPackingInfo)
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

bool DialogReplacePiece::GeneratePackingInfo(TV_PACKING_PIECE_INFO& vPackingInfo, const wxSize& bmpSize)
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

bool DialogReplacePiece::GetPieceFromList(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo)
{
	wxArrayString arrayString = m_LstPieces->GetStrings();
	if (arrayString.size() <= 0) return true;

	for (wxArrayString::iterator it = arrayString.begin(); it != arrayString.end(); ++it)
	{
		const wxString& strPath = (*it);

		PACKING_PIECE_INFO* pPackingInfo = new PACKING_PIECE_INFO();
		pPackingInfo->pNode = NULL;

		pPackingInfo->strId = FileUtil::GetFileName(strPath);
		pPackingInfo->strPath = strPath;
		pPackingInfo->pPieceInfo = ImagePieceDocument::GetInstance().FindPieceInfo(pPackingInfo->strId);
		if (!pPackingInfo->pPieceInfo || pPackingInfo->pPieceInfo->GetImageInfo() != pImageInfo)
		{
			m_strError = wxString::Format(_("Can not replace piece (%s),\ncan not find piece (%s) under image (%s)"), strPath, pPackingInfo->strId, pImageInfo->GetId());
			SAFE_DELETE(pPackingInfo);
			return false;
		}

		if (!pPackingInfo->subBitmap.LoadFile(strPath, wxBITMAP_TYPE_ANY))
		{
			m_strError = wxString::Format(_("Load Bimap Failed: %s"), strPath);
			SAFE_DELETE(pPackingInfo);
			return false;
		}
		pPackingInfo->bmpSize = pPackingInfo->subBitmap.GetSize();

		vPackingInfo.push_back(pPackingInfo);
	}

	return true;
}

bool DialogReplacePiece::GetRestPieceFromImage(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo)
{
	ImagePieceDocument::TV_PIECE_INFO PiecesInImage;
	int numPieces = ImagePieceDocument::GetInstance().EnumImagePieces(PiecesInImage, pImageInfo);

	for (ImagePieceDocument::TV_PIECE_INFO::const_iterator it = PiecesInImage.begin(); it != PiecesInImage.end(); ++it)
	{
		const PieceInfo* pPieceInfo = (*it);
		if (FindPackingInfo(vPackingInfo, pPieceInfo)) continue;

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

bool DialogReplacePiece::FindPackingInfo(const TV_PACKING_PIECE_INFO& vPackingInfo, const PieceInfo* pPieceInfo)
{
	for (TV_PACKING_PIECE_INFO::const_iterator it = vPackingInfo.begin(); it != vPackingInfo.end(); ++it)
	{
		const PACKING_PIECE_INFO* pPackingInfo = (*it);
		if (pPackingInfo->pPieceInfo == pPieceInfo) return true;
	}

	return false;
}

void DialogReplacePiece::SetDefaultImageInfo(const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return;

	InitImageInfo(pImageInfo);
}

const ImageInfo* DialogReplacePiece::GetCurrImageInfo()
{
	return m_pCurrImageInfo;
}