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

Texture2D_Impl::Texture2D_Impl(const tstring& id, const IBitmapData* pBitmapData)
:ITexture(id)
{
	InitMember();
	SetOk(CreateTexture(pBitmapData));
}

Texture2D_Impl::Texture2D_Impl(const tstring& id, uint width, uint height, uint nTexFormat)
:ITexture(id)
{
	InitMember();
	SetOk(CreateTexture(width, height, nTexFormat));
}

Texture2D_Impl::~Texture2D_Impl()
{
	FreeTexture();
	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

void Texture2D_Impl::InitMember()
{
	m_nHandler = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nTexFormat = TEXTURE_FORMAT::TF_RGBA;
	m_nFilter = TEXTURE_FILTER::TF_LINEAR;
}

const Vector2& Texture2D_Impl::GetSize() const
{
	return m_Size;
}

uint Texture2D_Impl::GetWidth() const
{
	return m_nWidth;
}

uint Texture2D_Impl::GetHeight() const
{
	return m_nHeight;
}

uint Texture2D_Impl::GetType() const
{
	return TEXTURE_TYPE::TT_TEXTURE_2D;
}

uint Texture2D_Impl::GetHandler() const
{
	return m_nHandler;
}

uint Texture2D_Impl::GetFormat() const
{
	return m_nTexFormat;
}

void Texture2D_Impl::SetFilter(uint filter)
{
	glBindTexture(GL_TEXTURE_2D, m_nHandler);
	m_nFilter = filter;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_nFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_nFilter);
}

uint Texture2D_Impl::GetFilter() const
{
	return m_nFilter;
}

bool Texture2D_Impl::CreateTexture(const IBitmapData* pBitmapData)
{
	FreeTexture();

	glGenTextures(1, &m_nHandler);
	if (m_nHandler == 0) return false;

	m_nTexFormat = TextureUtil::Bpp2TexFormat(pBitmapData->GetBPP());
	m_nWidth = pBitmapData->GetWidth();
	m_nHeight = pBitmapData->GetHeight();
	if (!TextureUtil::IsValidTextureSize(m_nWidth, m_nHeight))
	{
		LOG(_T("invalid texture size: %dx%d"), m_nWidth, m_nHeight);
		return false;
	}

	m_Size.Reset(float(m_nWidth), float(m_nHeight));

	glBindTexture(GL_TEXTURE_2D, m_nHandler);
	glTexImage2D(GL_TEXTURE_2D, 0, m_nTexFormat, m_nWidth, m_nHeight, 0, m_nTexFormat, GL_UNSIGNED_BYTE, pBitmapData->GetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}

bool Texture2D_Impl::CreateTexture(uint width, uint height, uint nTexFormat)
{
	FreeTexture();

	glGenTextures(1, &m_nHandler);
	if (m_nHandler == 0) return false;

	m_nTexFormat = nTexFormat;
	if (!TextureUtil::IsValidTextureSize(width, height))
	{
		LOG(_T("invalid texture size: %dx%d"), width, height);
		return false;
	}

	m_nWidth = width;
	m_nHeight = height;
	m_Size.Reset(float(m_nWidth), float(m_nHeight));

	glBindTexture(GL_TEXTURE_2D, m_nHandler);
	glTexImage2D(GL_TEXTURE_2D, 0, m_nTexFormat, m_nWidth, m_nHeight, 0, m_nTexFormat, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}

void Texture2D_Impl::FreeTexture()
{
	if (m_nHandler != 0)
	{
		glDeleteTextures(1, &m_nHandler);
		m_nHandler = 0;
	}
}
