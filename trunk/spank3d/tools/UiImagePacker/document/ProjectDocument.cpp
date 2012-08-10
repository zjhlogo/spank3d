/*!
 * \file ProjectDocument.cpp
 * \date 2-14-2012 13:21:35
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ProjectDocument.h"
#include "ImagePieceDocument.h"
#include "BitmapStyleDocument.h"
#include "NineGridStyleDocument.h"
#include "ColorStyleDocument.h"
#include "ClipBitmapStyleDocument.h"
#include "../utils/FileUtil.h"

ProjectDocument::ProjectDocument()
{
	// TODO: 
}

ProjectDocument::~ProjectDocument()
{
	// TODO: 
}

ProjectDocument& ProjectDocument::GetInstance()
{
	static ProjectDocument s_ProjectDocument;
	return s_ProjectDocument;
}

bool ProjectDocument::OpenFile(const wxString& strFile)
{
	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	Reset();
	SetFilePath(strFile);

	wxXmlNode* pNodeIpProject = doc.GetRoot();
	if (!pNodeIpProject || pNodeIpProject->GetName() != wxT("ip_project")) return false;

	m_strRootDir = pNodeIpProject->GetAttribute(wxT("root_dir"));
	FileUtil::FormatDir(m_strRootDir);

	wxXmlNode* pNodeImagePiece = this->FindXmlChild(pNodeIpProject, wxT("image_piece"));
	if (!pNodeImagePiece) return false;
	m_strImagePieceFilePath = (m_strProjectDir + wxT("/") + pNodeImagePiece->GetAttribute(wxT("path")));

	wxXmlNode* pNodeBitmapStyle = this->FindXmlChild(pNodeIpProject, wxT("bitmap_style"));
	if (!pNodeBitmapStyle) return false;
	m_strBitmapStyleFilePath = (m_strProjectDir + wxT("/") + pNodeBitmapStyle->GetAttribute(wxT("path")));

	wxXmlNode* pNodeNineGridStyle = this->FindXmlChild(pNodeIpProject, wxT("nine_grid_style"));
	if (!pNodeNineGridStyle) return false;
	m_strNineGridStyleFilePath = (m_strProjectDir + wxT("/") + pNodeNineGridStyle->GetAttribute(wxT("path")));

	wxXmlNode* pNodeColorStyle = this->FindXmlChild(pNodeIpProject, wxT("color_style"));
	if (!pNodeColorStyle) return false;
	m_strColorStyleFilePath = (m_strProjectDir + wxT("/") + pNodeColorStyle->GetAttribute(wxT("path")));

	wxXmlNode* pNodeClipBitmapStyle = this->FindXmlChild(pNodeIpProject, wxT("clip_bitmap_style"));
	if (!pNodeClipBitmapStyle) return false;
	m_strClipBitmapStyleFilePath = (m_strProjectDir + wxT("/") + pNodeClipBitmapStyle->GetAttribute(wxT("path")));

	return true;
}

bool ProjectDocument::SaveFile(const wxString& strFile)
{
	if (!IsModified()) return true;
	ClearModifiedFlag();

	wxXmlDocument doc;
	wxXmlNode* pNodeIpProject = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ip_project"));
	doc.SetRoot(pNodeIpProject);
	pNodeIpProject->AddAttribute(wxT("root_dir"), m_strRootDir);

	wxXmlNode* pNodeImagePiece = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("image_piece"));
	pNodeIpProject->AddChild(pNodeImagePiece);
	pNodeImagePiece->AddAttribute(wxT("path"), FileUtil::RemoveRootDir(ImagePieceDocument::GetInstance().GetFilePath(), m_strProjectDir + wxT("/")));

	wxXmlNode* pNodeBitmapStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("bitmap_style"));
	pNodeIpProject->AddChild(pNodeBitmapStyle);
	pNodeBitmapStyle->AddAttribute(wxT("path"), FileUtil::RemoveRootDir(BitmapStyleDocument::GetInstance().GetFilePath(), m_strProjectDir + wxT("/")));

	wxXmlNode* pNodeNineGridStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("nine_grid_style"));
	pNodeIpProject->AddChild(pNodeNineGridStyle);
	pNodeNineGridStyle->AddAttribute(wxT("path"), FileUtil::RemoveRootDir(NineGridStyleDocument::GetInstance().GetFilePath(), m_strProjectDir + wxT("/")));

	wxXmlNode* pNodeColorStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("color_style"));
	pNodeIpProject->AddChild(pNodeColorStyle);
	pNodeColorStyle->AddAttribute(wxT("path"), FileUtil::RemoveRootDir(ColorStyleDocument::GetInstance().GetFilePath(), m_strProjectDir + wxT("/")));

	wxXmlNode* pElmClipBitmapStyle = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("clip_bitmap_style"));
	pNodeIpProject->AddChild(pElmClipBitmapStyle);
	pElmClipBitmapStyle->AddAttribute(wxT("path"), FileUtil::RemoveRootDir(ClipBitmapStyleDocument::GetInstance().GetFilePath(), m_strProjectDir + wxT("/")));

	return doc.Save(strFile);
}

void ProjectDocument::Reset()
{
	ClearModifiedFlag();
}

const wxString& ProjectDocument::GetRootDir() const
{
	return m_strRootDir;
}

wxString ProjectDocument::GetRootPath() const
{
	return m_strProjectDir + wxT("/") + m_strRootDir;
}

void ProjectDocument::SetProjectDir(const wxString& strProjectDir)
{
	m_strProjectDir = strProjectDir;
	FileUtil::FormatDir(m_strProjectDir);
}

const wxString& ProjectDocument::GetProjectDir() const
{
	return m_strProjectDir;
}

bool ProjectDocument::SetImagePieceFilePath(const wxString& strPath)
{
	m_strImagePieceFilePath = strPath;
	return true;
}

const wxString& ProjectDocument::GetImagePieceFilePath() const
{
	return m_strImagePieceFilePath;
}

bool ProjectDocument::SetBitmapStyleFilePath(const wxString& strPath)
{
	m_strBitmapStyleFilePath = strPath;
	return true;
}

const wxString& ProjectDocument::GetBitmapStyleFilePath() const
{
	return m_strBitmapStyleFilePath;
}

bool ProjectDocument::SetNineGridStyleFilePath(const wxString& strPath)
{
	m_strNineGridStyleFilePath = strPath;
	return true;
}

const wxString& ProjectDocument::GetNineGridStyleFilePath() const
{
	return m_strNineGridStyleFilePath;
}

bool ProjectDocument::SetColorStyleFilePath(const wxString& strPath)
{
	m_strColorStyleFilePath = strPath;
	return true;
}

const wxString& ProjectDocument::GetColorStyleFilePath() const
{
	return m_strColorStyleFilePath;
}

bool ProjectDocument::SetClipBitmapStyleFilePath(const wxString& strPath)
{
	m_strClipBitmapStyleFilePath = strPath;
	return true;
}

const wxString& ProjectDocument::GetClipBitmapStyleFilePath() const
{
	return m_strClipBitmapStyleFilePath;
}
