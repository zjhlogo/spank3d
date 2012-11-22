/*!
 * \file DocBitmapStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCBITMAPSTYLE_H__
#define __DOCBITMAPSTYLE_H__

#include "IDoc.h"

class DocBitmapStyle : public IDoc
{
public:
	DocBitmapStyle();
	virtual ~DocBitmapStyle();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCBITMAPSTYLE_H__
