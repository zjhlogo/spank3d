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
#include <util/PngUtil.h>
#include <tinyxml-2.6.2/tinyxml.h>

#include "BitmapData_Impl.h"
#include "../render/Texture_Impl.h"
#include "../render/Shader_Impl.h"

ResMgr_Impl::ResMgr_Impl()
{
	g_pResMgr = this;
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
	Mesh_Impl* pMesh = new Mesh_Impl(strFullPath);
	if (!pMesh || !pMesh->IsOk())
	{
		LOG(_("IResMgr::CreateMesh Failed %s"), strFullPath.c_str());
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strFullPath, pMesh));
	pMesh->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnMeshDestroyed);
	return pMesh;
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

	return InternalCreateBitmapData(id, width, height, bpp);
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

	return PngUtil::DecodePngFromFile(strFullPath);
}

ITexture* ResMgr_Impl::CreateTexture(const tstring& id, const IBitmapData* pBitmapData, ITexture::TEXTURE_SAMPLE eSample /* = ITexture::TS_LINEAR */)
{
	// check texture is exist in the cache
	TM_TEXTURE::iterator itFound = m_TextureMap.find(id);
	if (itFound != m_TextureMap.end())
	{
		ITexture* pTexture = itFound->second;
		pTexture->IncRef();
		return pTexture;
	}

	return InternalCreateTexture(id, pBitmapData, eSample);
}

ITexture* ResMgr_Impl::CreateTexture(const tstring& strFile, ITexture::TEXTURE_SAMPLE eSample /* = ITexture::TS_LINEAR */)
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

	IBitmapData* pBitmapData = PngUtil::DecodePngFromFile(strFullPath);
	if (!pBitmapData)
	{
		LOG(_("IResMgr::CreateTexture, create bitmap data failed %s"), strFullPath.c_str());
		return NULL;
	}

	ITexture* pTexture = InternalCreateTexture(strFullPath, pBitmapData, eSample);
	SAFE_RELEASE(pBitmapData);
	return pTexture;
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
		LOG(_("IResMgr::CreateShader failed, %s"), strFullPath.c_str());
		return NULL;
	}

	TiXmlDocument doc;
	doc.Parse(strXmlFile.c_str());
	if (doc.Error())
	{
		LOG(_("IResMgr::CreateShader, parse xml %s failed, error=%s"), strFullPath.c_str(), doc.ErrorDesc());
		return NULL;
	}

	// parse the xml files
	TiXmlElement* pXmlShader = doc.RootElement();
	if (!pXmlShader)
	{
		LOG(_("IResMgr::CreateShader, parse xml %s failed, no root element"), strFullPath.c_str());
		return NULL;
	}

	// vertex shader
	std::string strVertexShaderData;
	const char* pszVertexShader = pXmlShader->Attribute("vertex_shader");
	if (pszVertexShader) FileUtil::ReadFileIntoString(strVertexShaderData, StringUtil::char2tchar(pszVertexShader));

	// geometry shader
	std::string strGeometryShaderData;
	const char* pszGeometryShader = pXmlShader->Attribute("geometry_shader");
	if (pszGeometryShader) FileUtil::ReadFileIntoString(strGeometryShaderData, StringUtil::char2tchar(pszGeometryShader));

	// fragment shader
	std::string strFragmentShaderData;
	const char* pszFregmentShader = pXmlShader->Attribute("fregment_shader");
	if (pszFregmentShader) FileUtil::ReadFileIntoString(strFragmentShaderData, StringUtil::char2tchar(pszFregmentShader));

	// vertex attribute
	TiXmlElement* pElmAttrs = pXmlShader->FirstChildElement("attributes");
	if (!pElmAttrs)
	{
		LOG(_("IResMgr::CreateShader, parse xml %s failed, no attributes element"), strFullPath.c_str());
		return NULL;
	}

	VertexAttribute::ATTRIBUTE_ITEM attrItems[VertexAttribute::MAX_ATTRIBUTE_ITEMS+1];
	int nAttrIndex = 0;

	TiXmlElement* pXmlVertexAttribute = pElmAttrs->FirstChildElement("attribute");
	while (pXmlVertexAttribute)
	{
		int nSize = 0;
		pXmlVertexAttribute->Attribute("size", &nSize);
		if (nSize <= 0)
		{
			LOG(_("IResMgr::CreateShader, parse xml %s failed, no size attributes in attribute element"), strFullPath.c_str());
			return NULL;
		}

		const char* pszAttrName = pXmlVertexAttribute->Attribute("name");
		if (!pszAttrName)
		{
			LOG(_("IResMgr::CreateShader, parse xml %s failed, no name attributes in attribute element"), strFullPath.c_str());
			return NULL;
		}

		attrItems[nAttrIndex].nSize = nSize;
		attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_FLOAT;
		strncpy_s(attrItems[nAttrIndex].szParamName, pszAttrName, VertexAttribute::MAX_ATTRIBUTE_NAME_LENGTH);

		++nAttrIndex;
		pXmlVertexAttribute = pXmlVertexAttribute->NextSiblingElement("attribute");
	}

	if (nAttrIndex <= 0 || nAttrIndex > VertexAttribute::MAX_ATTRIBUTE_ITEMS)
	{
		LOG(_("IResMgr::CreateShader, parse xml %s failed, attributes count out of boundary, must 0 < count <= 8"), strFullPath.c_str());
		return NULL;
	}

	attrItems[nAttrIndex].nSize = 0;
	attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_UNKNOWN;
	attrItems[nAttrIndex].nOffset = 0;
	attrItems[nAttrIndex].szParamName[0] = '\0';

	// create shader
	Shader_Impl* pShader = new Shader_Impl(strFullPath, strVertexShaderData, strGeometryShaderData, strFragmentShaderData, attrItems);
	if (!pShader || !pShader->IsOk())
	{
		SAFE_RELEASE(pShader);
		LOG(_("IResMgr::CreateShader Failed"));
		return NULL;
	}

	// cache the shader
	m_ShaderMap.insert(std::make_pair(strFullPath, pShader));
	pShader->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnShaderDestroyed);
	return pShader;
}

bool ResMgr_Impl::ReadStringFile(tstring& strOut, const tstring& strFile)
{
	// get full path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return false;

	return FileUtil::ReadFileIntoString(strOut, strFullPath);
}

ITexture* ResMgr_Impl::InternalCreateTexture(const tstring& id, const IBitmapData* pBitmapData, ITexture::TEXTURE_SAMPLE eSample)
{
	Texture_Impl* pTexture = new Texture_Impl(id);
	if (!pTexture->LoadFromBitmapData(pBitmapData, eSample))
	{
		SAFE_DELETE(pTexture);
		LOG(_("IResMgr::InternalCreateTexture failed, %s"), id.c_str());
		return NULL;
	}

	// cache the texture res
	m_TextureMap.insert(std::make_pair(id, pTexture));
	pTexture->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnTextureDestroyed);
	return pTexture;
}

IBitmapData* ResMgr_Impl::InternalCreateBitmapData(const tstring& id, uint width, uint height, uint bpp)
{
	BitmapData_Impl* pBitmapData = new BitmapData_Impl(id, width, height, bpp);
	if (!pBitmapData || !pBitmapData->IsOk())
	{
		SAFE_DELETE(pBitmapData);
		LOG(_("IResMgr::InternalCreateBitmapData failed, width=%d, height=%d, bpp=%d"), width, height, bpp);
		return NULL;
	}

	// cache the bitmap data
	m_BitmapDataMap.insert(std::make_pair(id, pBitmapData));
	pBitmapData->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&ResMgr_Impl::OnBitmapDataDestroyed);
	return pBitmapData;
}

bool ResMgr_Impl::OnMeshDestroyed(Event& event)
{
	IMesh* pMesh = (IMesh*)event.GetEventDispatcher();
	if (!pMesh) return true;

	TM_MESH::iterator itFound = m_MeshMap.find(pMesh->GetId());
	if (itFound == m_MeshMap.end()) return true;

	m_MeshMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnBitmapDataDestroyed(Event& event)
{
	IBitmapData* pBitmapData = (IBitmapData*)event.GetEventDispatcher();
	if (!pBitmapData) return true;

	TM_BITMAP_DATA::iterator itFound = m_BitmapDataMap.find(pBitmapData->GetId());
	if (itFound == m_BitmapDataMap.end()) return true;

	m_BitmapDataMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnTextureDestroyed(Event& event)
{
	ITexture* pTexture = (ITexture*)event.GetEventDispatcher();
	if (!pTexture) return true;

	TM_TEXTURE::iterator itFound = m_TextureMap.find(pTexture->GetId());
	if (itFound == m_TextureMap.end()) return true;

	m_TextureMap.erase(itFound);
	return true;
}

bool ResMgr_Impl::OnShaderDestroyed(Event& event)
{
	IShader* pShader = (IShader*)event.GetEventDispatcher();
	if (!pShader) return true;

	TM_SHADER::iterator itFound = m_ShaderMap.find(pShader->GetId());
	if (itFound == m_ShaderMap.end()) return true;

	m_ShaderMap.erase(itFound);
	return true;
}
