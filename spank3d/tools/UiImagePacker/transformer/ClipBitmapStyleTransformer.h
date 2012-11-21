/*!
 * \file ClipBitmapStyleTransformer.h
 * \date 2-14-2012 17:04:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __CLIPBITMAPSTYLETRANSFORMER_H__
#define __CLIPBITMAPSTYLETRANSFORMER_H__

#include <wx/propgrid/propgrid.h>
#include "../document/ClipBitmapStyle.h"

class ClipBitmapStyleTransformer
{
public:
	static ClipBitmapStyleTransformer& GetInstance();

	bool Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid);
	void UpdateListView();
	void UpdateProperty(const ClipBitmapStyle* pClipBitmapStyle);
	void PropertyChanged(const wxPGProperty* pProperty);

	void SetSelectedClipBitmapStyle(const ClipBitmapStyle* pClipBitmapStyle);
	const ClipBitmapStyle* GetSelectedClipBitmapStyle();

protected:
	ClipBitmapStyleTransformer();
	~ClipBitmapStyleTransformer();

private:
	wxTreeCtrl* m_pListView;
	wxPropertyGrid* m_pPropertyGrid;
	bool m_bSkipUpdateProperty;

};
#endif // __CLIPBITMAPSTYLETRANSFORMER_H__
