/*!
 * \file DialogMovePiece.h
 * \date 3-22-2012 17:25:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __DIALOGMOVEPIECE_H__
#define __DIALOGMOVEPIECE_H__

#include <wx/wx.h>
#include "../document/ImageInfo.h"
#include "../document/PieceInfo.h"
#include "../utils/RectangleBinPack.h"
#include "../editor/ImagePieceEditor.h"

#include <vector>

class DialogMovePiece: public wxDialog
{
	DECLARE_DYNAMIC_CLASS(DialogMovePiece)
	DECLARE_EVENT_TABLE()

public:
	enum CONST_DEFINE
	{
		ID_MOVEPIECE = 10000,
		ID_LISTBOX = 10001,
		ID_TEXTCTRL2 = 10002,
		ID_CHOICE1 = 10005,
		ID_TEXTCTRL = 10003,
		ID_TEXTCTRL1 = 10004
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
	DialogMovePiece();
	DialogMovePiece(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	virtual ~DialogMovePiece();

	virtual bool Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

	bool InitializeData(const ImageInfo* pImageInfo, const ImagePieceEditor::TV_PIECE_INFO& vPieces);

	const ImageInfo* GetFromImageInfo() const;
	const ImageInfo* GetToImageInfo() const;

private:
	void Init();
	void CreateControls();

	void OnChoImageIdsSelected(wxCommandEvent& event);
	void OnOkClick(wxCommandEvent& event);
	void OnCancelClick(wxCommandEvent& event);

	void InitToImageInfo(const ImageInfo* pImageInfo);
	bool AddPieceToExistingImage(const wxSize& newSize);
	bool GetPieceFromList(TV_PACKING_PIECE_INFO& vPackingInfo);
	bool GetPieceFromImage(TV_PACKING_PIECE_INFO& vPackingInfo, const ImageInfo* pImageInfo);
	bool GeneratePackingInfo(TV_PACKING_PIECE_INFO& vPackingInfo, const wxSize& bmpSize);
	void FreePackingPiecesInfo(TV_PACKING_PIECE_INFO& vPackingInfo);
	wxBitmap* PackImage(const wxSize& bmpSize, const TV_PACKING_PIECE_INFO& vPackingInfo);

private:
	wxListBox* m_LstPieces;
	wxTextCtrl* m_EdtFromImage;
	wxChoice* m_ChoToImage;
	wxTextCtrl* m_EdtImageWidth;
	wxTextCtrl* m_EdtImageHeight;

	wxString m_strError;
	RectangleBinPack m_Packer;
	const ImageInfo* m_pFromImageInfo;
	const ImageInfo* m_pToImageInfo;

};

#endif // __DIALOGMOVEPIECE_H__
