/*!
 * \file ProjectDocument.h
 * \date 2-14-2012 13:21:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PROJECTDOCUMENT_H__
#define __PROJECTDOCUMENT_H__

#include "DocumentBase.h"

class ProjectDocument: public DocumentBase
{
public:
	virtual ~ProjectDocument();

	static ProjectDocument& GetInstance();

	virtual bool OpenFile(const wxString& strFile);
	virtual bool SaveFile(const wxString& strFile);
	virtual void Reset();

	const wxString& GetRootDir() const;
	wxString GetRootPath() const;

	void SetProjectDir(const wxString& strProjectDir);
	const wxString& GetProjectDir() const;

	bool SetImagePieceFilePath(const wxString& strPath);
	const wxString& GetImagePieceFilePath() const;

	bool SetBitmapStyleFilePath(const wxString& strPath);
	const wxString& GetBitmapStyleFilePath() const;

	bool SetNineGridStyleFilePath(const wxString& strPath);
	const wxString& GetNineGridStyleFilePath() const;

	bool SetColorStyleFilePath(const wxString& strPath);
	const wxString& GetColorStyleFilePath() const;

	bool SetClipBitmapStyleFilePath(const wxString& strPath);
	const wxString& GetClipBitmapStyleFilePath() const;

protected:
	ProjectDocument();

private:
	wxString m_strRootDir;
	wxString m_strProjectDir;

	wxString m_strImagePieceFilePath;
	wxString m_strBitmapStyleFilePath;
	wxString m_strNineGridStyleFilePath;
	wxString m_strColorStyleFilePath;
	wxString m_strClipBitmapStyleFilePath;
};

#endif // __PROJECTDOCUMENT_H__
