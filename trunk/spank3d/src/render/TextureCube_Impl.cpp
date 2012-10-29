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

TextureCube_Impl::TextureCube_Impl(const tstring& id)
:ITexture(id)
{
	m_nTextureId = 0;
}

TextureCube_Impl::~TextureCube_Impl()
{
	FreeTextures();
	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

const Vector2& TextureCube_Impl::GetSize() const
{
	return m_Size;
}

uint TextureCube_Impl::GetTextureHandler() const
{
	return m_nTextureId;
}

bool TextureCube_Impl::LoadFromFile(const tstring& strFile)
{
	SetOk(CreateTextures(strFile));
	return IsOk();
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

	glGenTextures(1, &m_nTextureId);
	if (m_nTextureId == 0) return false;
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_nTextureId);

	TiXmlElement* pXmlTexture = doc.RootElement()->FirstChildElement(_("Texture"));
	for (int i = 0; i < NUM_TARGETS; ++i)
	{
		if (!pXmlTexture) return false;

		const tchar* pszTexPath = pXmlTexture->Attribute(_("path"));
		if (!pszTexPath) return false;

		IBitmapData* pBitmapData = g_pResMgr->CreateBitmapData(pszTexPath);
		if (!pBitmapData) return false;

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
			SAFE_RELEASE(pBitmapData);
			return false;
		}

		int width = pBitmapData->GetWidth();
		int height = pBitmapData->GetHeight();
		if (!TextureUtil::IsValidTextureSize(width, height))
		{
			SAFE_RELEASE(pBitmapData);
			LOG(_T("invalid texture size: %dx%d"), width, height);
			return false;
		}

		m_Size.Reset(float(width), float(height));
		glTexImage2D(s_Targets[i], 0, nColorFormat, width, height, 0, nColorFormat, GL_UNSIGNED_BYTE, pBitmapData->GetData());
		SAFE_RELEASE(pBitmapData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

void TextureCube_Impl::FreeTextures()
{
	if (m_nTextureId != 0)
	{
		glDeleteTextures(1, &m_nTextureId);
		m_nTextureId = 0;
	}
}
