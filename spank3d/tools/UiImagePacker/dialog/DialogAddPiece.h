/*!
 * \file DialogAddPiece.h
 * \date 2-16-2012 9:08:13
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DIALOGADDPIECE_H__
#define __DIALOGADDPIECE_H__

#include <wx/wx.h>
#include "../document/ImageInfo.h"
#include "../document/PieceInfo.h"
#include "../utils/RectangleBinPack.h"
#include <vector>

class DialogAddPiece: public wxDialog
{
	DECLARE_DYNAMIC_CLASS(DialogAddPiece)
	DECLARE_EVENT_TABLE()

public:
	enum CONST_DEFINE
	{
		ID_ADDPIECE = 10000,
		ID_BUTTON = 10002,
		ID_BUTTON1 = 10003,
		ID_LISTBOX = 10001,
		ID_RADIOBUTTON = 10004,
		ID_CHOICE = 10006,
		ID_RADIOBUTTON1 = 10005,
		ID_TEXTCTRL = 10007,
		ID_TEXTCTRL1 = 10008,
		ID_TEXTCTRL2 = 10009
	};

	typedef struct PACKING_PIECE_INFO_tag
	{
		wxString strId;
		wxBitmap subBitmap;
		wxSize bmpSize;
		RectangleBinPack::Node* pNode;
		const PieceInfo* pPieceInfo;
	} PACKING_PIECE_INFO;
	typedef std::vector<PACKING_PIECE_INFO*> TV_PACKING_PIECE_INFO;

public:
	DialogAddPiece();
	DialogAddPiece(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~DialogAddPiece();

	virtual bool Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

	void SetDefaultImageInfo(const ImageInfo* pImageInfo);
	const ImageInfo* GetCurrImageInfo();

	bool RepackImagePiece(const ImageInfo* pImageInfo);

private:
	void Init();
	void CreateControls();
	void InitializeData();

	void OnBtnAddPieceClicked(wxCommandEvent& event);
	void OnBtnRemovePieceClicked(wxCommandEvent& event);
	void OnLstPiecesSelected(wxCommandEvent& event);
	void OnRadAddImageSelected(wxCommandEvent& event);
	void OnChoImageSelected(wxCommandEvent& event);
	void OnOkClicked(wxCommandEvent& event);
	void OnCancelClicked(wxCommandEvent& event);

	void InitImageInfo(const ImageInfo* pImageInfo);
	void AddListItems(const wxArrayString& arrayString);
	void RemoveListSelectedItems();

	bool AddPieceToExistingImage(const wxSize& newSize);
	bool AddPieceIntoNewImage(const wxSize& newSize);
	void FreePackingPiecesInfo(TV_PACKING_PIECE_INFO& vPackingInfo);

	wxBitmap* PackImage(const wxSize& bmpSize, const TV_PACKING_PIECE_INFO& vPackingInfo);
	bool GeneratePackingInfo(TV_PACKING_PIECE_INFO& vPackingInfo, const wxSize& bmpSize);
	bool GetPieceFromList(TV_PACKING_PIECE_INFO& vPackingInfo);
	bool GetPieceFromImage(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo);

private:
	wxButton* m_BtnAddPiece;
	wxButton* m_BtnRemovePiece;
	wxListBox* m_LstPieces;
	wxRadioButton* m_RadAddExisting;
	wxChoice* m_ChoImage;
	wxRadioButton* m_RadCreateNew;
	wxTextCtrl* m_EdtImageName;
	wxTextCtrl* m_EdtImageWidth;
	wxTextCtrl* m_EdtImageHeight;

	wxString m_strError;
	RectangleBinPack m_Packer;
	const ImageInfo* m_pCurrImageInfo;

};

#endif // __DIALOGADDPIECE_H__
