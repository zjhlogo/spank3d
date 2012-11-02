/*!
 * \file TextureCube_Impl.cpp
 * \date 10-29-2012 10:14:19
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "TextureCube_Impl.h"
#include <util/LogUtil.h>
#include <util/TextureUtil.h>
#include <util/FileUtil.h>
#include <tinyxml-2.6.2/tinyxml.h>
#include <gl/glew.h>
#include <event/EventIds.h>
#include <Spank3d.h>

TextureCube_Impl::TextureCube_Impl(const tstring& id, const tstring& strFile)
:ITexture(id)
{
	InitMember();
	SetOk(CreateTextures(strFile));
}

TextureCube_Impl::~TextureCube_Impl()
{
	FreeTextures();
	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

void TextureCube_Impl::InitMember()
{
	m_nHandler = 0;
	m_nTexWidth = 0;
	m_nTexHeight = 0;
	m_nTexFormat = TEXTURE_FORMAT::TF_RGBA;
	m_nFilter = TEXTURE_FILTER::TF_LINEAR;
}

const Vector2& TextureCube_Impl::GetSize() const
{
	return m_Size;
}

uint TextureCube_Impl::GetWidth() const
{
	return m_nTexWidth;
}

uint TextureCube_Impl::GetHeight() const
{
	return m_nTexHeight;
}

uint TextureCube_Impl::GetType() const
{
	return TEXTURE_TYPE::TT_TEXTURE_CUBE_MAP;
}

uint TextureCube_Impl::GetHandler() const
{
	return m_nHandler;
}

uint TextureCube_Impl::GetFormat() const
{
	return m_nTexFormat;
}

void TextureCube_Impl::SetFilter(uint filter)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_nHandler);
	m_nFilter = filter;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_nFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_nFilter);
}

uint TextureCube_Impl::GetFilter() const
{
	return m_nFilter;
}

bool TextureCube_Impl::CreateTextures(const tstring& strFile)
{
	static const uint s_Targets[NUM_TARGETS] = 
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	FreeTextures();

	tstring strXmlData;
	if (!FileUtil::ReadFileIntoString(strXmlData, strFile))
	{
		LOG(_T("TextureCube_Impl::CreateTextures, load xml %s failed"), strFile.c_str());
		return false;
	}

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error())
	{
		LOG(_T("TextureCube_Impl::CreateTextures, parse xml %s failed, error=%s"), strFile.c_str(), doc.ErrorDesc());
		return false;
	}

	glGenTextures(1, &m_nHandler);
	if (m_nHandler == 0) return false;
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_nHandler);

	TiXmlElement* pXmlTexture = doc.RootElement()->FirstChildElement(_("Texture"));
	for (int i = 0; i < NUM_TARGETS; ++i)
	{
		if (!pXmlTexture) return false;

		const tchar* pszTexPath = pXmlTexture->Attribute(_("path"));
		if (!pszTexPath) return false;

		IBitmapData* pBitmapData = g_pResMgr->CreateBitmapData(pszTexPath);
		if (!pBitmapData) return false;

		m_nTexFormat = TextureUtil::Bpp2TexFormat(pBitmapData->GetBPP());
		m_nTexWidth = pBitmapData->GetWidth();
		m_nTexHeight = pBitmapData->GetHeight();
		if (!TextureUtil::IsValidTextureSize(m_nTexWidth, m_nTexHeight))
		{
			SAFE_RELEASE(pBitmapData);
			LOG(_T("invalid texture size: %dx%d"), m_nTexWidth, m_nTexHeight);
			return false;
		}

		m_Size.Reset(float(m_nTexWidth), float(m_nTexHeight));
		glTexImage2D(s_Targets[i], 0, m_nTexFormat, m_nTexWidth, m_nTexHeight, 0, m_nTexFormat, GL_UNSIGNED_BYTE, pBitmapData->GetData());
		SAFE_RELEASE(pBitmapData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return true;
}

void TextureCube_Impl::FreeTextures()
{
	if (m_nHandler != 0)
	{
		glDeleteTextures(1, &m_nHandler);
		m_nHandler = 0;
	}
}
