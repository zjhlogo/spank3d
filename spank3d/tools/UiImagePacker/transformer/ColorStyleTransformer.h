/*!
 * \file ColorStyleTransformer.h
 * \date 2-14-2012 17:00:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __COLORSTYLETRANSFORMER_H__
#define __COLORSTYLETRANSFORMER_H__

#include <wx/propgrid/propgrid.h>
#include "../document/ColorStyle.h"

class ColorStyleTransformer
{
public:
	static ColorStyleTransformer& GetInstance();

	bool Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid);
	void UpdateListView();
	void UpdateProperty(const ColorStyle* pColorStyle);
	void PropertyChanged(const wxPGProperty* pProperty);

	void SetSelectedColorStyle(const ColorStyle* pColorStyle);
	const ColorStyle* GetSelectedColorStyle();

protected:
	ColorStyleTransformer();
	~ColorStyleTransformer();

private:
	wxTreeCtrl* m_pListView;
	wxPropertyGrid* m_pPropertyGrid;
	bool m_bSkipUpdateProperty;

};
#endif // __COLORSTYLETRANSFORMER_H__
