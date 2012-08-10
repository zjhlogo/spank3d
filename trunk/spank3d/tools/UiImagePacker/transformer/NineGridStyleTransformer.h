/*!
 * \file NineGridStyleTransformer.h
 * \date 2-14-2012 16:08:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __NINEGRIDSTYLETRANSFORMER_H__
#define __NINEGRIDSTYLETRANSFORMER_H__

#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include "../document/NineGridStyle.h"

class NineGridStyleTransformer
{
public:
	static NineGridStyleTransformer& GetInstance();

	bool Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid);
	void UpdateListView();
	void UpdateProperty(const NineGridStyle* pNineGrieStyle);
	void PropertyChanged(wxPGProperty* pProperty);

	void SetSelectedNineGridStyle(const NineGridStyle* pNineGrieStyle);
	const NineGridStyle* GetSelectedNineGridStyle();

protected:
	NineGridStyleTransformer();
	~NineGridStyleTransformer();

private:
	wxTreeCtrl* m_pListView;
	wxPropertyGrid* m_pPropertyGrid;
	bool m_bSkipUpdateProperty;

};
#endif // __NINEGRIDSTYLETRANSFORMER_H__
