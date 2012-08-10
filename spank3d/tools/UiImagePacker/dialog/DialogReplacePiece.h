/*!
 * \file DialogReplacePiece.h
 * \date 2-21-2012 13:50:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DIALOGREPLACEPIECE_H__
#define __DIALOGREPLACEPIECE_H__

#include <wx/wx.h>
#include "../document/ImageInfo.h"
#include "../document/PieceInfo.h"
#include "../utils/RectangleBinPack.h"
#include <vector>

class DialogReplacePiece: public wxDialog
{
	DECLARE_DYNAMIC_CLASS(DialogReplacePiece)
	DECLARE_EVENT_TABLE()

public:
	enum CONST_DEFINE
	{
		ID_REPLACEPIECE = 10010,
		ID_BUTTON = 10002,
		ID_BUTTON1 = 10003,
		ID_LISTBOX = 10001,
		ID_TEXTCTRL3 = 10011,
		ID_TEXTCTRL1 = 10008,
		ID_TEXTCTRL2 = 10009
	};

	typedef struct PACKING_PIECE_INFO_tag
	{
		wxString strId;
		wxString strPath;
		wxBitmap subBitmap;
		wxSize bmpSize;
		RectangleBinPack::Node* pNode;
		const PieceInfo* pPieceInfo;
	} PACKING_PIECE_INFO;
	typedef std::vector<PACKING_PIECE_INFO*> TV_PACKING_PIECE_INFO;

public:
	DialogReplacePiece();
	DialogReplacePiece(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~DialogReplacePiece();

	virtual bool Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

	void SetDefaultImageInfo(const ImageInfo* pImageInfo);
	const ImageInfo* GetCurrImageInfo();

private:
	void Init();
	void CreateControls();

	void OnBtnAddPieceClicked(wxCommandEvent& event);
	void OnBtnRemovePieceClicked(wxCommandEvent& event);
	void OnLstPiecesSelected(wxCommandEvent& event);
	void OnOkClicked(wxCommandEvent& event);
	void OnCancelClicked(wxCommandEvent& event);

	void InitImageInfo(const ImageInfo* pImageInfo);
	void AddListItems(const wxArrayString& arrayString);
	void RemoveListSelectedItems();

	bool ReplacePieceFromExistingImage(const wxSize& newSize);
	void FreePackingPiecesInfo(TV_PACKING_PIECE_INFO& vPackingInfo);

	wxBitmap* PackImage(const wxSize& bmpSize, const TV_PACKING_PIECE_INFO& vPackingInfo);
	bool GeneratePackingInfo(TV_PACKING_PIECE_INFO& vPackingInfo, const wxSize& bmpSize);
	bool GetPieceFromList(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo);
	bool GetRestPieceFromImage(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo);
	bool FindPackingInfo(const TV_PACKING_PIECE_INFO& vPackingInfo, const PieceInfo* pPieceInfo);

private:
	wxButton* m_BtnAddPiece;
	wxButton* m_BtnRemovePiece;
	wxListBox* m_LstPieces;
	wxTextCtrl* m_EdtImageId;
	wxTextCtrl* m_EdtImageWidth;
	wxTextCtrl* m_EdtImageHeight;

	wxString m_strError;
	RectangleBinPack m_Packer;
	const ImageInfo* m_pCurrImageInfo;

};
#endif // __DIALOGREPLACEPIECE_H__
