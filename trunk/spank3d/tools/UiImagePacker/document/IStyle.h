/*!
 * \file IStyle.h
 * \date 2-14-2012 13:59:11
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __ISTYLE_H__
#define __ISTYLE_H__

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/xml/xml.h>

class IStyle
{
public:
	enum STYLE_STATE
	{
		SS_NORMAL = 0,
		SS_DOWN,
		SS_HOVER,
		SS_DISABLED,
		SS_NUM
	};

public:
	IStyle();
	~IStyle();

	void SetId(const wxString& strId);
	const wxString& GetId() const;

	void SetAutoGenBitmap(bool bAutoGenBitmap);
	bool IsAutoGenBitmap() const;

	void SetFlipX(bool bFlipX);
	bool IsFlipX() const;

	void SetFlipY(bool bFlipY);
	bool IsFlipY() const;

	void SetTreeItemId(wxTreeItemId itemId);
	const wxTreeItemId& GetTreeItemId() const;

protected:
	wxXmlNode* FindXmlChild(wxXmlNode* pParent, const wxString& name);

private:
	wxString m_strId;
	wxTreeItemId m_TreeItemId;
	bool m_AutoGenBitmap;
	bool m_FlipX;
	bool m_FlipY;

};

#endif // __ISTYLE_H__
