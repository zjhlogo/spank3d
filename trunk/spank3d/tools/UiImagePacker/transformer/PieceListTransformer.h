/*!
 * \file PieceListTransformer.h
 * \date 2-14-2012 1:25:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PIECELISTTRANSFORMER_H__
#define __PIECELISTTRANSFORMER_H__

#include <wx/propgrid/propgrid.h>
#include "../document/PieceInfo.h"

class PieceListTransformer
{
public:
	static PieceListTransformer& GetInstance();

	bool Initialize(wxTreeCtrl* pTreeCtrl, wxPropertyGrid* pPropertyGrid);
	void UpdateListView();
	void UpdateProperty(const PieceInfo* pPieceInfo);
	void PropertyChanged(const wxPGProperty* pProperty);

	void SetSelectedPieceInfo(const PieceInfo* pPieceInfo);
	const PieceInfo* GetSelectedPieceInfo();

protected:
	PieceListTransformer();
	~PieceListTransformer();

private:
	wxTreeCtrl* m_pListView;
	wxPropertyGrid* m_pPropertyGrid;
	bool m_bSkipUpdateProperty;

};
#endif // __PIECELISTTRANSFORMER_H__
