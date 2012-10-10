/*!
 * \file Texture_Impl.cpp
 * \date 7-19-2012 14:29:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Texture_Impl.h"
#include <util/LogUtil.h>
#include <gl/glew.h>
#include <event/EventIds.h>

Texture_Impl::Texture_Impl(const tstring& id)
{
	m_strId = id;
	m_nTextureId = 0;
}

Texture_Impl::~Texture_Impl()
{
	FreeTexture();
	DispatchEvent(Event(EID_OBJECT_DESTROYED, this));
}

const tstring& Texture_Impl::GetId() const
{
	return m_strId;
}

const Vector2& Texture_Impl::GetSize() const
{
	return m_Size;
}

bool Texture_Impl::LoadFromBitmapData(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample)
{
	SetOk(CreateTexture(pBitmapData, eSample));
	return IsOk();
}

GLuint Texture_Impl::GetTextureId()
{
	return m_nTextureId;
}

bool Texture_Impl::CreateTexture(const IBitmapData* pBitmapData, TEXTURE_SAMPLE eSample)
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
	if (!IsValidTextureSize(width, height))
	{
		LOG("invalid texture size: %dx%d", width, height);
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

void Texture_Impl::FreeTexture()
{
	if (m_nTextureId != 0)
	{
		glDeleteTextures(1, &m_nTextureId);
		m_nTextureId = 0;
	}
}

bool Texture_Impl::IsValidTextureSize(uint width, uint height)
{
	static const uint s_nValidSize[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 0};

	bool bValidWidth = false;
	bool bValidHeight = false;

	int nIndex = 0;
	while (s_nValidSize[nIndex] != 0)
	{
		if (!bValidWidth && width == s_nValidSize[nIndex])
		{
			bValidWidth = true;
		}
		if (!bValidHeight && height == s_nValidSize[nIndex])
		{
			bValidHeight = true;
		}

		++nIndex;
	}

	return bValidWidth && bValidHeight;
}
