/*!
 * \file ImageInfo.h
 * \date 2-13-2012 19:34:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMAGEINFO_H__
#define __IMAGEINFO_H__

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/xml/xml.h>

class ImageInfo
{
public:
	ImageInfo(const wxString& strBaseDir);
	~ImageInfo();

	bool LoadFromXml(wxXmlNode* pNodeImage);
	bool SaveToXml(wxXmlNode* pNodeImageList);
	bool SaveImage();

	void SetId(const wxString& strId);
	const wxString& GetId() const;

	void SetFileName(const wxString& strFileName);
	const wxString& GetFileName() const;
	const wxString& GetBaseDir() const;

	bool SetBitmap(wxBitmap* pBitmap);
	const wxBitmap* GetBitmap();

	void SetTreeItemId(const wxTreeItemId& itemId);
	const wxTreeItemId& GetTreeItemId() const;

	bool ClearBitmapArea(const wxRect& rect);

private:
	bool LoadImageFromFile();

private:
	wxString m_strId;
	wxString m_strFileName;
	wxBitmap* m_pBitmap;
	bool m_bLoaded;
	bool m_bIsModified;
	wxTreeItemId m_TreeItemId;

	wxString m_strBaseDir;
};
#endif // __IMAGEINFO_H__
