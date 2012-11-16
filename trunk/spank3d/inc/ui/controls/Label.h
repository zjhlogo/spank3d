/*!
 * \file Label.h
 * \date 11-08-2012 14:38:14
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __LABEL_H__
#define __LABEL_H__

#include "IWindow.h"
#include "../style/IFontStyle.h"

class Label : public IWindow
{
public:
	RTTI_DEF(Label, IWindow);

	Label(IWindow* pParent, bool solid = false);
	virtual ~Label();

	void SetLabel(const tstring& strLabel);
	const tstring& GetLabel() const;

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);
	virtual void AdjustSize();

protected:
	IFontStyle* m_pFontStyle;
	tstring m_strLabel;
	Vector2 m_TextSize;

	Vector2 m_paddingLT;
	Vector2 m_paddingRB;

};
#endif // __LABEL_H__
