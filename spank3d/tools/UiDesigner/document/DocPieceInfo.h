/*!
 * \file DocPieceInfo.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCPIECEINFO_H__
#define __DOCPIECEINFO_H__

#include "IDoc.h"
#include <wx/xml/xml.h>
#include <map>

class DocPieceInfo : public IDoc
{
public:
	typedef struct PIECE_INFO_tag
	{
		int x, y, width, height;
		wxString strId;
	} PIECE_INFO;

	typedef std::map<wxString, PIECE_INFO*> TM_PIECE_INFO;

public:
	DocPieceInfo();
	virtual ~DocPieceInfo();

	PIECE_INFO* FindPieceInfo(const wxString& strId);
	TM_PIECE_INFO& GetPieceInfoMap();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

private:
	PIECE_INFO* FromXml(wxXmlNode* pXmlPieceInfo);
	wxXmlNode* ToXml(const PIECE_INFO* pPieceInfo);

private:
	wxString m_strTexture;
	TM_PIECE_INFO m_PieceInfoMap;

};
#endif // __DOCPIECEINFO_H__
