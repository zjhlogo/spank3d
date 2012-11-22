/*!
 * \file DocBitmapFont.h
 * \date 11-22-2012 9:35:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DOCBITMAPFONT_H__
#define __DOCBITMAPFONT_H__

#include "IDoc.h"

class DocBitmapFont: public IDoc
{
public:
	DocBitmapFont();
	virtual ~DocBitmapFont();

protected:
	virtual bool DoOpen(const wxString& strFile);
	virtual bool DoSave(const wxString& strFile);
	virtual void Reset();

};
#endif // __DOCBITMAPFONT_H__
