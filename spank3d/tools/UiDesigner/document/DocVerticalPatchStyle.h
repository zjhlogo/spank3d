/*!
 * \file DocVerticalPatchStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCVERTICALPATCHSTYLE_H__
#define __DOCVERTICALPATCHSTYLE_H__

#include "IDoc.h"

class DocVerticalPatchStyle : public IDoc
{
public:
	DocVerticalPatchStyle();
	virtual ~DocVerticalPatchStyle();

	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCVERTICALPATCHSTYLE_H__
