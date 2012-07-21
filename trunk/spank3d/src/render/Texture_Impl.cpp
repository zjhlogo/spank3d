/*!
 * \file Texture_Impl.cpp
 * \date 7-19-2012 14:29:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Texture_Impl.h"
#include <util/LogUtil.h>

Texture_Impl::Texture_Impl()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nTextureId = 0;
}

Texture_Impl::~Texture_Impl()
{
	FreeTexture();
}

uint Texture_Impl::GetWidth() const
{
	return m_nWidth;
}

uint Texture_Impl::GetHeight() const
{
	return m_nHeight;
}

bool Texture_Impl::LoadFromBitmapData(const IBitmapData* pBitmapData)
{
	SetOk(CreateTexture(pBitmapData));
	return IsOk();
}

bool Texture_Impl::CreateTexture(const IBitmapData* pBitmapData)
{
	FreeTexture();

	glGenTextures(1, &m_nTextureId);
	if (m_nTextureId == 0) return false;

	m_nWidth = pBitmapData->GetWidth();
	m_nHeight = pBitmapData->GetHeight();

	if (!IsValidTextureSize(m_nWidth, m_nHeight))
	{
		LOG("invalid texture size: %dx%d", m_nWidth, m_nHeight);
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_nTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBitmapData->GetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}

void Texture_Impl::FreeTexture()
{
	if (m_nTextureId != 0)
	{
		glDeleteTextures(1, &m_nTextureId);
		m_nTextureId = 0;
	}
	m_nWidth = 0;
	m_nHeight = 0;
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
