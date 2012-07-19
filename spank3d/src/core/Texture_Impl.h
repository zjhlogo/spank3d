/*!
 * \file Texture_Impl.h
 * \date 7-19-2012 14:28:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TEXTURE_IMPL_H__
#define __TEXTURE_IMPL_H__

#include <core/ITexture.h>

class Texture_Impl : public ITexture
{
public:
	RTTI_DEF(Texture_Impl, ITexture);

	Texture_Impl(const tstring& strFile);
	virtual ~Texture_Impl();

	virtual const Vector2i& GetSize() const;

private:
	bool CreateTexture(const tstring& strFile);

private:
	Vector2i m_vTexSize;

};
#endif // __TEXTURE_IMPL_H__
