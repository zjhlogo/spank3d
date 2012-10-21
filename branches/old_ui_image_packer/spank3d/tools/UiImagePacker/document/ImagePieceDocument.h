/*!
 * \file ImagePieceDocument.h
 * \date 2-14-2012 1:23:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMAGEPIECEDOCUMENT_H__
#define __IMAGEPIECEDOCUMENT_H__

#include "DocumentBase.h"
#include <wx/gdicmn.h>
#include <map>
#include <vector>
#include "ImageInfo.h"
#include "PieceInfo.h"

class ImagePieceDocument : public DocumentBase
{
public:
	typedef std::map<wxString, ImageInfo*> TM_IMAGE_INFO;
	typedef std::map<wxString, PieceInfo*> TM_PIECE_INFO;
	typedef std::vector<const PieceInfo*> TV_PIECE_INFO;

public:
	virtual ~ImagePieceDocument();

	virtual bool OpenFile(const wxString& strFile);
	virtual bool SaveFile(const wxString& strFile);
	virtual void Reset();

	static ImagePieceDocument& GetInstance();

	const ImageInfo* FindImageInfo(const wxString& strId);
	const PieceInfo* FindPieceInfo(const wxString& strId);
	const PieceInfo* FindPieceInfoUnderPoint(const wxPoint& pos, const ImageInfo* pImageInfo);
	int EnumImagePieces(TV_PIECE_INFO& vPieceOut, const ImageInfo* pImageInfo);

	const TM_IMAGE_INFO& GetImageInfoMap();
	const TM_PIECE_INFO& GetPieceInfoMap();

	const wxArrayString& GetImageIds();
	const wxArrayInt& GetImageIdsIndex();
	int FindImageIndex(const wxString& strId);
	const wxString& GetImageId(int index);

	const wxArrayString& GetPieceIds();
	const wxArrayInt& GetPieceIdsIndex();
	int FindPieceIndex(const PieceInfo* pPieceInfo);
	int FindPieceIndex(const wxString& strId);
	const wxString& GetPieceId(int index);

	bool RenameImageInfoId(const ImageInfo* pImageInfo, const wxString& strNewId);
	bool RenamePieceInfoId(const PieceInfo* pPieceInfo, const wxString& strNewId);

	bool SetImageBitmap(const ImageInfo* pImageInfo, wxBitmap* pNewBitmap);
	bool ClearImageBitmapArea(const ImageInfo* pImageInfo, const wxRect& rect);
	bool SetPieceRect(const PieceInfo* pPieceInfo, const wxRect& rect);
	bool SetPieceImageInfo(const PieceInfo* pPieceInfo, const ImageInfo* pImageInfo);

	const ImageInfo* AddImage(const wxString& strImageId, const wxString& strPath, wxBitmap* pImageBitmap);
	bool RemoveImage(const wxString& strId);

	const PieceInfo* AddPiece(const wxString& strId, const wxRect& rect, const ImageInfo* pImageInfo);
	bool RemovePiece(const wxString& strId);

protected:
	ImagePieceDocument();

	wxString GenerateNewImageId(const wxString& strId);
	wxString GenerateNewPieceId(const wxString& strId);

	void UpdateImageArrayString();
	void UpdatePieceArrayString();

private:
	TM_IMAGE_INFO m_ImageInfoMap;
	TM_PIECE_INFO m_PieceInfoMap;

	wxArrayString m_ImageIds;
	wxArrayInt m_ImageIdsIndex;

	wxArrayString m_PieceIds;
	wxArrayInt m_PieceIdsIndex;

	bool m_bNeedUpdateImageIds;
	bool m_bNeedUpdatePieceIds;

};
#endif // __IMAGEPIECEDOCUMENT_H__
