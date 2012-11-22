/*!
 * \file DocHorizontalPatchStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCHORIZONTALPATCHSTYLE_H__
#define __DOCHORIZONTALPATCHSTYLE_H__

#include "IDoc.h"

class DocHorizontalPatchStyle : public IDoc
{
public:
	DocHorizontalPatchStyle();
	virtual ~DocHorizontalPatchStyle();

	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCHORIZONTALPATCHSTYLE_H__
