/*!
 * \file ClipBitmapStyleDocument.h
 * \date 2-14-2012 16:47:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __CLIPBITMAPSTYLEDOCUMENT_H__
#define __CLIPBITMAPSTYLEDOCUMENT_H__

#include "DocumentBase.h"
#include "ClipBitmapStyle.h"
#include <map>
#include <vector>

class ClipBitmapStyleDocument : public DocumentBase
{
public:
	typedef std::map<wxString, ClipBitmapStyle*> TM_CLIP_BITMAP_STYLE;
	typedef std::vector<const ClipBitmapStyle*> TV_CLIP_BITMAP_STYLE;

public:
	virtual ~ClipBitmapStyleDocument();
	static ClipBitmapStyleDocument& GetInstance();

	virtual bool OpenFile(const wxString& strFile);
	virtual bool SaveFile(const wxString& strFile);
	virtual void Reset();

	const ClipBitmapStyle* FindClipBitmapStyle(const wxString& strId);
	const TM_CLIP_BITMAP_STYLE& GetClipBitmapStyleMap();
	int EnumClipBitmapStyles(TV_CLIP_BITMAP_STYLE& vClipBitmapStyleOut, const PieceInfo* pPieceInfo);

	bool RenameClipBitmapStyleId(const ClipBitmapStyle* pClipBitmapStyle, const wxString& strNewId);
	bool SetAutoGenBitmap(const ClipBitmapStyle* pClipBitmapStyle, bool bAutoGenBitmap);
	bool SetFlipX(const ClipBitmapStyle* pClipBitmapStyle, bool bFlipX);
	bool SetFlipY(const ClipBitmapStyle* pClipBitmapStyle, bool bFlipY);
	bool SetStatePiece(const ClipBitmapStyle* pClipBitmapStyle, const PieceInfo* pPieceInfo, IStyle::STYLE_STATE eState);

	const ClipBitmapStyle* AddClipBitmapStyle(const wxString& strId);
	bool RemoveClipBitmapStyle(const wxString& strId);

protected:
	ClipBitmapStyleDocument();
	ClipBitmapStyle* InternalFindClipBitmapStyle(const wxString& strId);
	wxString GenerateNewClipBitmapStyleId(const wxString& strId);

private:
	TM_CLIP_BITMAP_STYLE m_ClipBitmapStyleMap;

};
#endif // __CLIPBITMAPSTYLEDOCUMENT_H__
