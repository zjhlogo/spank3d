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
	typedef std::map<wxString, DocPieceInfo*> TM_PIECE_INFO;
	typedef std::map<wxString, DocBitmapFont*> TM_BITMAP_FONT;
	typedef std::map<wxString, DocBitmapStyle*> TM_BITMAP_STYLE;
	typedef std::map<wxString, DocNinePatchStyle*> TM_NINE_PATCH_STYLE;
	typedef std::map<wxString, DocVerticalPatchStyle*> TM_VERTICAL_PATCH_STYLE;
	typedef std::map<wxString, DocHorizontalPatchStyle*> TM_HORIZONTAL_PATCH_STYLE;
	typedef std::map<wxString, DocBitmapFontStyle*> TM_BITMAP_FONT_STYLE;

public:
	DocProject();
	virtual ~DocProject();

	static DocProject& GetInstance();

	const TM_PIECE_INFO& GetPieceInfoMap() const;
	const TM_BITMAP_FONT& GetBitmapFontMap() const;
	const TM_BITMAP_STYLE& GetBitmapStyleMap() const;
	const TM_NINE_PATCH_STYLE& GetNinePatchStyleMap() const;
	const TM_VERTICAL_PATCH_STYLE& GetVerticalPatchStyleMap() const;
	const TM_HORIZONTAL_PATCH_STYLE& GetHorizontalPatchStyleMap() const;
	const TM_BITMAP_FONT_STYLE& GetBitmapFontStyleMap() const;

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	wxString m_strRootDir;

	TM_PIECE_INFO m_pieceInfoMap;
	TM_BITMAP_FONT m_bitmapFontMap;
	TM_BITMAP_STYLE m_bitmapStyleMap;
	TM_NINE_PATCH_STYLE m_ninePatchStyleMap;
	TM_VERTICAL_PATCH_STYLE m_verticalPatchStyleMap;
	TM_HORIZONTAL_PATCH_STYLE m_horizontalPatchStyleMap;
	TM_BITMAP_FONT_STYLE m_bitmapFontStyleMap;

};
#endif // __DOCPROJECT_H__
