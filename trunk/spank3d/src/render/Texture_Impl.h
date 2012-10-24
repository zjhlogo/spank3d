/*!
 * \file Texture_Impl.h
 * \date 7-19-2012 14:28:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TEXTURE_IMPL_H__
#define __TEXTURE_IMPL_H__

#include <render/ITexture.h>
#include <core/IBitmapData.h>

class Texture_Impl : public ITexture
{
public:
	RTTI_DEF(Texture_Impl, ITexture);

	Texture_Impl(const tstring& id);
	virtual ~Texture_Impl();

	virtual const tstring& GetId() const;
	virtual const Vector2& GetSize() const;

	bool LoadFromBitmapData(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample);

	uint GetTextureId();

private:
	bool CreateTexture(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample);
	void FreeTexture();
	bool IsValidTextureSize(uint width, uint height);

private:
	tstring m_strId;
	Vector2 m_Size;
	uint m_nTextureId;

};
#endif // __TEXTURE_IMPL_H__