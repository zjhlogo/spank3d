/*!
 * \file NineGridStyleEditor.h
 * \date 2-15-2012 12:44:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __NINEGRIDSTYLEEDITOR_H__
#define __NINEGRIDSTYLEEDITOR_H__

#include "BaseEditor.h"
#include "../document/NineGridStyle.h"

class NineGridStyleEditor : public BaseEditor
{
public:
	NineGridStyleEditor();
	NineGridStyleEditor(wxWindow *parent,
		wxWindowID winid,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

	virtual ~NineGridStyleEditor();

	static NineGridStyleEditor& GetInstance();

	virtual void Reset();
	bool SetNineGridStyle(const NineGridStyle* pNineGridStyle);
	const NineGridStyle* GetNineGridStyle();

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

	void DoSelState(const wxPoint& pos);

private:
	static NineGridStyleEditor* m_pNineGridStyleEditor;
	const NineGridStyle* m_pNineGridStyle;

	IStyle::STYLE_STATE m_eSelState;
	wxRect m_rectState[IStyle::SS_NUM];
	wxSize m_TotalSize;
};
#endif // __NINEGRIDSTYLEEDITOR_H__
