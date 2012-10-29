/*!
 * \file TextureCube_Impl.h
 * \date 10-29-2012 10:14:14
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TEXTURECUBE_IMPL_H__
#define __TEXTURECUBE_IMPL_H__

#include <render/ITexture.h>
#include <core/IBitmapData.h>

class TextureCube_Impl : public ITexture
{
public:
	enum CONST_DEFINE
	{
		NUM_TARGETS = 6,
	};

public:
	RTTI_DEF(TextureCube_Impl, ITexture);

	TextureCube_Impl(const tstring& id);
	virtual ~TextureCube_Impl();

	virtual const Vector2& GetSize() const;
	virtual uint GetTextureHandler() const;

	bool LoadFromFile(const tstring& strFile);

private:
	bool CreateTextures(const tstring& strFile);
	void FreeTextures();

private:
	Vector2 m_Size;
	uint m_nTextureId;

};
#endif // __TEXTURECUBE_IMPL_H__
