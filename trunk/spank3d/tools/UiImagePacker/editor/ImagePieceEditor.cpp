/*!
 * \file ImagePieceEditor.cpp
 * \date 2-14-2012 1:24:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ImagePieceEditor.h"
#include "../document/ImagePieceDocument.h"
#include "../transformer/PieceListTransformer.h"
#include "../transformer/ImageListTransformer.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

ImagePieceEditor* ImagePieceEditor::m_pImagePieceEditor = NULL;

ImagePieceEditor::ImagePieceEditor()
:BaseEditor()
{
	Init();
}

ImagePieceEditor::ImagePieceEditor(wxWindow *parent, wxWindowID winid, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxPanelNameStr */)
:BaseEditor(parent, winid, pos, size, style, name)
{
	Init();
}

ImagePieceEditor::~ImagePieceEditor()
{
	Release();
}

void ImagePieceEditor::Init()
{
	m_pImagePieceEditor = this;
	m_pImageInfo = NULL;
	Reset();
}

void ImagePieceEditor::Release()
{
	Reset();
	m_pImagePieceEditor = NULL;
}

ImagePieceEditor& ImagePieceEditor::GetInstance()
{
	return *m_pImagePieceEditor;
}

void ImagePieceEditor::Reset()
{
	SetSelPieceInfo(NULL);
	SetImageInfo(NULL);
}

bool ImagePieceEditor::SetSelPieceInfo(const PieceInfo* pPieceInfo)
{
	ClearSelections();
	AddSelPieceInfo(pPieceInfo);
	return true;
}

const PieceInfo* ImagePieceEditor::GetSelPieceInfo() const
{
	if (m_vSelPieceInfo.size() == 1) return m_vSelPieceInfo[0];
	return NULL;
}

const ImagePieceEditor::TV_PIECE_INFO& ImagePieceEditor::GetSelections()
{
	return m_vSelPieceInfo;
}

bool ImagePieceEditor::SetImageInfo(const ImageInfo* pImageInfo)
{
	if (m_pImageInfo == pImageInfo) return false;
	m_pImageInfo = pImageInfo;
	ClearSelections();

	UpdateVirtualSize();
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	return true;
}

const ImageInfo* ImagePieceEditor::GetImageInfo() const
{
	return m_pImageInfo;
}

wxSize ImagePieceEditor::CalculateVirtualSize()
{
	if (!m_pImageInfo)	return wxSize(0, 0);

	const wxBitmap* pBitmap = ((ImageInfo*)m_pImageInfo)->GetBitmap();
	if (!pBitmap) return wxSize(0, 0);

	return pBitmap->GetSize();
}

void ImagePieceEditor::Draw(wxDC& dc)
{
	if (!m_pImageInfo) return;

	ImageInfo* pImageInfo = (ImageInfo*)m_pImageInfo;
	dc.SetPen(*wxLIGHT_GREY_PEN);
	if (pImageInfo->GetBitmap())
	{
		DrawRectangle(dc, wxPoint(0, 0), pImageInfo->GetBitmap()->GetSize());
	}

	DrawImage(dc, wxPoint(0, 0), pImageInfo);

	DrawSelections(dc);
}

void ImagePieceEditor::DrawSelections(wxDC& dc)
{
	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	for (TV_PIECE_INFO::const_iterator it = m_vSelPieceInfo.begin(); it != m_vSelPieceInfo.end(); ++it)
	{
		const PieceInfo* pPieceInfo = (*it);
		DrawRectangle(dc, pPieceInfo->GetRect());
	}
}

bool ImagePieceEditor::AddSelPieceInfo(const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return false;
	if (IsPieceInfoSelected(pPieceInfo)) return false;
	m_vSelPieceInfo.push_back(pPieceInfo);
	Refresh(false);
	return true;
}

bool ImagePieceEditor::RemoveSelPieceInfo(const PieceInfo* pPieceInfo)
{
	for (TV_PIECE_INFO::iterator it = m_vSelPieceInfo.begin(); it != m_vSelPieceInfo.end(); ++it)
	{
		if (pPieceInfo == (*it))
		{
			m_vSelPieceInfo.erase(it);
			Refresh(false);
			return true;
		}
	}

	return false;
}

bool ImagePieceEditor::IsPieceInfoSelected(const PieceInfo* pPieceInfo)
{
	for (TV_PIECE_INFO::const_iterator it = m_vSelPieceInfo.begin(); it != m_vSelPieceInfo.end(); ++it)
	{
		if (pPieceInfo == (*it)) return true;
	}

	return false;
}

void ImagePieceEditor::ClearSelections()
{
	m_vSelPieceInfo.clear();
	Refresh(false);
}

void ImagePieceEditor::OnLButtonDown(wxMouseEvent& event)
{
	wxPoint posMouse = (event.GetPosition() + GetOriginOffset()) / GetZoom();
	const PieceInfo* pPieceInfo = ImagePieceDocument::GetInstance().FindPieceInfoUnderPoint(posMouse, m_pImageInfo);

	bool bUpdate = false;
	if (event.ControlDown())
	{
		bUpdate = AddSelPieceInfo(pPieceInfo);
	}
	else
	{
		bUpdate = SetSelPieceInfo(pPieceInfo);
	}

	if (bUpdate)
	{
		PieceListTransformer::GetInstance().SetSelectedPieceInfo(GetSelPieceInfo());
		PieceListTransformer::GetInstance().UpdateProperty(GetSelPieceInfo());
		if (!GetSelPieceInfo() && m_pImageInfo)
		{
			ImageListTransformer::GetInstance().UpdateProperty(m_pImageInfo);
		}
	}
}
