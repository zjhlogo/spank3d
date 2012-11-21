/*!
 * \file ClipBitmapStyleEditor.cpp
 * \date 2-15-2012 11:01:35
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ClipBitmapStyleEditor.h"
#include "../Config.h"

ClipBitmapStyleEditor* ClipBitmapStyleEditor::m_pClipBitmapStyleEditor = NULL;

ClipBitmapStyleEditor::ClipBitmapStyleEditor()
:BaseEditor()
{
	Init();
}

ClipBitmapStyleEditor::ClipBitmapStyleEditor(wxWindow *parent, wxWindowID winid, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxPanelNameStr */)
:BaseEditor(parent, winid, pos, size, style, name)
{
	Init();
}

ClipBitmapStyleEditor::~ClipBitmapStyleEditor()
{
	Release();
}

void ClipBitmapStyleEditor::Init()
{
	m_pClipBitmapStyleEditor = this;
	m_pClipBitmapStyle = NULL;
	m_eSelState = IStyle::SS_NUM;
	Reset();
}

void ClipBitmapStyleEditor::Release()
{
	Reset();
	m_pClipBitmapStyleEditor = NULL;
}

ClipBitmapStyleEditor& ClipBitmapStyleEditor::GetInstance()
{
	return *m_pClipBitmapStyleEditor;
}

void ClipBitmapStyleEditor::Reset()
{
	SetClipBitmapStyle(NULL);
}

bool ClipBitmapStyleEditor::SetClipBitmapStyle(const ClipBitmapStyle* pClipBitmapStyle)
{
	if (m_pClipBitmapStyle == pClipBitmapStyle) return false;
	m_pClipBitmapStyle = pClipBitmapStyle;

	UpdateSubBitmapRect();

	SetSelState(IStyle::SS_NUM);
	UpdateVirtualSize();
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	return true;
}

const ClipBitmapStyle* ClipBitmapStyleEditor::GetClipBitmapStyle() const
{
	return m_pClipBitmapStyle;
}

wxSize ClipBitmapStyleEditor::CalculateVirtualSize()
{
	return m_TotalSize;
}

void ClipBitmapStyleEditor::Draw(wxDC& dc)
{
	if (!m_pClipBitmapStyle) return;

	const PieceInfo* pDefaultPieceInfo = m_pClipBitmapStyle->GetStatePiece(IStyle::SS_NORMAL);

	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		const PieceInfo* pPieceInfo = pDefaultPieceInfo;
		const PieceInfo* pCurrPieceInfo = m_pClipBitmapStyle->GetStatePiece((IStyle::STYLE_STATE)i);
		if (pCurrPieceInfo) pPieceInfo = pCurrPieceInfo;

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

void ClipBitmapStyleEditor::DrawSelection(wxDC& dc)
{
	if (m_eSelState < 0 || m_eSelState >= IStyle::SS_NUM) return;
	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	DrawRectangle(dc, m_rectState[m_eSelState]);
}

void ClipBitmapStyleEditor::OnLButtonDown(wxMouseEvent& event)
{
	wxPoint posOrigin = (event.GetPosition() + GetOriginOffset()) / GetZoom();
	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		if (m_rectState[i].Contains(posOrigin))
		{
			// set selection
			SetSelState((IStyle::STYLE_STATE)i);
			return;
		}
	}

	SetSelState(IStyle::SS_NUM);
}

void ClipBitmapStyleEditor::SetSelState(IStyle::STYLE_STATE eState)
{
	m_eSelState = eState;
	Refresh(false);
}

IStyle::STYLE_STATE ClipBitmapStyleEditor::GetSelState() const
{
	return m_eSelState;
}

void ClipBitmapStyleEditor::UpdateSubBitmapRect()
{
	wxRect subRect(0, 0, 0, 0);
	m_TotalSize.x = 0;
	m_TotalSize.y = 0;

	wxSize defaultPieceSize(Config::DEFAULT_PIECE_SIZE, Config::DEFAULT_PIECE_SIZE);

	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		wxSize pieceSize = defaultPieceSize;

		if (m_pClipBitmapStyle)
		{
			const PieceInfo* pPieceInfo = m_pClipBitmapStyle->GetStatePiece((IStyle::STYLE_STATE)i);
			if (!pPieceInfo) pPieceInfo = m_pClipBitmapStyle->GetStatePiece(IStyle::SS_NORMAL);
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
