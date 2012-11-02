/*!
 * \file Texture2D_Impl.h
 * \date 10-29-2012 10:09:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TEXTURE2D_IMPL_H__
#define __TEXTURE2D_IMPL_H__

#include <render/ITexture.h>
#include <core/IBitmapData.h>

class Texture2D_Impl : public ITexture
{
public:
	RTTI_DEF(Texture2D_Impl, ITexture);

	Texture2D_Impl(const tstring& id, const IBitmapData* pBitmapData);
	Texture2D_Impl(const tstring& id, uint width, uint height, uint nTexFormat);
	virtual ~Texture2D_Impl();

	virtual const Vector2& GetSize() const;
	virtual uint GetWidth() const;
	virtual uint GetHeight() const;

	virtual uint GetType() const;
	virtual uint GetHandler() const;
	virtual uint GetFormat() const;

	virtual void SetFilter(uint filter);
	virtual uint GetFilter() const;

private:
	void InitMember();
	bool CreateTexture(const IBitmapData* pBitmapData);
	bool CreateTexture(uint width, uint height, uint nTexFormat);
	void FreeTexture();

private:
	Vector2 m_Size;
	uint m_nWidth;
	uint m_nHeight;

	uint m_nHandler;
	uint m_nTexFormat;
	uint m_nFilter;

};
#endif // __TEXTURE2D_IMPL_H__
