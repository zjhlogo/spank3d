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
:ITexture(id, GL_TEXTURE_2D)
{
	m_nTextureId = 0;
	m_nTexWidth = 0;
	m_nTexHeight = 0;
}

Texture2D_Impl::Texture2D_Impl(const tstring& id, uint width, uint height, uint depth, TEXTURE_SAMPLE eSample)
:ITexture(id, GL_TEXTURE_2D)
{
	m_nTextureId = 0;
	m_nTexWidth = 0;
	m_nTexHeight = 0;

	SetOk(CreateTexture(width, height, depth, eSample));
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

uint Texture2D_Impl::GetWidth() const
{
	return m_nTexWidth;
}

uint Texture2D_Impl::GetHeight() const
{
	return m_nTexHeight;
}

uint Texture2D_Impl::GetTextureId() const
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

	m_nTexWidth = pBitmapData->GetWidth();
	m_nTexHeight = pBitmapData->GetHeight();
	if (!TextureUtil::IsValidTextureSize(m_nTexWidth, m_nTexHeight))
	{
		LOG(_T("invalid texture size: %dx%d"), m_nTexWidth, m_nTexHeight);
		return false;
	}

	m_Size.Reset(float(m_nTexWidth), float(m_nTexHeight));

	glBindTexture(GL_TEXTURE_2D, m_nTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, nColorFormat, m_nTexWidth, m_nTexHeight, 0, nColorFormat, GL_UNSIGNED_BYTE, pBitmapData->GetData());

	uint texSample = GL_LINEAR;
	if (eSample == TS_NEAREST) texSample = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texSample);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texSample);

	return true;
}

bool Texture2D_Impl::CreateTexture(uint width, uint height, uint depth, TEXTURE_SAMPLE eSample)
{
	FreeTexture();

	glGenTextures(1, &m_nTextureId);
	if (m_nTextureId == 0) return false;

	uint nColorFormat = GL_RGBA;
	switch (depth)
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

	if (!TextureUtil::IsValidTextureSize(width, height))
	{
		LOG(_T("invalid texture size: %dx%d"), width, height);
		return false;
	}

	m_nTexWidth = width;
	m_nTexHeight = height;
	m_Size.Reset(float(m_nTexWidth), float(m_nTexHeight));

	glBindTexture(GL_TEXTURE_2D, m_nTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, nColorFormat, m_nTexWidth, m_nTexHeight, 0, nColorFormat, GL_UNSIGNED_BYTE, NULL);

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
