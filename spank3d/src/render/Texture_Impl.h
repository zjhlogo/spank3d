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
#include <gl/glew.h>

class Texture_Impl : public ITexture
{
public:
	RTTI_DEF(Texture_Impl, ITexture);

	Texture_Impl();
	virtual ~Texture_Impl();

	virtual uint GetWidth() const;
	virtual uint GetHeight() const;

	bool LoadFromBitmapData(const IBitmapData* pBitmapData);

private:
	bool CreateTexture(const IBitmapData* pBitmapData);
	void FreeTexture();
	bool IsValidTextureSize(uint width, uint height);

private:
	uint m_nWidth;
	uint m_nHeight;
	GLuint m_nTextureId;

};
#endif // __TEXTURE_IMPL_H__
