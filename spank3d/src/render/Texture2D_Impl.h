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

	Texture2D_Impl(const tstring& id);
	Texture2D_Impl(const tstring& id, uint width, uint height, uint depth, TEXTURE_SAMPLE eSample);
	virtual ~Texture2D_Impl();

	virtual const Vector2& GetSize() const;
	virtual uint GetWidth() const;
	virtual uint GetHeight() const;

	virtual uint GetTextureId() const;

	bool LoadFromBitmapData(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample);

private:
	bool CreateTexture(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample);
	bool CreateTexture(uint width, uint height, uint depth, TEXTURE_SAMPLE eSample);
	void FreeTexture();

private:
	Vector2 m_Size;
	uint m_nTexWidth;
	uint m_nTexHeight;

	uint m_nTextureId;

};
#endif // __TEXTURE2D_IMPL_H__
