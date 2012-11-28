/*!
 * \file IDoc.h
 * \date 11-22-2012 9:12:34
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IDOC_H__
#define __IDOC_H__

#include "wxDocEvent.h"
#include "../utils/XmlUtil.h"

class IDoc : public wxEvtHandler
{
public:
	IDoc();
	virtual ~IDoc();

	virtual bool Open(const wxString& strDir, const wxString& strFile);
	virtual bool Save(const wxString& strFile = wxEmptyString);
	virtual void Close();

	const wxString& GetFileName() const;

protected:
	virtual bool DoOpen(const wxString& strFile) = 0;
	virtual bool DoSave(const wxString& strFile) = 0;
	virtual void Reset() = 0;

protected:
	wxString m_strBaseDir;
	wxString m_strFileName;
	wxString m_strPath;

};
#endif // __IDOC_H__
