/*!
 * \file ColorStyleEditor.cpp
 * \date 2-15-2012 14:16:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ColorStyleEditor.h"
#include "../Config.h"

ColorStyleEditor* ColorStyleEditor::m_pColorStyleEditor = NULL;

ColorStyleEditor::ColorStyleEditor()
:BaseEditor()
{
	Init();
}

ColorStyleEditor::ColorStyleEditor(wxWindow *parent, wxWindowID winid, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxPanelNameStr */)
:BaseEditor(parent, winid, pos, size, style, name)
{
	Init();
}

ColorStyleEditor::~ColorStyleEditor()
{
	Release();
}

void ColorStyleEditor::Init()
{
	m_pColorStyleEditor = this;
	m_pColorStyle = NULL;
	m_eSelState = IStyle::SS_NUM;
	Reset();
}

void ColorStyleEditor::Release()
{
	Reset();
	m_pColorStyleEditor = NULL;
}

ColorStyleEditor& ColorStyleEditor::GetInstance()
{
	return *m_pColorStyleEditor;
}

void ColorStyleEditor::Reset()
{
	SetColorStyle(NULL);
}

bool ColorStyleEditor::SetColorStyle(const ColorStyle* pColorStyle)
{
	if (m_pColorStyle == pColorStyle) return false;
	m_pColorStyle = pColorStyle;

	UpdateSubRect();

	SetSelState(IStyle::SS_NUM);
	UpdateVirtualSize();
	UpdateScrollPosition(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL));
	return true;
}

const ColorStyle* ColorStyleEditor::GetColorStyle()
{
	return m_pColorStyle;
}

wxSize ColorStyleEditor::CalculateVirtualSize()
{
	return m_TotalSize;
}

void ColorStyleEditor::Draw(wxDC& dc)
{
	if (!m_pColorStyle) return;

	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		wxColour color;
		color.SetRGB(m_pColorStyle->GetStateColor((IStyle::STYLE_STATE)i));
		dc.SetBrush(wxBrush(color));
		DrawRectangle(dc, m_rectState[i]);
	}

	DrawSelection(dc);
}

void ColorStyleEditor::DrawSelection(wxDC& dc)
{
	if (m_eSelState < 0 || m_eSelState >= IStyle::SS_NUM) return;
	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	DrawRectangle(dc, m_rectState[m_eSelState]);
}

void ColorStyleEditor::OnLButtonDown(wxMouseEvent& event)
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

void ColorStyleEditor::SetSelState(IStyle::STYLE_STATE eState)
{
	m_eSelState = eState;
	Refresh(false);
}

IStyle::STYLE_STATE ColorStyleEditor::GetSelState() const
{
	return m_eSelState;
}

void ColorStyleEditor::UpdateSubRect()
{
	wxRect subRect(0, 0, 0, 0);
	m_TotalSize.x = 0;
	m_TotalSize.y = 0;

	for (int i = 0; i < IStyle::SS_NUM; ++i)
	{
		if (m_pColorStyle)
		{
			subRect.width = Config::DEFAULT_PIECE_SIZE;
			subRect.height = Config::DEFAULT_PIECE_SIZE;

			m_rectState[i] = subRect;

			subRect.x = 0;
			subRect.y += Config::DEFAULT_PIECE_SIZE;

			if (m_TotalSize.x < Config::DEFAULT_PIECE_SIZE) m_TotalSize.x = Config::DEFAULT_PIECE_SIZE;
			m_TotalSize.y += Config::DEFAULT_PIECE_SIZE;
		}
		else
		{
			m_rectState[i] = subRect;
		}
	}
}
