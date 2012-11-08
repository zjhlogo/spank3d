/*!
 * \file ResMgr_Impl.cpp
 * \date 4-19-2012 22:56:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ResMgr_Impl.h"
#include <Spank3d.h>
#include <util/LogUtil.h>
#include <util/StringUtil.h>
#include <util/FileUtil.h>
#include <util/TextureUtil.h>
#include <tinyxml-2.6.2/tinyxml.h>

#include "BitmapData_Impl.h"
#include "../render/Texture2D_Impl.h"
#include "../render/TextureCube_Impl.h"
#include "../render/Shader_Impl.h"
#include "../render/RenderTarget_Impl.h"

ResMgr_Impl::ResMgr_Impl()
{
	g_pResMgr = this;
	m_nNumMemoryTextures = 0;
}

ResMgr_Impl::~ResMgr_Impl()
{
	g_pResMgr = NULL;
}

ResMgr_Impl& ResMgr_Impl::GetInstance()
{
	static ResMgr_Impl s_ResMgr_Impl;
	return s_ResMgr_Impl;
}

bool ResMgr_Impl::Initialize()
{
	SetDefaultDir(_("data"));
	return true;
}

void ResMgr_Impl::Terminate()
{
	// TODO: check m_MeshMap/m_BonesMap whether is empty, and logout
}

void ResMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& ResMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

IMesh* ResMgr_Impl::CreateMesh(const tstring& strFile)
{
	// get file path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return NULL;

	// check whether the mesh created
	TM_MESH::iterator itfound = m_MeshMap.find(strFullPath);
	if (itfound != m_MeshMap.end())
	{
		IMesh* pMesh = itfound->second;
		pMesh->IncRef();
		return pMesh;
	}

	// create new
	Mesh_Impl* pMesh_Impl = new Mesh_Impl(strFullPath);
	if (!pMesh_Impl || !pMesh_Impl->IsOk())
	{
		LOG(_T("IResMgr::CreateMesh Failed %s"), strFullPath.c_str());
		SAFE_RELEASE(pMesh_Impl);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strFullPath, pMesh_Impl));
	pMesh_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnMeshDestroyed);
	return pMesh_Impl;
}

IBitmapData* ResMgr_Impl::CreateBitmapData(const tstring& id, uint width, uint height, uint bpp /* = 32 */)
{
	// check bitmap data exist
	TM_BITMAP_DATA::iterator itFound = m_BitmapDataMap.find(id);
	if (itFound != m_BitmapDataMap.end())
	{
		IBitmapData* pBitmapData = itFound->second;
		pBitmapData->IncRef();
		return pBitmapData;
	}

	BitmapData_Impl* pBitmapData_Impl = new BitmapData_Impl(id, width, height, bpp);
	if (!pBitmapData_Impl || !pBitmapData_Impl->IsOk())
	{
		SAFE_DELETE(pBitmapData_Impl);
		LOG(_T("IResMgr::CreateBitmapData failed, width=%d, height=%d, bpp=%d"), width, height, bpp);
		return NULL;
	}

	// cache the bitmap data
	m_BitmapDataMap.insert(std::make_pair(id, pBitmapData_Impl));
	pBitmapData_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnBitmapDataDestroyed);
	return pBitmapData_Impl;
}

IBitmapData* ResMgr_Impl::CreateBitmapData(const tstring& strFile)
{
	// get file path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return NULL;

	// check bitmap data exist
	TM_BITMAP_DATA::iterator itFound = m_BitmapDataMap.find(strFullPath);
	if (itFound != m_BitmapDataMap.end())
	{
		IBitmapData* pBitmapData = itFound->second;
		pBitmapData->IncRef();
		return pBitmapData;
	}

	return TextureUtil::DecodePngFromFile(strFullPath);
}

ITexture* ResMgr_Impl::CreateTexture2D(uint width, uint height, uint nTexFormat /* = TEXTURE_FORMAT::TF_RGBA */)
{
	tstring strNewId;
	StringUtil::strformat(strNewId, _("memory_texture: %d"), m_nNumMemoryTextures++);

	// check texture is exist in the cache
	TM_TEXTURE::iterator itFound = m_TextureMap.find(strNewId);
	if (itFound != m_TextureMap.end())
	{
		LOG(_T("IResMgr::CreateTexture2D failed, duplicate texture id: %s"), strNewId.c_str());
		return NULL;
	}

	Texture2D_Impl* pTexture2D_Impl = new Texture2D_Impl(strNewId, width, height, nTexFormat);
	if (!pTexture2D_Impl || !pTexture2D_Impl->IsOk())
	{
		SAFE_DELETE(pTexture2D_Impl);
		LOG(_T("IResMgr::CreateTexture2D failed, with width:%d, height:%d, format:0x%X"), width, height, nTexFormat);
		return NULL;
	}

	// cache the texture res
	m_TextureMap.insert(std::make_pair(strNewId, pTexture2D_Impl));
	pTexture2D_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnTextureDestroyed);
	return pTexture2D_Impl;
}

ITexture* ResMgr_Impl::CreateTexture2D(const tstring& id, const IBitmapData* pBitmapData)
{
	// check texture is exist in the cache
	TM_TEXTURE::iterator itFound = m_TextureMap.find(id);
	if (itFound != m_TextureMap.end())
	{
		ITexture* pTexture = itFound->second;
		pTexture->IncRef();
		return pTexture;
	}

	Texture2D_Impl* pTexture2D_Impl = new Texture2D_Impl(id, pBitmapData);
	if (!pTexture2D_Impl || !pTexture2D_Impl->IsOk())
	{
		SAFE_DELETE(pTexture2D_Impl);
		LOG(_T("IResMgr::CreateTexture2D failed, %s"), id.c_str());
		return NULL;
	}

	// cache the texture res
	m_TextureMap.insert(std::make_pair(id, pTexture2D_Impl));
	pTexture2D_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnTextureDestroyed);
	return pTexture2D_Impl;
}

ITexture* ResMgr_Impl::CreateTexture2D(const tstring& strFile)
{
	// get file path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return NULL;

	// check texture is exist in the cache
	TM_TEXTURE::iterator itFound = m_TextureMap.find(strFullPath);
	if (itFound != m_TextureMap.end())
	{
		ITexture* pTexture = itFound->second;
		pTexture->IncRef();
		return pTexture;
	}

	IBitmapData* pBitmapData = TextureUtil::DecodePngFromFile(strFullPath);
	if (!pBitmapData)
	{
		LOG(_T("IResMgr::CreateTexture2D, create bitmap data failed %s"), strFullPath.c_str());
		return NULL;
	}

	Texture2D_Impl* pTexture2D_Impl = new Texture2D_Impl(strFullPath, pBitmapData);
	SAFE_RELEASE(pBitmapData);

	if (!pTexture2D_Impl || !pTexture2D_Impl->IsOk())
	{
		SAFE_DELETE(pTexture2D_Impl);
		LOG(_T("IResMgr::CreateTexture2D failed, %s"), strFullPath.c_str());
		return NULL;
	}

	// cache the texture res
	m_TextureMap.insert(std::make_pair(strFullPath, pTexture2D_Impl));
	pTexture2D_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnTextureDestroyed);
	return pTexture2D_Impl;
}

ITexture* ResMgr_Impl::CreateTextureCube(const tstring& strFile)
{
	// get file path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return NULL;

	// check texture is exist in the cache
	TM_TEXTURE::iterator itFound = m_TextureMap.find(strFullPath);
	if (itFound != m_TextureMap.end())
	{
		ITexture* pTexture = itFound->second;
		pTexture->IncRef();
		return pTexture;
	}

	TextureCube_Impl* pTextureCube_Impl = new TextureCube_Impl(strFullPath, strFullPath);
	if (!pTextureCube_Impl || !pTextureCube_Impl->IsOk())
	{
		SAFE_DELETE(pTextureCube_Impl);
		LOG(_T("IResMgr::CreateTextureCube failed, %s"), strFile.c_str());
		return NULL;
	}

	// cache the texture res
	m_TextureMap.insert(std::make_pair(pTextureCube_Impl->GetId(), pTextureCube_Impl));
	pTextureCube_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnTextureDestroyed);
	return pTextureCube_Impl;
}

IShader* ResMgr_Impl::CreateShader(const tstring& strFile)
{
	// get full path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return NULL;

	// check shader exist
	TM_SHADER::iterator itFound = m_ShaderMap.find(strFullPath);
	if (itFound != m_ShaderMap.end())
	{
		IShader* pShader = itFound->second;
		pShader->IncRef();
		return pShader;
	}

	std::string strXmlFile;
	if (!FileUtil::ReadFileIntoString(strXmlFile, strFullPath))
	{
		LOG(_T("IResMgr::CreateShader failed, %s"), strFullPath.c_str());
		return NULL;
	}

	TiXmlDocument doc;
	doc.Parse(strXmlFile.c_str());
	if (doc.Error())
	{
		LOG(_T("IResMgr::CreateShader, parse xml %s failed, error=%s"), strFullPath.c_str(), doc.ErrorDesc());
		return NULL;
	}

	// parse the xml files
	TiXmlElement* pXmlShader = doc.RootElement();
	if (!pXmlShader)
	{
		LOG(_T("IResMgr::CreateShader, parse xml %s failed, no root element"), strFullPath.c_str());
		return NULL;
	}

	// vertex shader
	std::string strVertexShaderData;
	const tchar* pszVertexShader = pXmlShader->Attribute(_("vertex_shader"));
	if (pszVertexShader) FileUtil::ReadFileIntoString(strVertexShaderData, StringUtil::char2tchar(pszVertexShader));

	// geometry shader
	std::string strGeometryShaderData;
	const tchar* pszGeometryShader = pXmlShader->Attribute(_("geometry_shader"));
	if (pszGeometryShader) FileUtil::ReadFileIntoString(strGeometryShaderData, StringUtil::char2tchar(pszGeometryShader));

	// fragment shader
	std::string strFragmentShaderData;
	const tchar* pszFregmentShader = pXmlShader->Attribute(_("fregment_shader"));
	if (pszFregmentShader) FileUtil::ReadFileIntoString(strFragmentShaderData, StringUtil::char2tchar(pszFregmentShader));

	// vertex attribute
	TiXmlElement* pElmAttrs = pXmlShader->FirstChildElement(_("attributes"));
	if (!pElmAttrs)
	{
		LOG(_T("IResMgr::CreateShader, parse xml %s failed, no attributes element"), strFullPath.c_str());
		return NULL;
	}

	VertexAttribute::ATTRIBUTE_ITEM attrItems[VertexAttribute::MAX_ATTRIBUTE_ITEMS+1];
	int nAttrIndex = 0;

	TiXmlElement* pXmlVertexAttribute = pElmAttrs->FirstChildElement(_("attribute"));
	while (pXmlVertexAttribute)
	{
		int nSize = 0;
		pXmlVertexAttribute->Attribute(_("size"), &nSize);
		if (nSize <= 0)
		{
			LOG(_T("IResMgr::CreateShader, parse xml %s failed, no size attributes in attribute element"), strFullPath.c_str());
			return NULL;
		}

		const tchar* pszTypeName = pXmlVertexAttribute->Attribute(_("type"));
		if (!pszTypeName)
		{
			LOG(_T("IResMgr::CreateShader, parse xml %s failed, no type attributes in attribute element"), strFullPath.c_str());
			return NULL;
		}

		const tchar* pszAttrName = pXmlVertexAttribute->Attribute(_("name"));
		if (!pszAttrName)
		{
			LOG(_T("IResMgr::CreateShader, parse xml %s failed, no name attributes in attribute element"), strFullPath.c_str());
			return NULL;
		}

		attrItems[nAttrIndex].nSize = nSize;
		attrItems[nAttrIndex].eItemType = VertexAttribute::GetAttributeItemType(pszTypeName);
		strncpy_s(attrItems[nAttrIndex].szParamName, pszAttrName, VertexAttribute::MAX_ATTRIBUTE_NAME_LENGTH);

		++nAttrIndex;
		pXmlVertexAttribute = pXmlVertexAttribute->NextSiblingElement(_("attribute"));
	}

	if (nAttrIndex <= 0 || nAttrIndex > VertexAttribute::MAX_ATTRIBUTE_ITEMS)
	{
		LOG(_T("IResMgr::CreateShader, parse xml %s failed, attributes count out of boundary, must 0 < count <= 8"), strFullPath.c_str());
		return NULL;
	}

	attrItems[nAttrIndex].nSize = 0;
	attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_UNKNOWN;
	attrItems[nAttrIndex].nOffset = 0;
	attrItems[nAttrIndex].szParamName[0] = _('\0');

	// create shader
	Shader_Impl* pShader_Impl = new Shader_Impl(strFullPath, strVertexShaderData, strGeometryShaderData, strFragmentShaderData, attrItems);
	if (!pShader_Impl || !pShader_Impl->IsOk())
	{
		SAFE_RELEASE(pShader_Impl);
		LOG(_T("IResMgr::CreateShader Failed"));
		return NULL;
	}

	// cache the shader
	m_ShaderMap.insert(std::make_pair(strFullPath, pShader_Impl));
	pShader_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnShaderDestroyed);
	return pShader_Impl;
}


IRenderTarget* ResMgr_Impl::CreateRenderTarget(ITexture* pColorTexture, ITexture* pDepthTexture)
{
	RenderTarget_Impl* pRenderTarget_Impl = new RenderTarget_Impl(pColorTexture, pDepthTexture);
	if (!pRenderTarget_Impl || !pRenderTarget_Impl->IsOk())
	{
		SAFE_DELETE(pRenderTarget_Impl);
		LOG(_T("IResMgr::CreateRenderTarget failed"));
		return NULL;
	}

	m_RenderTargetSet.insert(pRenderTarget_Impl);
	pRenderTarget_Impl->RegisterEvent(ObjectEvent::OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnRenderTargetDestroyed);
	return pRenderTarget_Impl;
}

bool ResMgr_Impl::ReadStringFile(tstring& strOut, const tstring& strFile)
{
	// get full path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return false;

	return FileUtil::ReadFileIntoString(strOut, strFullPath);
}

bool ResMgr_Impl::OnMeshDestroyed(IEvent& event)
{
	IMesh* pMesh = (IMesh*)event.GetEventDispatcher();
	if (!pMesh) return true;

	TM_MESH::iterator itFound = m_MeshMap.find(pMesh->GetId());
	if (itFound == m_MeshMap.end()) return true;

	m_MeshMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnBitmapDataDestroyed(IEvent& event)
{
	IBitmapData* pBitmapData = (IBitmapData*)event.GetEventDispatcher();
	if (!pBitmapData) return true;

	TM_BITMAP_DATA::iterator itFound = m_BitmapDataMap.find(pBitmapData->GetId());
	if (itFound == m_BitmapDataMap.end()) return true;

	m_BitmapDataMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnTextureDestroyed(IEvent& event)
{
	ITexture* pTexture = (ITexture*)event.GetEventDispatcher();
	if (!pTexture) return true;

	TM_TEXTURE::iterator itFound = m_TextureMap.find(pTexture->GetId());
	if (itFound == m_TextureMap.end()) return true;

	m_TextureMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnShaderDestroyed(IEvent& event)
{
	IShader* pShader = (IShader*)event.GetEventDispatcher();
	if (!pShader) return true;

	TM_SHADER::iterator itFound = m_ShaderMap.find(pShader->GetId());
	if (itFound == m_ShaderMap.end()) return true;

	m_ShaderMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnRenderTargetDestroyed(IEvent& event)
{
	IRenderTarget* pRenderTarget = (IRenderTarget*)event.GetEventDispatcher();
	if (!pRenderTarget) return true;

	TS_RENDER_TARGET::iterator itFound = m_RenderTargetSet.find(pRenderTarget);
	if (itFound == m_RenderTargetSet.end()) return true;

	m_RenderTargetSet.erase(itFound);
	return true;
}
