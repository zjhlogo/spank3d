/*!
 * \file DocVerticalPatchStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCVERTICALPATCHSTYLE_H__
#define __DOCVERTICALPATCHSTYLE_H__

#include "DocGraphicsStyle.h"
#include "DocPieceInfo.h"
#include <vector>
#include <map>

class DocVerticalPatchStyle : public DocGraphicsStyle
{
public:
	typedef struct VERTICAL_PATCH_STATE_INFO_tag
	{
		unsigned int state;
		const DocPieceInfo::PIECE_INFO* pPieceInfo;
		int minY, maxY;
	} VERTICAL_PATCH_STATE_INFO;

	typedef std::vector<VERTICAL_PATCH_STATE_INFO> TV_VERTICAL_PATCH_STATE_INFO;

	typedef struct VERTICAL_PATCH_STYLE_tag : public STYLE_INFO
	{
		TV_VERTICAL_PATCH_STATE_INFO vVerticalPatchState;
	} VERTICAL_PATCH_STYLE;

	typedef std::map<wxString, VERTICAL_PATCH_STYLE*> TM_VERTICAL_PATCH_STYLE;

public:
	DocVerticalPatchStyle();
	virtual ~DocVerticalPatchStyle();

	VERTICAL_PATCH_STYLE* FindVerticalPatchStyle(const wxString& strId);
	TM_VERTICAL_PATCH_STYLE& GetVerticalPatchStyleMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	VERTICAL_PATCH_STYLE* FromXml(wxXmlNode* pXmlVerticalPatchStyle);
	wxXmlNode* ToXml(const VERTICAL_PATCH_STYLE* pNorizontalPatchStyle);

private:
	TM_VERTICAL_PATCH_STYLE m_VerticalPatchStyleMap;

};
#endif // __DOCVERTICALPATCHSTYLE_H__
