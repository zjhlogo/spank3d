/*!
 * \file ImagePieceDocument.cpp
 * \date 2-14-2012 1:23:54
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ImagePieceDocument.h"
#include <wx/msgdlg.h>
#include "../ImagePackerFrame.h"

#define SAFE_DELETE(x) if (x) {delete (x); (x) = NULL;}

ImagePieceDocument::ImagePieceDocument()
{
	m_bNeedUpdateImageIds = false;
	m_bNeedUpdatePieceIds = false;
}

ImagePieceDocument::~ImagePieceDocument()
{
	Reset();
}

bool ImagePieceDocument::OpenFile(const wxString& strFile)
{
	wxXmlDocument doc;
	if (!doc.Load(strFile)) return false;

	Reset();
	SetFilePath(strFile);

	wxXmlNode* pNodeImagePiece = doc.GetRoot();
	if (!pNodeImagePiece || pNodeImagePiece->GetName() != wxT("ImagePiece")) return false;

	wxXmlNode* pNodeImageList = this->FindXmlChild(pNodeImagePiece, wxT("ImageList"));
	if (!pNodeImageList) return false;

	// parse image list
	wxXmlNode* pNodeImage = this->FindXmlChild(pNodeImageList, wxT("Image"));
	while (pNodeImage)
	{
		ImageInfo* pImageInfo = new ImageInfo();
		if (!pImageInfo->LoadFromXml(pNodeImage))
		{
			wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("load image failed, id=%s"), pImageInfo->GetId()));
			msg.ShowModal();
			SAFE_DELETE(pImageInfo);
			SetModifiedFlag();
		}
		else
		{
			m_ImageInfoMap.insert(std::make_pair(pImageInfo->GetId(), pImageInfo));
		}
		pNodeImage = this->GetNextXml(pNodeImage, wxT("Image"));
	}
	m_bNeedUpdateImageIds = true;

	wxXmlNode* pNodePieceList = this->FindXmlChild(pNodeImagePiece, wxT("PieceList"));
	if (!pNodePieceList) return false;

	// parse piece list
	wxXmlNode* pNodePiece = this->FindXmlChild(pNodePieceList, wxT("Piece"));
	while (pNodePiece)
	{
		PieceInfo* pPieceInfo = new PieceInfo();
		if (!pPieceInfo->LoadFromXml(pNodePiece))
		{
			wxMessageDialog msg(&ImagePackerFrame::GetInstance(), wxString::Format(_("load piece failed, id=%s"), pPieceInfo->GetId()));
			msg.ShowModal();
			SAFE_DELETE(pPieceInfo);
			SetModifiedFlag();
		}
		else
		{
			m_PieceInfoMap.insert(std::make_pair(pPieceInfo->GetId(), pPieceInfo));
		}
		pNodePiece = this->GetNextXml(pNodePiece, wxT("Piece"));
	}
	m_bNeedUpdatePieceIds = true;

	return true;
}

bool ImagePieceDocument::SaveFile(const wxString& strFile)
{
	if (!IsModified()) return true;
	ClearModifiedFlag();

	wxXmlDocument doc;
	wxXmlNode* pNodeImagePiece = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ImagePiece"));
	doc.SetRoot(pNodeImagePiece);

	wxXmlNode* pNodeImageList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ImageList"));
	pNodeImagePiece->AddChild(pNodeImageList);
	for (TM_IMAGE_INFO::iterator it = m_ImageInfoMap.begin(); it != m_ImageInfoMap.end(); ++it)
	{
		ImageInfo* pImageInfo = it->second;
		pImageInfo->SaveToXml(pNodeImageList);
		pImageInfo->SaveImage();
	}

	wxXmlNode* pElmPieceList = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("PieceList"));
	pNodeImagePiece->AddChild(pElmPieceList);
	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		PieceInfo* pPieceInfo = it->second;
		pPieceInfo->SaveToXml(pElmPieceList);
	}

	return doc.Save(strFile);
}

void ImagePieceDocument::Reset()
{
	SetFilePath(wxEmptyString);
	for (TM_IMAGE_INFO::iterator it = m_ImageInfoMap.begin(); it != m_ImageInfoMap.end(); ++it)
	{
		ImageInfo* pImageInfo = it->second;
		SAFE_DELETE(pImageInfo);
	}
	m_ImageInfoMap.clear();

	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		PieceInfo* pPieceInfo = it->second;
		SAFE_DELETE(pPieceInfo);
	}
	m_PieceInfoMap.clear();

	m_ImageIds.Clear();
	m_ImageIdsIndex.Clear();

	m_PieceIds.Clear();
	m_PieceIdsIndex.Clear();

	m_bNeedUpdateImageIds = false;
	m_bNeedUpdatePieceIds = false;

	ClearModifiedFlag();
}

ImagePieceDocument& ImagePieceDocument::GetInstance()
{
	static ImagePieceDocument s_UIImagePieceDocument;
	return s_UIImagePieceDocument;
}

const ImageInfo* ImagePieceDocument::FindImageInfo(const wxString& strId)
{
	TM_IMAGE_INFO::iterator itfound = m_ImageInfoMap.find(strId);
	if (itfound == m_ImageInfoMap.end()) return NULL;
	return itfound->second;
}

const PieceInfo* ImagePieceDocument::FindPieceInfo(const wxString& strId)
{
	TM_PIECE_INFO::iterator itfound = m_PieceInfoMap.find(strId);
	if (itfound == m_PieceInfoMap.end()) return NULL;
	return itfound->second;
}

const PieceInfo* ImagePieceDocument::FindPieceInfoUnderPoint(const wxPoint& pos, const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return NULL;

	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		PieceInfo* pPieceInfo = it->second;
		if (pPieceInfo->GetImageInfo() != pImageInfo) continue;

		if (pPieceInfo->GetRect().Contains(pos)) return pPieceInfo;
	}

	return NULL;
}

int ImagePieceDocument::EnumImagePieces(TV_PIECE_INFO& vPieceOut, const ImageInfo* pImageInfo)
{
	int nFound = 0;
	for (TM_PIECE_INFO::const_iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		const PieceInfo* pPieceInfo = it->second;
		if (pPieceInfo->GetImageInfo() == pImageInfo)
		{
			vPieceOut.push_back(pPieceInfo);
			nFound++;
		}
	}

	return nFound;
}

const ImagePieceDocument::TM_IMAGE_INFO& ImagePieceDocument::GetImageInfoMap()
{
	return m_ImageInfoMap;
}

const ImagePieceDocument::TM_PIECE_INFO& ImagePieceDocument::GetPieceInfoMap()
{
	return m_PieceInfoMap;
}

const wxArrayString& ImagePieceDocument::GetImageIds()
{
	UpdateImageArrayString();
	return m_ImageIds;
}

const wxArrayInt& ImagePieceDocument::GetImageIdsIndex()
{
	UpdateImageArrayString();
	return m_ImageIdsIndex;
}

int ImagePieceDocument::FindImageIndex(const wxString& strId)
{
	UpdateImageArrayString();
	for (size_t i = 0; i < m_ImageIds.GetCount(); ++i)
	{
		if (m_ImageIds[i] == strId) return i;
	}

	return 0;
}

const wxString& ImagePieceDocument::GetImageId(int index)
{
	UpdateImageArrayString();
	return m_ImageIds[index];
}

const wxArrayString& ImagePieceDocument::GetPieceIds()
{
	UpdatePieceArrayString();
	return m_PieceIds;
}

const wxArrayInt& ImagePieceDocument::GetPieceIdsIndex()
{
	UpdatePieceArrayString();
	return m_PieceIdsIndex;
}

int ImagePieceDocument::FindPieceIndex(const PieceInfo* pPieceInfo)
{
	if (!pPieceInfo) return 0;
	return FindPieceIndex(pPieceInfo->GetId());
}

int ImagePieceDocument::FindPieceIndex(const wxString& strId)
{
	UpdatePieceArrayString();
	for (size_t i = 0; i < m_PieceIds.GetCount(); ++i)
	{
		if (m_PieceIds[i] == strId) return i;
	}

	return 0;
}

const wxString& ImagePieceDocument::GetPieceId(int index)
{
	UpdatePieceArrayString();
	return m_PieceIds[index];
}

bool ImagePieceDocument::RenameImageInfoId(const ImageInfo* pImageInfo, const wxString& strNewId)
{
	if (!pImageInfo) return false;

	wxString strOldId = pImageInfo->GetId();
	if (strOldId == strNewId) return true;
	if (FindImageInfo(strNewId)) return false;

	TM_IMAGE_INFO::iterator itfound = m_ImageInfoMap.find(strOldId);
	if (itfound == m_ImageInfoMap.end()) return false;
	SetModifiedFlag();

	ImageInfo* pFoundImageInfo = itfound->second;
	m_ImageInfoMap.erase(itfound);

	pFoundImageInfo->SetId(strNewId);
	m_ImageInfoMap.insert(std::make_pair(pFoundImageInfo->GetId(), pFoundImageInfo));

	m_bNeedUpdateImageIds = true;
	return true;
}

bool ImagePieceDocument::RenamePieceInfoId(const PieceInfo* pPieceInfo, const wxString& strNewId)
{
	if (!pPieceInfo) return false;

	wxString strOldId = pPieceInfo->GetId();
	if (strOldId == strNewId) return true;
	if (FindPieceInfo(strNewId)) return false;

	TM_PIECE_INFO::iterator itfound = m_PieceInfoMap.find(strOldId);
	if (itfound == m_PieceInfoMap.end()) return false;
	SetModifiedFlag();

	PieceInfo* pFoundPieceInfo = itfound->second;
	m_PieceInfoMap.erase(itfound);

	pFoundPieceInfo->SetId(strNewId);
	m_PieceInfoMap.insert(std::make_pair(pFoundPieceInfo->GetId(), pFoundPieceInfo));

	m_bNeedUpdatePieceIds = true;
	return true;
}

bool ImagePieceDocument::SetImageBitmap(const ImageInfo* pImageInfo, wxBitmap* pNewBitmap)
{
	if (!pImageInfo) return false;

	TM_IMAGE_INFO::iterator itfound = m_ImageInfoMap.find(pImageInfo->GetId());
	if (itfound == m_ImageInfoMap.end()) return false;
	SetModifiedFlag();

	ImageInfo* pFoundImageInfo = itfound->second;

	pFoundImageInfo->SetBitmap(pNewBitmap);
	return true;
}

bool ImagePieceDocument::ClearImageBitmapArea(const ImageInfo* pImageInfo, const wxRect& rect)
{
	if (!pImageInfo) return false;

	TM_IMAGE_INFO::iterator itfound = m_ImageInfoMap.find(pImageInfo->GetId());
	if (itfound == m_ImageInfoMap.end()) return false;
	SetModifiedFlag();

	ImageInfo* pFoundImageInfo = itfound->second;

	pFoundImageInfo->ClearBitmapArea(rect);
	return true;
}

bool ImagePieceDocument::SetPieceRect(const PieceInfo* pPieceInfo, const wxRect& rect)
{
	if (!pPieceInfo) return false;

	TM_PIECE_INFO::iterator itfound = m_PieceInfoMap.find(pPieceInfo->GetId());
	if (itfound == m_PieceInfoMap.end()) return false;
	SetModifiedFlag();

	PieceInfo* pFoundPieceInfo = itfound->second;
	pFoundPieceInfo->SetRect(rect);
	return true;
}

bool ImagePieceDocument::SetPieceImageInfo(const PieceInfo* pPieceInfo, const ImageInfo* pImageInfo)
{
	if (!pPieceInfo) return false;

	TM_PIECE_INFO::iterator itfound = m_PieceInfoMap.find(pPieceInfo->GetId());
	if (itfound == m_PieceInfoMap.end()) return false;
	SetModifiedFlag();

	PieceInfo* pFoundPieceInfo = itfound->second;
	pFoundPieceInfo->SetImageInfo(pImageInfo);
	return true;
}

const ImageInfo* ImagePieceDocument::AddImage(const wxString& strImageId, const wxString& strPath, wxBitmap* pImageBitmap)
{
	if (!pImageBitmap) return NULL;
	SetModifiedFlag();

	wxString strNewId = GenerateNewImageId(strImageId);
	ImageInfo* pImageInfo = new ImageInfo();
	pImageInfo->SetId(strNewId);
	pImageInfo->SetBitmap(pImageBitmap);
	pImageInfo->SetPath(strPath);
	m_ImageInfoMap.insert(std::make_pair(pImageInfo->GetId(), pImageInfo));

	m_bNeedUpdateImageIds = true;
	return pImageInfo;
}

bool ImagePieceDocument::RemoveImage(const wxString& strId)
{
	TM_IMAGE_INFO::iterator itfound = m_ImageInfoMap.find(strId);
	if (itfound == m_ImageInfoMap.end()) return false;
	SetModifiedFlag();

	ImageInfo* pImageInfo = (itfound->second);
	m_ImageInfoMap.erase(itfound);

	TV_PIECE_INFO vPieceInfo;
	EnumImagePieces(vPieceInfo, pImageInfo);
	for (TV_PIECE_INFO::iterator it = vPieceInfo.begin(); it != vPieceInfo.end(); ++it)
	{
		RemovePiece((*it)->GetId());
	}

	SAFE_DELETE(pImageInfo);
	return true;
}

const PieceInfo* ImagePieceDocument::AddPiece(const wxString& strId, const wxRect& rect, const ImageInfo* pImageInfo)
{
	if (!pImageInfo) return NULL;
	if (rect.width <= 0 || rect.height <= 0) return NULL;
	SetModifiedFlag();

	wxString strNewId = GenerateNewPieceId(strId);
	PieceInfo* pNewPieceInfo = new PieceInfo();
	pNewPieceInfo->SetId(strNewId);
	pNewPieceInfo->SetRect(rect);
	pNewPieceInfo->SetImageInfo(pImageInfo);
	m_PieceInfoMap.insert(std::make_pair(pNewPieceInfo->GetId(), pNewPieceInfo));

	m_bNeedUpdatePieceIds = true;
	return pNewPieceInfo;
}

bool ImagePieceDocument::RemovePiece(const wxString& strId)
{
	TM_PIECE_INFO::iterator itfound = m_PieceInfoMap.find(strId);
	if (itfound == m_PieceInfoMap.end()) return false;
	SetModifiedFlag();

	PieceInfo* pPieceInfo = (itfound->second);
	m_PieceInfoMap.erase(itfound);

	ImageInfo* pImageInfo = (ImageInfo*)pPieceInfo->GetImageInfo();
	if (pImageInfo)
	{
		pImageInfo->ClearBitmapArea(pPieceInfo->GetRect());
	}

	SAFE_DELETE(pPieceInfo);

	return true;
}

wxString ImagePieceDocument::GenerateNewImageId(const wxString& strId)
{
	wxString strNewId = strId;
	int index = 0;

	while (FindImageInfo(strNewId))
	{
		strNewId = wxString::Format(wxT("%s%d"), strId, index++);
	}

	return strNewId;
}

wxString ImagePieceDocument::GenerateNewPieceId(const wxString& strId)
{
	wxString strNewId = strId;
	int index = 0;

	while (FindPieceInfo(strNewId))
	{
		strNewId = wxString::Format(wxT("%s%d"), strId, index++);
	}

	return strNewId;
}

void ImagePieceDocument::UpdateImageArrayString()
{
	if (!m_bNeedUpdateImageIds) return;
	m_bNeedUpdateImageIds = false;

	int nIndex = 0;
	m_ImageIds.Clear();
	m_ImageIdsIndex.Clear();

	m_ImageIds.Add(wxT("null"));
	m_ImageIdsIndex.Add(nIndex++);

	for (TM_IMAGE_INFO::iterator it = m_ImageInfoMap.begin(); it != m_ImageInfoMap.end(); ++it)
	{
		m_ImageIds.Add(it->first);
		m_ImageIdsIndex.Add(nIndex++);
	}
}

void ImagePieceDocument::UpdatePieceArrayString()
{
	if (!m_bNeedUpdatePieceIds) return;
	m_bNeedUpdatePieceIds = false;

	int nIndex = 0;
	m_PieceIds.Clear();
	m_PieceIdsIndex.Clear();

	m_PieceIds.Add(wxT("null"));
	m_PieceIdsIndex.Add(nIndex++);

	for (TM_PIECE_INFO::iterator it = m_PieceInfoMap.begin(); it != m_PieceInfoMap.end(); ++it)
	{
		m_PieceIds.Add(it->first);
		m_PieceIdsIndex.Add(nIndex++);
	}
}
