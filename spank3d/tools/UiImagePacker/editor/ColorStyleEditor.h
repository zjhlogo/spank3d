/*!
 * \file ColorStyleEditor.h
 * \date 2-15-2012 14:16:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __COLORSTYLEEDITOR_H__
#define __COLORSTYLEEDITOR_H__

#include "BaseEditor.h"
#include "../document/ColorStyle.h"

class ColorStyleEditor : public BaseEditor
{
public:
	ColorStyleEditor();
	ColorStyleEditor(wxWindow *parent,
		wxWindowID winid,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

	virtual ~ColorStyleEditor();

	static ColorStyleEditor& GetInstance();

	virtual void Reset();
	bool SetColorStyle(const ColorStyle* pColorStyle);
	const ColorStyle* GetColorStyle();

	virtual wxSize CalculateVirtualSize();
	virtual void Draw(wxDC& dc);
	virtual void OnLButtonDown(wxMouseEvent& event);

	void SetSelState(IStyle::STYLE_STATE eState);
	IStyle::STYLE_STATE GetSelState() const;

private:
	void Init();
	void Release();

	void UpdateSubRect();

	void DrawSelection(wxDC& dc);

private:
	static ColorStyleEditor* m_pColorStyleEditor;
	const ColorStyle* m_pColorStyle;

	IStyle::STYLE_STATE m_eSelState;
	wxRect m_rectState[IStyle::SS_NUM];
	wxSize m_TotalSize;

};
#endif // __COLORSTYLEEDITOR_H__
