/*!
 * \file DocProject.cpp
 * \date 11-22-2012 9:14:07
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "DocProject.h"
#include "../utils/StringUtil.h"
#include <wx/xml/xml.h>

DocProject::DocProject()
{
	// TODO: 
}

DocProject::~DocProject()
{
	Reset();
}

DocProject& DocProject::GetInstance()
{
	static DocProject s_DocProject;
	return s_DocProject;
}

const DocProject::TM_PIECE_INFO& DocProject::GetPieceInfoMap() const
{
	return m_pieceInfoMap;
}

const DocProject::TM_BITMAP_FONT& DocProject::GetBitmapFontMap() const
{
	return m_bitmapFontMap;
}

const DocProject::TM_BITMAP_STYLE& DocProject::GetBitmapStyleMap() const
{
	return m_bitmapStyleMap;
}

const DocProject::TM_NINE_PATCH_STYLE& DocProject::GetNinePatchStyleMap() const
{
	return m_ninePatchStyleMap;
}

const DocProject::TM_VERTICAL_PATCH_STYLE& DocProject::GetVerticalPatchStyleMap() const
{
	return m_verticalPatchStyleMap;
}

const DocProject::TM_HORIZONTAL_PATCH_STYLE& DocProject::GetHorizontalPatchStyleMap() const
{
	return m_horizontalPatchStyleMap;
}

const DocProject::TM_BITMAP_FONT_STYLE& DocProject::GetBitmapFontStyleMap() const
{
	return m_bitmapFontStyleMap;
}

bool DocProject::DoOpen(const wxString& strFile)
{
	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	wxXmlNode* pXmlUiRes = doc.GetRoot();
	if (!pXmlUiRes || pXmlUiRes->GetName() != wxT("UiRes")) return false;

	m_strRootDir = pXmlUiRes->GetAttribute(wxT("root"));

	// load piece info
	wxXmlNode* pXmlPieceInfo = FindChildNode(pXmlUiRes, wxT("PieceInfo"));
	while (pXmlPieceInfo)
	{
		DocPieceInfo* pPieceInfo = new DocPieceInfo();
		if (pPieceInfo->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlPieceInfo->GetAttribute(wxT("path"))))
		{
			m_pieceInfoMap.insert(std::make_pair(pPieceInfo->GetFileName(), pPieceInfo));
		}
		else
		{
			wxDELETE(pPieceInfo);
		}

		pXmlPieceInfo = FindNextSiblingNode(pXmlPieceInfo, wxT("PieceInfo"));
	}

	// load bitmap font
	wxXmlNode* pXmlBitmapFont = FindChildNode(pXmlUiRes, wxT("BitmapFont"));
	while (pXmlBitmapFont)
	{
		DocBitmapFont* pBitmapFont = new DocBitmapFont();
		if (pBitmapFont->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlBitmapFont->GetAttribute(wxT("path"))))
		{
			m_bitmapFontMap.insert(std::make_pair(pBitmapFont->GetFileName(), pBitmapFont));
		}
		else
		{
			wxDELETE(pBitmapFont);
		}

		pXmlBitmapFont = FindNextSiblingNode(pXmlBitmapFont, wxT("BitmapFont"));
	}

	// load bitmap style
	wxXmlNode* pXmlBitmapStyle = FindChildNode(pXmlUiRes, wxT("BitmapStyle"));
	while (pXmlBitmapStyle)
	{
		DocBitmapStyle* pBitmapStyle = new DocBitmapStyle();
		if (pBitmapStyle->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlBitmapStyle->GetAttribute(wxT("path"))))
		{
			m_bitmapStyleMap.insert(std::make_pair(pBitmapStyle->GetFileName(), pBitmapStyle));
		}
		else
		{
			wxDELETE(pBitmapStyle);
		}

		pXmlBitmapStyle = FindNextSiblingNode(pXmlBitmapStyle, wxT("BitmapStyle"));
	}

	// load nine patch style
	wxXmlNode* pXmlNinePatchStyle = FindChildNode(pXmlUiRes, wxT("NinePatchStyle"));
	while (pXmlNinePatchStyle)
	{
		DocNinePatchStyle* pNinePatchStyle = new DocNinePatchStyle();
		if (pNinePatchStyle->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlNinePatchStyle->GetAttribute(wxT("path"))))
		{
			m_ninePatchStyleMap.insert(std::make_pair(pNinePatchStyle->GetFileName(), pNinePatchStyle));
		}
		else
		{
			wxDELETE(pNinePatchStyle);
		}

		pXmlNinePatchStyle = FindNextSiblingNode(pXmlNinePatchStyle, wxT("NinePatchStyle"));
	}

	// load horizontal patch style
	wxXmlNode* pXmlHorizontalPatchStyle = FindChildNode(pXmlUiRes, wxT("HorizontalPatchStyle"));
	while (pXmlHorizontalPatchStyle)
	{
		DocHorizontalPatchStyle* pHorizontalPatchStyle = new DocHorizontalPatchStyle();
		if (pHorizontalPatchStyle->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlHorizontalPatchStyle->GetAttribute(wxT("path"))))
		{
			m_horizontalPatchStyleMap.insert(std::make_pair(pHorizontalPatchStyle->GetFileName(), pHorizontalPatchStyle));
		}
		else
		{
			wxDELETE(pHorizontalPatchStyle);
		}

		pXmlHorizontalPatchStyle = FindNextSiblingNode(pXmlHorizontalPatchStyle, wxT("HorizontalPatchStyle"));
	}

	// load vertical patch style
	wxXmlNode* pXmlVerticalPatchStyle = FindChildNode(pXmlUiRes, wxT("VerticalPatchStyle"));
	while (pXmlVerticalPatchStyle)
	{
		DocVerticalPatchStyle* pVerticalPatchStyle = new DocVerticalPatchStyle();
		if (pVerticalPatchStyle->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlVerticalPatchStyle->GetAttribute(wxT("path"))))
		{
			m_verticalPatchStyleMap.insert(std::make_pair(pVerticalPatchStyle->GetFileName(), pVerticalPatchStyle));
		}
		else
		{
			wxDELETE(pVerticalPatchStyle);
		}

		pXmlVerticalPatchStyle = FindNextSiblingNode(pXmlVerticalPatchStyle, wxT("VerticalPatchStyle"));
	}

	// load bitmap font style
	wxXmlNode* pXmlBitmapFontStyle = FindChildNode(pXmlUiRes, wxT("BitmapFontStyle"));
	while (pXmlBitmapFontStyle)
	{
		DocBitmapFontStyle* pBitmapFontStyle = new DocBitmapFontStyle();
		if (pBitmapFontStyle->Open(m_strBaseDir+wxT("/")+m_strRootDir, pXmlBitmapFontStyle->GetAttribute(wxT("path"))))
		{
			m_bitmapFontStyleMap.insert(std::make_pair(pBitmapFontStyle->GetFileName(), pBitmapFontStyle));
		}
		else
		{
			wxDELETE(pBitmapFontStyle);
		}

		pXmlBitmapFontStyle = FindNextSiblingNode(pXmlBitmapFontStyle, wxT("BitmapFontStyle"));
	}

	wxDocEvent event(wxEVT_DOCUMENT_RESET);
	event.SetEventObject(this);
	ProcessEvent(event);

	return true;
}

bool DocProject::DoSave(const wxString& strFile)
{
	// TODO: 
	return false;
}

void DocProject::Reset()
{
	for (TM_PIECE_INFO::iterator it = m_pieceInfoMap.begin(); it != m_pieceInfoMap.end(); ++it)
	{
		DocPieceInfo* pPieceInfo = it->second;
		wxDELETE(pPieceInfo);
	}
	m_pieceInfoMap.clear();

	for (TM_BITMAP_FONT::iterator it = m_bitmapFontMap.begin(); it != m_bitmapFontMap.end(); ++it)
	{
		DocBitmapFont* pBitmapFont = it->second;
		wxDELETE(pBitmapFont);
	}
	m_bitmapFontMap.clear();

	for (TM_BITMAP_STYLE::iterator it = m_bitmapStyleMap.begin(); it != m_bitmapStyleMap.end(); ++it)
	{
		DocBitmapStyle* pBitmapStyle = it->second;
		wxDELETE(pBitmapStyle);
	}
	m_bitmapStyleMap.clear();

	for (TM_NINE_PATCH_STYLE::iterator it = m_ninePatchStyleMap.begin(); it != m_ninePatchStyleMap.end(); ++it)
	{
		DocNinePatchStyle* pNinePatchStyle = it->second;
		wxDELETE(pNinePatchStyle);
	}
	m_ninePatchStyleMap.clear();

	for (TM_VERTICAL_PATCH_STYLE::iterator it = m_verticalPatchStyleMap.begin(); it != m_verticalPatchStyleMap.end(); ++it)
	{
		DocVerticalPatchStyle* pVerticalPatchStyle = it->second;
		wxDELETE(pVerticalPatchStyle);
	}
	m_verticalPatchStyleMap.clear();

	for (TM_HORIZONTAL_PATCH_STYLE::iterator it = m_horizontalPatchStyleMap.begin(); it != m_horizontalPatchStyleMap.end(); ++it)
	{
		DocHorizontalPatchStyle* pHorizontalPatchStyle = it->second;
		wxDELETE(pHorizontalPatchStyle);
	}
	m_horizontalPatchStyleMap.clear();

	for (TM_BITMAP_FONT_STYLE::iterator it = m_bitmapFontStyleMap.begin(); it != m_bitmapFontStyleMap.end(); ++it)
	{
		DocBitmapFontStyle* pBitmapFontStyle = it->second;
		wxDELETE(pBitmapFontStyle);
	}
	m_bitmapFontStyleMap.clear();
}
