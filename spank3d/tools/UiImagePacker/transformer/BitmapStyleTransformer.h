/*!
 * \file BitmapStyleTransformer.h
 * \date 2-14-2012 14:46:14
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BITMAPSTYLETRANSFORMER_H__
#define __BITMAPSTYLETRANSFORMER_H__

#include <wx/propgrid/propgrid.h>
#include "../document/BitmapStyle.h"

class BitmapStyleTransformer
{
public:
	static BitmapStyleTransformer& GetInstance();

	bool Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid);
	void UpdateListView();
	void UpdateProperty(const BitmapStyle* pBitmapStyle);
	void PropertyChanged(wxPGProperty* pProperty);

	void SetSelectedBitmapStyle(const BitmapStyle* pBitmapStyle);
	const BitmapStyle* GetSelectedBitmapStyle();

protected:
	BitmapStyleTransformer();
	~BitmapStyleTransformer();

private:
	wxTreeCtrl* m_pListView;
	wxPropertyGrid* m_pPropertyGrid;
	bool m_bSkipUpdateProperty;

};
#endif // __BITMAPSTYLETRANSFORMER_H__
