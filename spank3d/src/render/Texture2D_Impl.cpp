/*!
 * \file Texture2D_Impl.cpp
 * \date 10-29-2012 10:10:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Texture2D_Impl.h"
#include <util/LogUtil.h>
#include <util/TextureUtil.h>
#include <gl/glew.h>
#include <event/EventIds.h>

Texture2D_Impl::Texture2D_Impl(const tstring& id)
:ITexture(id)
{
	m_nTextureId = 0;
}

Texture2D_Impl::~Texture2D_Impl()
{
	FreeTexture();
	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

const Vector2& Texture2D_Impl::GetSize() const
{
	return m_Size;
}

uint Texture2D_Impl::GetTextureHandler() const
{
	return m_nTextureId;
}

bool Texture2D_Impl::LoadFromBitmapData(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample)
{
	SetOk(CreateTexture(pBitmapData, eSample));
	return IsOk();
}

bool Texture2D_Impl::CreateTexture(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample)
{
	FreeTexture();

	glGenTextures(1, &m_nTextureId);
	if (m_nTextureId == 0) return false;

	uint nColorFormat = GL_RGBA;
	switch (pBitmapData->GetBPP())
	{
	case 8:
		nColorFormat = GL_RED;
		break;
	case 16:
		nColorFormat = GL_RG;
		break;
	case 24:
		nColorFormat = GL_RGB;
		break;
	case 32:
		nColorFormat = GL_RGBA;
		break;
	default:
		return false;
	}

	int width = pBitmapData->GetWidth();
	int height = pBitmapData->GetHeight();
	if (!TextureUtil::IsValidTextureSize(width, height))
	{
		LOG(_T("invalid texture size: %dx%d"), width, height);
		return false;
	}

	m_Size.Reset(float(width), float(height));

	glBindTexture(GL_TEXTURE_2D, m_nTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, nColorFormat, width, height, 0, nColorFormat, GL_UNSIGNED_BYTE, pBitmapData->GetData());

	uint texSample = GL_LINEAR;
	if (eSample == TS_NEAREST) texSample = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texSample);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texSample);

	return true;
}

void Texture2D_Impl::FreeTexture()
{
	if (m_nTextureId != 0)
	{
		glDeleteTextures(1, &m_nTextureId);
		m_nTextureId = 0;
	}
}
