/*!
 * \file ImageListTransformer.h
 * \date 2-14-2012 1:02:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMAGELISTTRANSFORMER_H__
#define __IMAGELISTTRANSFORMER_H__

#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include "../document/ImageInfo.h"

class ImageListTransformer
{
public:
	static ImageListTransformer& GetInstance();

	bool Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid);
	void UpdateListView();
	void UpdateProperty(const ImageInfo* pImageInfo);
	void PropertyChanged(wxPGProperty* pProperty);

	void SetSelectedImageInfo(const ImageInfo* pImageInfo);
	const ImageInfo* GetSelectedImageInfo();

protected:
	ImageListTransformer();
	~ImageListTransformer();

private:
	wxTreeCtrl* m_pListView;
	wxPropertyGrid* m_pPropertyGrid;
	bool m_bSkipUpdateProperty;

};
#endif // __IMAGELISTTRANSFORMER_H__
