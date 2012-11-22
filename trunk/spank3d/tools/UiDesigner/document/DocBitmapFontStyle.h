/*!
 * \file DocBitmapFontStyle.h
 * \date 11-22-2012 9:29:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCBITMAPFONTSTYLE_H__
#define __DOCBITMAPFONTSTYLE_H__

#include "IDoc.h"

class DocBitmapFontStyle : public IDoc
{
public:
	DocBitmapFontStyle();
	virtual ~DocBitmapFontStyle();

	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCBITMAPFONTSTYLE_H__
