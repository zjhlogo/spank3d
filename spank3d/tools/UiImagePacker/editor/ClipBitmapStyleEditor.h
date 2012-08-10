/*!
 * \file ClipBitmapStyleEditor.h
 * \date 2-15-2012 11:01:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __CLIPBITMAPSTYLEEDITOR_H__
#define __CLIPBITMAPSTYLEEDITOR_H__

#include "BaseEditor.h"
#include "../document/ClipBitmapStyle.h"

class ClipBitmapStyleEditor : public BaseEditor
{
public:
	ClipBitmapStyleEditor();
	ClipBitmapStyleEditor(wxWindow *parent,
		wxWindowID winid,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

	virtual ~ClipBitmapStyleEditor();

	static ClipBitmapStyleEditor& GetInstance();

	virtual void Reset();

	bool SetClipBitmapStyle(const ClipBitmapStyle* pClipBitmapStyle);
	const ClipBitmapStyle* GetClipBitmapStyle() const;

	virtual wxSize CalculateVirtualSize();
	virtual void Draw(wxDC& dc);
	virtual void OnLButtonDown(wxMouseEvent& event);

	void SetSelState(IStyle::STYLE_STATE eState);
	IStyle::STYLE_STATE GetSelState() const;

private:
	void Init();
	void Release();

	void UpdateSubBitmapRect();

	void DrawSelection(wxDC& dc);

private:
	static ClipBitmapStyleEditor* m_pClipBitmapStyleEditor;
	const ClipBitmapStyle* m_pClipBitmapStyle;

	IStyle::STYLE_STATE m_eSelState;
	wxRect m_rectState[IStyle::SS_NUM];
	wxSize m_TotalSize;

};
#endif // __CLIPBITMAPSTYLEEDITOR_H__
