/*!
 * \file ImageInfo.cpp
 * \date 2-13-2012 19:34:32
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <wx/wxprec.h>
#include "ImageInfo.h"
#include "ProjectDocument.h"
#include "../utils/FileUtil.h"
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

ImageInfo::ImageInfo(const wxString& strBaseDir)
{
	m_pBitmap = NULL;
	m_bLoaded = false;
	m_bIsModified = false;

	m_strBaseDir = strBaseDir;
	FileUtil::FormatDir(m_strBaseDir);
}

ImageInfo::~ImageInfo()
{
	SAFE_DELETE(m_pBitmap);
}

bool ImageInfo::LoadFromXml(wxXmlNode* pNodeImage)
{
	if (!pNodeImage) return false;

	m_strId = pNodeImage->GetAttribute(wxT("id"));

	m_strFileName = pNodeImage->GetAttribute(wxT("file_name"));
	FileUtil::FormatDir(m_strFileName);

	return true;
}

bool ImageInfo::SaveToXml(wxXmlNode* pNodeImageList)
{
	wxXmlNode* pNodeImage = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Image"));
	pNodeImage->AddAttribute(wxT("id"), m_strId);
	pNodeImage->AddAttribute(wxT("file_name"), m_strFileName);
	pNodeImage->AddAttribute(wxT("width"), wxString::Format(wxT("%d"), GetBitmap()->GetWidth()));
	pNodeImage->AddAttribute(wxT("height"), wxString::Format(wxT("%d"), GetBitmap()->GetHeight()));
	pNodeImageList->AddChild(pNodeImage);
	return true;
}

bool ImageInfo::SaveImage()
{
	if (!m_bIsModified) return true;
	m_bIsModified = false;
	if (!m_pBitmap) return true;

	wxString strFullPath = m_strBaseDir + wxT("/") + m_strFileName;
	if (!m_pBitmap->SaveFile(strFullPath, wxBITMAP_TYPE_PNG))
	{
		wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("save image bitmap failed, path=%s"), strFullPath));
		msg.ShowModal();
	}

	return true;
}

void ImageInfo::SetId(const wxString& strId)
{
	m_strId = strId;
}

const wxString& ImageInfo::GetId() const
{
	return m_strId;
}

void ImageInfo::SetFileName(const wxString& strFileName)
{
	m_strFileName = strFileName;
}

const wxString& ImageInfo::GetFileName() const
{
	return m_strFileName;
}

const wxString& ImageInfo::GetBaseDir() const
{
	return m_strBaseDir;
}

bool ImageInfo::SetBitmap(wxBitmap* pBitmap)
{
	if (m_pBitmap == pBitmap) return false;

	SAFE_DELETE(m_pBitmap);
	m_pBitmap = pBitmap;
	m_bIsModified = true;
	return true;
}

const wxBitmap* ImageInfo::GetBitmap()
{
	if (!m_pBitmap && !m_bLoaded)
	{
		LoadImageFromFile();
		m_bLoaded = true;
	}

	return m_pBitmap;
}

bool ImageInfo::LoadImageFromFile()
{
	if (m_pBitmap) return false;
	SAFE_DELETE(m_pBitmap);

	// create new bitmap
	m_pBitmap = new wxBitmap();

	// load bitmap from path
	wxString strFullPath = m_strBaseDir + wxT("/") + m_strFileName;
	if (!m_pBitmap->LoadFile(strFullPath, wxBITMAP_TYPE_ANY))
	{
		wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("can not open file: %s"), strFullPath));
		msg.ShowModal();
		SAFE_DELETE(m_pBitmap);
		return false;
	}

	return true;
}

void ImageInfo::SetTreeItemId(const wxTreeItemId& itemId)
{
	m_TreeItemId = itemId;
}

const wxTreeItemId& ImageInfo::GetTreeItemId() const
{
	return m_TreeItemId;
}

bool ImageInfo::ClearBitmapArea(const wxRect& rect)
{
	if (!m_pBitmap) return false;
	m_bIsModified = true;

	wxMemoryDC memDC;
	memDC.SelectObject(*m_pBitmap);
	memDC.SetBrush(*wxBLACK_BRUSH);
	memDC.SetPen(*wxBLACK_PEN);
	memDC.DrawRectangle(rect);
	memDC.SelectObject(wxNullBitmap);

	return true;
}
