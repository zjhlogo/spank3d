/*!
 * \file ResMgr_Impl.cpp
 * \date 4-19-2012 22:56:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ResMgr_Impl.h"
#include <Spank3D.h>
#include <util/LogUtil.h>
#include <util/StringUtil.h>

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

IMesh* ResMgr_Impl::CreateMesh(const tstring& strFile)
{
	// get file path
	tstring strFilePath;
	if (!GetMediaFilePath(strFilePath, strFile)) return NULL;

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

void ResMgr_Impl::SetDefaultDir(const tstring& strDir)
{
	m_strDefaultDir = strDir;
}

const tstring& ResMgr_Impl::GetDefaultDir()
{
	return m_strDefaultDir;
}

bool ResMgr_Impl::GetMediaFilePath(tstring& strFilePathOut, const tstring& strFile)
{
	strFilePathOut = m_strDefaultDir + _("\\") + strFile;
	StringUtil::tolower(strFilePathOut, strFilePathOut);
	return true;
}
