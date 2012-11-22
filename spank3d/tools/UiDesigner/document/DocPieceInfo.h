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

class DocPieceInfo : public IDoc
{
public:
	DocPieceInfo();
	virtual ~DocPieceInfo();

	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCPIECEINFO_H__
