/*!
 * \file DocBitmapStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCBITMAPSTYLE_H__
#define __DOCBITMAPSTYLE_H__

#include "DocGraphicsStyle.h"
#include "DocPieceInfo.h"
#include <vector>
#include <map>

class DocBitmapStyle : public DocGraphicsStyle
{
public:
	typedef struct BITMAP_STATE_INFO_tag
	{
		unsigned int state;
		const DocPieceInfo::PIECE_INFO* pPieceInfo;
	} BITMAP_STATE_INFO;

	typedef std::vector<BITMAP_STATE_INFO> TV_BITMAP_STATE_INFO;

	typedef struct BITMAP_STYLE_tag : public STYLE_INFO
	{
		TV_BITMAP_STATE_INFO vBitmapState;
	} BITMAP_STYLE;

	typedef std::map<wxString, BITMAP_STYLE*> TM_BITMAP_STYLE;

public:
	DocBitmapStyle();
	virtual ~DocBitmapStyle();

	BITMAP_STYLE* FindBitmapStyle(const wxString& strId);
	TM_BITMAP_STYLE& GetBitmapStyleMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	BITMAP_STYLE* FromXml(wxXmlNode* pXmlBitmapStyle);
	wxXmlNode* ToXml(const BITMAP_STYLE* pBitmapStyle);

private:
	TM_BITMAP_STYLE m_BitmapStyleMap;

};
#endif // __DOCBITMAPSTYLE_H__
