/*!
 * \file PieceInfo.h
 * \date 2-13-2012 19:33:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PIECEINFO_H__
#define __PIECEINFO_H__

#include <wx/wx.h>
#include <wx/treectrl.h>
#include "ImageInfo.h"

class PieceInfo
{
public:
	PieceInfo();
	~PieceInfo();

	bool LoadFromXml(wxXmlNode* pNodePiece);
	bool SaveToXml(wxXmlNode* pNodePieceList);

	void SetId(const wxString& strId);
	const wxString& GetId() const;

	void SetImageInfo(const ImageInfo* pImageInfo);
	const ImageInfo* GetImageInfo() const;

	void SetRect(const wxRect& rect);
	const wxRect& GetRect() const;

	void SetTreeItemId(const wxTreeItemId& itemId);
	const wxTreeItemId& GetTreeItemId() const;

private:
	wxString m_strId;
	const ImageInfo* m_pImageInfo;
	wxRect m_Rect;
	wxTreeItemId m_TreeItemId;

};
#endif // __PIECEINFO_H__
