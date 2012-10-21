/*!
 * \file BaseEditor.cpp
 * \date 2-14-2012 17:59:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BaseEditor.h"
#include <wx/dcbuffer.h>

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

BEGIN_EVENT_TABLE(BaseEditor, wxWindow)
	EVT_PAINT(BaseEditor::OnPaintEvt)
	EVT_MOUSEWHEEL(BaseEditor::OnMouseWheelEvt)
	EVT_LEFT_DOWN(BaseEditor::OnLButtonDownEvt)
	EVT_LEFT_UP(BaseEditor::OnLButtonUpEvt)
	EVT_MOTION(BaseEditor::OnMouseMoveEvt)
	EVT_SIZE(BaseEditor::OnSizeEvt)
	EVT_SCROLLWIN_LINEUP(BaseEditor::OnScrollLineUpEvt)
	EVT_SCROLLWIN_LINEDOWN(BaseEditor::OnScrollLineDownEvt)
	EVT_SCROLLWIN_PAGEUP(BaseEditor::OnScrollPageUpEvt)
	EVT_SCROLLWIN_PAGEDOWN(BaseEditor::OnScrollPageDownEvt)
	EVT_SCROLLWIN_THUMBTRACK(BaseEditor::OnScrollThumbTrackEvt)
	EVT_SCROLLWIN_THUMBRELEASE(BaseEditor::OnScrollThumbReleaseEvt)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(BaseEditor, wxWindow)

BaseEditor::BaseEditor()
{
	Init();
}

BaseEditor::BaseEditor(wxWindow *parent, wxWindowID winid, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxPanelNameStr */)
{
	Init();
	Create(parent, winid, pos, size, style, name);
}

BaseEditor::~BaseEditor()
{
	Release();
}

void BaseEditor::Init()
{
	m_nZoom = ZOOM_MIN;

	m_ZoomedSize.Set(DEFAULT_VIRTUAL_SIZE*m_nZoom, DEFAULT_VIRTUAL_SIZE*m_nZoom);
	m_ptOriginOffset.x = 0;
	m_ptOriginOffset.y = 0;
}

void BaseEditor::Release()
{
	// TODO: 
}

bool BaseEditor::Create(wxWindow *parent, wxWindowID winid, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxPanelNameStr */)
{
	if (!wxWindow::Create(parent, winid, pos, size, style, name)) return false;

	return true;
}

wxSize BaseEditor::DoGetBestSize() const
{
	return m_ZoomedSize;
}

void BaseEditor::Reset()
{
	// TODO: 
}

bool BaseEditor::ZoomIn()
{
	return Zoom(m_nZoom+1);
}

bool BaseEditor::ZoomOut()
{
	return Zoom(m_nZoom-1);
}

bool BaseEditor::Zoom(int zoom)
{
	if (zoom < ZOOM_MIN || zoom > ZOOM_MAX) return false;
	if (m_nZoom == zoom) return true;

	m_ptOriginOffset = (m_ptOriginOffset/m_nZoom)*zoom;
	m_nZoom = zoom;
	UpdateVirtualSize();
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));

	return true;
}

int BaseEditor::GetZoom() const
{
	return m_nZoom;
}

void BaseEditor::OnPaintEvt(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);

	// get client size
	wxSize maxSize = CalculateMaxSize();

	// fill background
	dc.SetBackground(*wxLIGHT_GREY_BRUSH);
	dc.Clear();

	// draw cross
	dc.SetBrush(wxBrush(*wxWHITE, wxBRUSHSTYLE_CROSSDIAG_HATCH));
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(wxPoint(0, 0), maxSize*m_nZoom);

	// custom draw
	Draw(dc);
}

void BaseEditor::Draw(wxDC& dc)
{
	// TODO: 
}

void BaseEditor::DrawImage(wxDC& dc, const wxPoint& destPos, const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return;

	const wxBitmap* pBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
	if (!pBitmap) return;

	m_memDC.SelectObject((wxBitmap)(*pBitmap));
	dc.StretchBlit(destPos*m_nZoom-m_ptOriginOffset, pBitmap->GetSize()*m_nZoom, &m_memDC, wxPoint(0, 0), pBitmap->GetSize());
	m_memDC.SelectObject(wxNullBitmap);
}

void BaseEditor::DrawPiece(wxDC& dc, const wxPoint& destPos, const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return;

	const ImageInfo* pImageInfo = pPieceInfo->GetImageInfo();
	if (!pImageInfo) return;

	const wxBitmap* pBitmap = ((ImageInfo*)pImageInfo)->GetBitmap();
	if (!pBitmap) return;

	m_memDC.SelectObject((wxBitmap)(*pBitmap));
	dc.StretchBlit(destPos*m_nZoom-m_ptOriginOffset, pPieceInfo->GetRect().GetSize()*m_nZoom, &m_memDC, pPieceInfo->GetRect().GetPosition(), pPieceInfo->GetRect().GetSize());
	m_memDC.SelectObject(wxNullBitmap);
}

void BaseEditor::DrawRectangle(wxDC& dc, const wxRect& rect)
{
	DrawRectangle(dc, rect.GetPosition(), rect.GetSize());
}

void BaseEditor::DrawRectangle(wxDC& dc, const wxPoint& pos, const wxSize& size)
{
	wxPoint zoomedPos = pos*m_nZoom-m_ptOriginOffset;
	wxSize zoomedSize = size*m_nZoom;
	dc.DrawRectangle(zoomedPos, zoomedSize);
}

void BaseEditor::DrawLine(wxDC& dc, const wxPoint& pt1, const wxPoint& pt2)
{
	wxPoint zoomedPt1 = pt1*m_nZoom-m_ptOriginOffset;
	wxPoint zoomedPt2 = pt2*m_nZoom-m_ptOriginOffset;
	dc.DrawLine(zoomedPt1, zoomedPt2);
}

void BaseEditor::OnMouseWheelEvt(wxMouseEvent& event)
{
	int lines = -event.GetWheelRotation() / event.GetWheelDelta();

	if (event.ShiftDown())
	{
		int nPos = GetScrollPos(wxHORIZONTAL);
		SetScrollPos(wxHORIZONTAL, nPos + lines * event.GetLinesPerAction() * SCROLL_LINE_DISTANCE);
		UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	}
	else if (event.ControlDown())
	{
		if (lines < 0)
		{
			ZoomIn();
		}
		else
		{
			ZoomOut();
		}
	}
	else
	{
		int nPos = GetScrollPos(wxVERTICAL);
		SetScrollPos(wxVERTICAL, nPos + lines * event.GetLinesPerAction() * SCROLL_LINE_DISTANCE);
		UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	}
}

void BaseEditor::OnLButtonDownEvt(wxMouseEvent& event)
{
	SetFocus();
	OnLButtonDown(event);
}

void BaseEditor::OnLButtonUpEvt(wxMouseEvent& event)
{
	OnLButtonUp(event);
}

void BaseEditor::OnMouseMoveEvt(wxMouseEvent& event)
{
	OnMouseMove(event);
}

void BaseEditor::OnSizeEvt(wxSizeEvent& event)
{
	UpdateVirtualSize();
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	event.Skip();
}

void BaseEditor::OnScrollLineUpEvt(wxScrollWinEvent& event)
{
	int nOrientation = event.GetOrientation();

	int nPos = GetScrollPos(nOrientation);
	SetScrollPos(nOrientation, nPos-SCROLL_LINE_DISTANCE, true);
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void BaseEditor::OnScrollLineDownEvt(wxScrollWinEvent& event)
{
	int nOrientation = event.GetOrientation();

	int nPos = GetScrollPos(nOrientation);
	SetScrollPos(nOrientation, nPos+SCROLL_LINE_DISTANCE, true);
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void BaseEditor::OnScrollPageUpEvt(wxScrollWinEvent& event)
{
	int nOrientation = event.GetOrientation();
	int nDistance = GetScrollThumb(nOrientation);
	int nPos = GetScrollPos(nOrientation);
	SetScrollPos(nOrientation, nPos-nDistance, true);
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void BaseEditor::OnScrollPageDownEvt(wxScrollWinEvent& event)
{
	int nOrientation = event.GetOrientation();
	int nDistance = GetScrollThumb(nOrientation);
	int nPos = GetScrollPos(nOrientation);
	SetScrollPos(nOrientation, nPos+nDistance, true);
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void BaseEditor::OnScrollThumbTrackEvt(wxScrollWinEvent& event)
{
	int nOrientation = event.GetOrientation();
	int nPos = event.GetPosition();
	if (nOrientation == wxHORIZONTAL)
	{
		UpdateScrollPosition(nPos, GetScrollPos(wxVERTICAL));
	}
	else
	{
		UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), nPos);
	}
}

void BaseEditor::OnScrollThumbReleaseEvt(wxScrollWinEvent& event)
{
	int nOrientation = event.GetOrientation();
	int nPos = event.GetPosition();
	SetScrollPos(nOrientation, nPos, true);
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
}

void BaseEditor::UpdateVirtualSize()
{
	m_ZoomedSize = CalculateVirtualSize() * m_nZoom;
	wxRect rect = GetClientRect();

	// hide unnessary scroll bar
	bool bShowHorizontal = true;
	if (m_ZoomedSize.GetWidth() <= rect.GetWidth())
	{
		SetScrollbar(wxHORIZONTAL, 0, 0, 0, true);
		bShowHorizontal = false;
	}

	bool bShowVertical = true;
	if (m_ZoomedSize.GetHeight() <= rect.GetHeight())
	{
		SetScrollbar(wxVERTICAL, 0, 0, 0, true);
		bShowVertical = false;
	}

	// get the rect again, it may be difference from the old rect after hide the scroll bar
	rect = GetClientRect();

	// recalculate the scroll bar range
	if (bShowHorizontal)
	{
		SetScrollbar(wxHORIZONTAL, m_ptOriginOffset.x, rect.GetWidth(), m_ZoomedSize.GetWidth(), true);
	}
	if (bShowVertical)
	{
		SetScrollbar(wxVERTICAL, m_ptOriginOffset.y, rect.GetHeight(), m_ZoomedSize.GetHeight(), true);
	}
}

void BaseEditor::UpdateScrollPosition(int x, int y)
{
	m_ptOriginOffset.x = x;
	m_ptOriginOffset.y = y;

	Refresh(false);
}

const wxPoint& BaseEditor::GetOriginOffset() const
{
	return m_ptOriginOffset;
}

wxSize BaseEditor::CalculateVirtualSize()
{
	return wxSize(0, 0);
}

wxSize BaseEditor::CalculateMaxSize()
{
	wxSize maxSize = GetClientSize();
	wxSize virtualSize = CalculateVirtualSize();
	if (maxSize.x < virtualSize.x) maxSize.x = virtualSize.x;
	if (maxSize.y < virtualSize.y) maxSize.y = virtualSize.y;
	return maxSize;
}

void BaseEditor::OnLButtonDown(wxMouseEvent& event)
{
	// TODO: 
}

void BaseEditor::OnLButtonUp(wxMouseEvent& event)
{
	// TODO: 
}

void BaseEditor::OnMouseMove(wxMouseEvent& event)
{
	// TODO: 
}

wxPoint BaseEditor::CalculateZoomedPos(const wxPoint& pos)
{
	return pos*m_nZoom + m_ptOriginOffset;
}

wxPoint BaseEditor::CalculateOriginPos(const wxPoint& zoomedPos)
{
	return (zoomedPos - m_ptOriginOffset) / m_nZoom;
}
