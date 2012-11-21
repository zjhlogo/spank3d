/*!
 * \file NineGridStyleEditor.cpp
 * \date 2-15-2012 12:45:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "NineGridStyleEditor.h"
#include "../Config.h"

NineGridStyleEditor* NineGridStyleEditor::m_pNineGridStyleEditor = NULL;

NineGridStyleEditor::NineGridStyleEditor()
:BaseEditor()
{
	Init();
}

NineGridStyleEditor::NineGridStyleEditor(wxWindow *parent, wxWindowID winid, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxPanelNameStr */)
:BaseEditor(parent, winid, pos, size, style, name)
{
	Init();
}

NineGridStyleEditor::~NineGridStyleEditor()
{
	Release();
}

void NineGridStyleEditor::Init()
{
	m_pNineGridStyleEditor = this;
	m_pNineGridStyle = NULL;
	Reset();
}

void NineGridStyleEditor::Release()
{
	Reset();
	m_pNineGridStyleEditor = NULL;
}

NineGridStyleEditor& NineGridStyleEditor::GetInstance()
{
	return *m_pNineGridStyleEditor;
}

void NineGridStyleEditor::Reset()
{
	SetNineGridStyle(NULL);
}

bool NineGridStyleEditor::SetNineGridStyle(const NineGridStyle* pNineGridStyle)
{
	if (m_pNineGridStyle == pNineGridStyle) return false;
	m_pNineGridStyle = pNineGridStyle;

	UpdateSubBitmapRect();

	SetSelState(IStyle::SS_NUM);

	UpdateVirtualSize();
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	return true;
}

const NineGridStyle* NineGridStyleEditor::GetNineGridStyle()
{
	return m_pNineGridStyle;
}

wxSize NineGridStyleEditor::CalculateVirtualSize()
{
	return m_TotalSize;
}

void NineGridStyleEditor::Draw(wxDC& dc)
{
	if (!m_pNineGridStyle) return;

	const PieceInfo* pDefaultPieceInfo = NULL;
	const NineGridStyle::NINE_GRID_INFO* pDefaultNineGridInfo = m_pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL);
	if (pDefaultNineGridInfo && pDefaultNineGridInfo->pPieceInfo) pDefaultPieceInfo = pDefaultNineGridInfo->pPieceInfo;

	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		const PieceInfo* pPieceInfo = pDefaultPieceInfo;
		const NineGridStyle::NINE_GRID_INFO* pNineGridInfo = m_pNineGridStyle->GetStateGridInfo((IStyle::STYLE_STATE)i);
		if (pNineGridInfo && pNineGridInfo->pPieceInfo) pPieceInfo = pNineGridInfo->pPieceInfo;

		if (!pPieceInfo)
		{
			dc.SetPen(wxPen(*wxLIGHT_GREY, Config::DISABLED_PEN_WIDTH));
			dc.SetBrush(*wxGREY_BRUSH);
			DrawRectangle(dc, m_rectState[i]);
			DrawLine(dc, wxPoint(m_rectState[i].x, m_rectState[i].y), wxPoint(m_rectState[i].x+m_rectState[i].width, m_rectState[i].y+m_rectState[i].height));
			DrawLine(dc, wxPoint(m_rectState[i].x, m_rectState[i].y+m_rectState[i].height), wxPoint(m_rectState[i].x+m_rectState[i].width, m_rectState[i].y));
		}
		else
		{
			DrawPiece(dc, m_rectState[i].GetPosition(), pPieceInfo);
		}
	}

	DrawSelection(dc);
}

void NineGridStyleEditor::DrawSelection(wxDC& dc)
{
	if (m_eSelState < 0 || m_eSelState >= IStyle::SS_NUM) return;

	const wxRect& rect = m_rectState[m_eSelState];

	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	DrawRectangle(dc, rect);

	if (!m_pNineGridStyle) return;
	wxSize maxSize = CalculateMaxSize();
	const NineGridStyle::NINE_GRID_INFO* pGridInfo = m_pNineGridStyle->GetStateGridInfo(m_eSelState);

	dc.SetPen(*wxGREEN_PEN);
	DrawLine(dc, wxPoint(0, rect.y+pGridInfo->min_y), wxPoint(maxSize.x, rect.y+pGridInfo->min_y));
	DrawLine(dc, wxPoint(0, rect.y+pGridInfo->max_y), wxPoint(maxSize.x, rect.y+pGridInfo->max_y));
	DrawLine(dc, wxPoint(rect.x+pGridInfo->min_x, 0), wxPoint(rect.x+pGridInfo->min_x, maxSize.y));
	DrawLine(dc, wxPoint(rect.x+pGridInfo->max_x, 0), wxPoint(rect.x+pGridInfo->max_x, maxSize.y));
}

void NineGridStyleEditor::OnLButtonDown(wxMouseEvent& event)
{
	wxPoint posOrigin = (event.GetPosition() + GetOriginOffset()) / GetZoom();
	DoSelState(posOrigin);
}

void NineGridStyleEditor::SetSelState(IStyle::STYLE_STATE eState)
{
	m_eSelState = eState;
	Refresh(false);
}

IStyle::STYLE_STATE NineGridStyleEditor::GetSelState() const
{
	return m_eSelState;
}

void NineGridStyleEditor::UpdateSubBitmapRect()
{
	wxRect subRect(0, 0, 0, 0);
	m_TotalSize.x = 0;
	m_TotalSize.y = 0;

	wxSize defaultPieceSize(Config::DEFAULT_PIECE_SIZE, Config::DEFAULT_PIECE_SIZE);

	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		wxSize pieceSize = defaultPieceSize;

		if (m_pNineGridStyle)
		{
			const PieceInfo* pPieceInfo = m_pNineGridStyle->GetStateGridInfo((IStyle::STYLE_STATE)i)->pPieceInfo;
			if (!pPieceInfo) pPieceInfo = m_pNineGridStyle->GetStateGridInfo(IStyle::SS_NORMAL)->pPieceInfo;
			if (pPieceInfo) pieceSize = pPieceInfo->GetRect().GetSize();
		}

		subRect.width = pieceSize.x;
		subRect.height = pieceSize.y;

		m_rectState[i] = subRect;

		subRect.y += pieceSize.y;

		if (m_TotalSize.x < pieceSize.x) m_TotalSize.x = pieceSize.x;
		m_TotalSize.y += pieceSize.y;
	}
}

void NineGridStyleEditor::DoSelState(const wxPoint& pos)
{
	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		if (m_rectState[i].Contains(pos))
		{
			// set selection
			SetSelState((IStyle::STYLE_STATE)i);
			return;
		}
	}

	SetSelState(IStyle::SS_NUM);
}
