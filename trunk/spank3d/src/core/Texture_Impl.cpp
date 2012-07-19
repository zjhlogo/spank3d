/*!
 * \file Texture_Impl.cpp
 * \date 7-19-2012 14:29:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Texture_Impl.h"

Texture_Impl::Texture_Impl(const tstring& strFile)
{
	SetOk(CreateTexture(strFile));
}

Texture_Impl::~Texture_Impl()
{
	// TODO: 
}

const Vector2i& Texture_Impl::GetSize() const
{
	return m_vTexSize;
}

bool Texture_Impl::CreateTexture(const tstring& strFile)
{
	// TODO: 
	return false;
}
