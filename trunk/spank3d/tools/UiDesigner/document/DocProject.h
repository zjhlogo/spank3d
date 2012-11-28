/*!
 * \file DocProject.h
 * \date 11-22-2012 9:13:58
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCPROJECT_H__
#define __DOCPROJECT_H__

#include "IDoc.h"
#include "DocPieceInfo.h"
#include "DocBitmapFont.h"
#include "DocBitmapStyle.h"
#include "DocNinePatchStyle.h"
#include "DocVerticalPatchStyle.h"
#include "DocHorizontalPatchStyle.h"
#include "DocBitmapFontStyle.h"

#include <map>

class DocProject : public IDoc
{
public:
	typedef std::map<wxString, DocPieceInfo*> TM_DOC_PIECE_INFO;
	typedef std::map<wxString, DocBitmapFont*> TM_DOC_BITMAP_FONT;
	typedef std::map<wxString, DocBitmapStyle*> TM_DOC_BITMAP_STYLE;
	typedef std::map<wxString, DocNinePatchStyle*> TM_DOC_NINE_PATCH_STYLE;
	typedef std::map<wxString, DocVerticalPatchStyle*> TM_DOC_VERTICAL_PATCH_STYLE;
	typedef std::map<wxString, DocHorizontalPatchStyle*> TM_DOC_HORIZONTAL_PATCH_STYLE;
	typedef std::map<wxString, DocBitmapFontStyle*> TM_DOC_BITMAP_FONT_STYLE;

public:
	DocProject();
	virtual ~DocProject();

	static DocProject& GetInstance();

	DocPieceInfo::PIECE_INFO* FindPieceInfo(const wxString& strId);

	TM_DOC_PIECE_INFO& GetPieceInfoMap();
	TM_DOC_BITMAP_FONT& GetBitmapFontMap();
	TM_DOC_BITMAP_STYLE& GetBitmapStyleMap();
	TM_DOC_NINE_PATCH_STYLE& GetNinePatchStyleMap();
	TM_DOC_VERTICAL_PATCH_STYLE& GetVerticalPatchStyleMap();
	TM_DOC_HORIZONTAL_PATCH_STYLE& GetHorizontalPatchStyleMap();
	TM_DOC_BITMAP_FONT_STYLE& GetBitmapFontStyleMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	wxString m_strRootDir;

	TM_DOC_PIECE_INFO m_pieceInfoMap;
	TM_DOC_BITMAP_FONT m_bitmapFontMap;
	TM_DOC_BITMAP_STYLE m_bitmapStyleMap;
	TM_DOC_NINE_PATCH_STYLE m_ninePatchStyleMap;
	TM_DOC_VERTICAL_PATCH_STYLE m_verticalPatchStyleMap;
	TM_DOC_HORIZONTAL_PATCH_STYLE m_horizontalPatchStyleMap;
	TM_DOC_BITMAP_FONT_STYLE m_bitmapFontStyleMap;

};
#endif // __DOCPROJECT_H__
