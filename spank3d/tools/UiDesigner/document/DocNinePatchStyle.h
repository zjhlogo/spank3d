/*!
 * \file DocNinePatchStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCNINEPATCHSTYLE_H__
#define __DOCNINEPATCHSTYLE_H__

#include "DocGraphicsStyle.h"
#include "DocPieceInfo.h"
#include <vector>
#include <map>

class DocNinePatchStyle : public DocGraphicsStyle
{
public:
	typedef struct NINE_PATCH_STATE_INFO_tag
	{
		unsigned int state;
		const DocPieceInfo::PIECE_INFO* pPieceInfo;
		int minX, minY, maxX, maxY;
	} NINE_PATCH_STATE_INFO;

	typedef std::vector<NINE_PATCH_STATE_INFO> TV_NINE_PATCH_STATE_INFO;

	typedef struct NINE_PATCH_STYLE_tag : public STYLE_INFO
	{
		TV_NINE_PATCH_STATE_INFO vNinePatchState;
	} NINE_PATCH_STYLE;

	typedef std::map<wxString, NINE_PATCH_STYLE*> TM_NINE_PATCH_STYLE;

public:
	DocNinePatchStyle();
	virtual ~DocNinePatchStyle();

	NINE_PATCH_STYLE* FindNinePatchStyle(const wxString& strId);
	TM_NINE_PATCH_STYLE& GetNinePatchStyleMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	NINE_PATCH_STYLE* FromXml(wxXmlNode* pXmlNinePatchStyle);
	wxXmlNode* ToXml(const NINE_PATCH_STYLE* pNinePatchStyle);

private:
	TM_NINE_PATCH_STYLE m_NinePatchStyleMap;

};
#endif // __DOCNINEPATCHSTYLE_H__
