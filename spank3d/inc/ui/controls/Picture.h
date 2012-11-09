/*!
 * \file Picture.h
 * \date 11-01-2012 15:31:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "IWindow.h"

class Picture : public IWindow
{
public:
	RTTI_DEF(Picture, IWindow);

	Picture(IWindow* pParent);
	virtual ~Picture();

	void SetTexture(ITexture* pTexture);
	ITexture* GetTexture();

protected:
	virtual bool Render(const Vector2& basePos, const Rect& clipRect, uint state);

private:
	ITexture* m_pTexture;

};
#endif // __PICTURE_H__
