/*!
 * \file ImagePieceEditor.h
 * \date 2-14-2012 1:24:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IMAGEPIECEEDITOR_H__
#define __IMAGEPIECEEDITOR_H__

#include "BaseEditor.h"
#include "../document/PieceInfo.h"
#include "../document/ImageInfo.h"
#include <vector>

class ImagePieceEditor : public BaseEditor
{
public:
	typedef std::vector<const PieceInfo*> TV_PIECE_INFO;

public:
	ImagePieceEditor();
	ImagePieceEditor(wxWindow *parent,
		wxWindowID winid,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

	virtual ~ImagePieceEditor();

	static ImagePieceEditor& GetInstance();

	virtual void Reset();

	bool SetSelPieceInfo(const PieceInfo* pPieceInfo);
	const PieceInfo* GetSelPieceInfo() const;
	const TV_PIECE_INFO& GetSelections();

	bool SetImageInfo(const ImageInfo* pImageInfo);
	const ImageInfo* GetImageInfo() const;

	virtual wxSize CalculateVirtualSize();
	virtual void Draw(wxDC& dc);
	virtual void OnLButtonDown(wxMouseEvent& event);

private:
	void Init();
	void Release();

	void DrawSelections(wxDC& dc);
	bool AddSelPieceInfo(const PieceInfo* pPieceInfo);
	bool RemoveSelPieceInfo(const PieceInfo* pPieceInfo);
	bool IsPieceInfoSelected(const PieceInfo* pPieceInfo);
	void ClearSelections();

private:
	static ImagePieceEditor* m_pImagePieceEditor;
	TV_PIECE_INFO m_vSelPieceInfo;
	const ImageInfo* m_pImageInfo;

};
#endif // __IMAGEPIECEEDITOR_H__
