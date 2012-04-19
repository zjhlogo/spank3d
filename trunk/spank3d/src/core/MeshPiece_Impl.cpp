/*!
 * \file MeshPiece_Impl.cpp
 * \date 4-19-2012 22:48:09
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "MeshPiece_Impl.h"
#include <util/StringUtil.h>

MeshPiece_Impl::MeshPiece_Impl(const FmtMesh::MESH_PIECE& meshPiece, IFile* pFile)
{
	Init();
	Create(meshPiece, pFile);
}

MeshPiece_Impl::~MeshPiece_Impl()
{
	Destroy();
}

void MeshPiece_Impl::Init()
{
	m_nMaterialId = 0;
	m_nVertexAttributes = 0;

	m_nVerts = 0;
	m_nIndis = 0;
	m_pVerts = NULL;
	m_pIndis = NULL;
}

void MeshPiece_Impl::Destroy()
{
	SAFE_DELETE_ARRAY(m_pVerts);
	SAFE_DELETE_ARRAY(m_pIndis);
}

const tstring& MeshPiece_Impl::GetName() const
{
	return m_strName;
}

int MeshPiece_Impl::GetMaterialID() const
{
	return m_nMaterialId;
}

int MeshPiece_Impl::GetVertDeclMask() const
{
	return m_nVertexAttributes;
}

int MeshPiece_Impl::GetNumVerts() const
{
	return m_nVerts;
}

int MeshPiece_Impl::GetNumIndis() const
{
	return m_nIndis;
}

void* MeshPiece_Impl::GetVerts() const
{
	return m_pVerts;
}

ushort* MeshPiece_Impl::GetIndis() const
{
	return m_pIndis;
}

bool MeshPiece_Impl::Create(const FmtMesh::MESH_PIECE& meshPiece, IFile* pFile)
{
	StringUtil::char2tchar(m_strName, meshPiece.szName);

	m_nMaterialId = meshPiece.nMaterialId;
	m_nVertexAttributes = meshPiece.nVertexAttributes;

	m_nVerts = meshPiece.nNumVerts;
	m_pVerts = (void*)new uchar[meshPiece.nOffIndis - meshPiece.nOffVerts];
	pFile->Seek(meshPiece.nOffVerts);
	pFile->Read(m_pVerts, meshPiece.nOffIndis - meshPiece.nOffVerts);

	m_nIndis = meshPiece.nNumIndis;
	m_pIndis = new ushort[m_nIndis];
	pFile->Seek(meshPiece.nOffIndis);
	pFile->Read(m_pIndis, sizeof(ushort)*m_nIndis);

	SetOk(true);
	return true;
}
