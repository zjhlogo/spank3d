/*!
 * \file DocHorizontalPatchStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCHORIZONTALPATCHSTYLE_H__
#define __DOCHORIZONTALPATCHSTYLE_H__

#include "DocGraphicsStyle.h"
#include "DocPieceInfo.h"
#include <vector>
#include <map>

class DocHorizontalPatchStyle : public DocGraphicsStyle
{
public:
	typedef struct HORIZONTAL_PATCH_STATE_INFO_tag
	{
		unsigned int state;
		const DocPieceInfo::PIECE_INFO* pPieceInfo;
		int minX, maxX;
	} HORIZONTAL_PATCH_STATE_INFO;

	typedef std::vector<HORIZONTAL_PATCH_STATE_INFO> TV_HORIZONTAL_PATCH_STATE_INFO;

	typedef struct HORIZONTAL_PATCH_STYLE_tag : public STYLE_INFO
	{
		TV_HORIZONTAL_PATCH_STATE_INFO vHorizontalPatchState;
	} HORIZONTAL_PATCH_STYLE;

	typedef std::map<wxString, HORIZONTAL_PATCH_STYLE*> TM_HORIZONTAL_PATCH_STYLE;

public:
	DocHorizontalPatchStyle();
	virtual ~DocHorizontalPatchStyle();

	HORIZONTAL_PATCH_STYLE* FindHorizontalPatchStyle(const wxString& strId);
	TM_HORIZONTAL_PATCH_STYLE& GetHorizontalPatchStyleMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	HORIZONTAL_PATCH_STYLE* FromXml(wxXmlNode* pXmlHorizontalPatchStyle);
	wxXmlNode* ToXml(const HORIZONTAL_PATCH_STYLE* pHorizontalPatchStyle);

private:
	TM_HORIZONTAL_PATCH_STYLE m_HorizontalPatchStyleMap;

};
#endif // __DOCHORIZONTALPATCHSTYLE_H__
