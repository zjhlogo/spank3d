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
	tstring strFilePath;
	if (!StringUtil::GetFileFullPath(strFilePath, m_strDefaultDir, strFile)) return NULL;

	// check whether the mesh created
	TM_MESH::iterator itfound = m_MeshMap.find(strFilePath);
	if (itfound != m_MeshMap.end())
	{
		IMesh* pMesh = itfound->second;
		pMesh->IncReference();
		return pMesh;
	}

	// create new
	Mesh_Impl* pMesh = new Mesh_Impl(strFilePath);
	if (!pMesh || !pMesh->IsOk())
	{
		LOG(_("IResMgr::CreateMesh Failed %s"), strFilePath.c_str());
		SAFE_RELEASE(pMesh);
		return NULL;
	}

	m_MeshMap.insert(std::make_pair(strFilePath, pMesh));
	return pMesh;
}

IBitmapData* ResMgr_Impl::CreateBitmapData(uint width, uint height, uint bpp /*= 32*/)
{
	BitmapData_Impl* pBitmapData = new BitmapData_Impl(width, height, bpp);
	if (!pBitmapData || !pBitmapData->IsOk())
	{
		SAFE_DELETE(pBitmapData);
		return NULL;
	}

	return pBitmapData;
}

IBitmapData* ResMgr_Impl::CreateBitmapData(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!StringUtil::GetFileFullPath(strFilePath, m_strDefaultDir, strFile)) return NULL;
	return InternalCreateBitmapData(strFilePath);
}

ITexture* ResMgr_Impl::CreateTexture(const IBitmapData* pBitmapData)
{
	Texture_Impl* pTexture = new Texture_Impl();
	if (!pTexture->LoadFromBitmapData(pBitmapData))
	{
		SAFE_DELETE(pTexture);
		return NULL;
	}

	return pTexture;
}

ITexture* ResMgr_Impl::CreateTexture(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!StringUtil::GetFileFullPath(strFilePath, m_strDefaultDir, strFile)) return NULL;

	// TODO: check texture is exist in the cache
	IBitmapData* pBitmapData = InternalCreateBitmapData(strFilePath);
	if (!pBitmapData) return NULL;

	ITexture* pTexture = CreateTexture(pBitmapData);
	// TODO: cache the texture res

	return pTexture;
}

IShader* ResMgr_Impl::CreateShader(const tstring& strFile)
{
	// get full path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return NULL;

	std::string strXmlFile;
	if (!FileUtil::ReadFileIntoString(strXmlFile, strFullPath)) return NULL;

	TiXmlDocument doc;
	doc.Parse(strXmlFile.c_str());
	if (doc.Error()) return NULL;

	// parse the xml files
	TiXmlElement* pXmlShader = doc.RootElement();
	if (!pXmlShader) return NULL;

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
	if (!pElmAttrs) return NULL;

	VertexAttribute::ATTRIBUTE_ITEM attrItems[VertexAttribute::MAX_ATTRIBUTE_ITEMS+1];
	int nAttrIndex = 0;

	TiXmlElement* pXmlVertexAttribute = pElmAttrs->FirstChildElement("attribute");
	while (pXmlVertexAttribute)
	{
		int nSize = 0;
		pXmlVertexAttribute->Attribute("size", &nSize);
		if (nSize <= 0) return NULL;

		const char* pszAttrName = pXmlVertexAttribute->Attribute("name");
		if (!pszAttrName) return NULL;

		attrItems[nAttrIndex].nSize = nSize;
		attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_FLOAT;
		strncpy_s(attrItems[nAttrIndex].szParamName, pszAttrName, VertexAttribute::MAX_ATTRIBUTE_NAME_LENGTH);

		++nAttrIndex;
		pXmlVertexAttribute = pXmlVertexAttribute->NextSiblingElement("attribute");
	}

	if (nAttrIndex <= 0 || nAttrIndex > VertexAttribute::MAX_ATTRIBUTE_ITEMS) return NULL;

	attrItems[nAttrIndex].nSize = 0;
	attrItems[nAttrIndex].eItemType = VertexAttribute::AIT_UNKNOWN;
	attrItems[nAttrIndex].nOffset = 0;
	attrItems[nAttrIndex].szParamName[0] = '\0';

	return InternalCreateShader(strVertexShaderData, strGeometryShaderData, strFragmentShaderData, attrItems);
}

bool ResMgr_Impl::ReadStringFile(tstring& strOut, const tstring& strFile)
{
	// get full path
	tstring strFullPath;
	if (!StringUtil::GetFileFullPath(strFullPath, m_strDefaultDir, strFile)) return false;

	return FileUtil::ReadFileIntoString(strOut, strFullPath);
}

IBitmapData* ResMgr_Impl::InternalCreateBitmapData(const tstring& strFile)
{
	return PngUtil::DecodePngFromFile(strFile);
}

IShader* ResMgr_Impl::InternalCreateShader(const tstring& strVertexShader, const tstring& strGeometryShader, const tstring& strFragmentShader, const VertexAttribute::ATTRIBUTE_ITEM* pVertexAttrItem)
{
	Shader_Impl* pShader = new Shader_Impl(strVertexShader.c_str(), strGeometryShader.c_str(), strFragmentShader.c_str(), pVertexAttrItem);
	if (!pShader || !pShader->IsOk())
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	return pShader;
}
