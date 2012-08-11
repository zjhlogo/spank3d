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

#include "BitmapData_Impl.h"
#include "../render/Texture_Impl.h"

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

IBitmapData* ResMgr_Impl::InternalCreateBitmapData(const tstring& strFile)
{
	return PngUtil::DecodePngFromFile(strFile);
}
