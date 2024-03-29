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

	TextureCube_Impl(const tstring& id, const tstring& strFile);
	virtual ~TextureCube_Impl();

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
	bool CreateTextures(const tstring& strFile);
	void FreeTextures();

private:
	Vector2 m_Size;
	uint m_nTexWidth;
	uint m_nTexHeight;

	uint m_nHandler;
	uint m_nTexFormat;
	uint m_nFilter;

};
#endif // __TEXTURECUBE_IMPL_H__
