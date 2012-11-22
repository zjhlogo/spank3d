/*!
 * \file DocNinePatchStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCNINEPATCHSTYLE_H__
#define __DOCNINEPATCHSTYLE_H__

#include "IDoc.h"

class DocNinePatchStyle : public IDoc
{
public:
	DocNinePatchStyle();
	virtual ~DocNinePatchStyle();

	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCNINEPATCHSTYLE_H__
