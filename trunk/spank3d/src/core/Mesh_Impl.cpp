/*!
 * \file Mesh.cpp
 * \date 4-19-2012 22:19:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Mesh_Impl.h"
#include <util/FileUtil.h>

Mesh_Impl::Mesh_Impl(const tstring& strFile)
{
	Init();
	CreatePieces(strFile);
}

Mesh_Impl::~Mesh_Impl()
{
	Destroy();
}

void Mesh_Impl::Init()
{
	m_vBoundingBoxMin.Reset(Math::FLOAT_MAX, Math::FLOAT_MAX, Math::FLOAT_MAX);
	m_vBoundingBoxMax.Reset(Math::FLOAT_MIN, Math::FLOAT_MIN, Math::FLOAT_MIN);
}

void Mesh_Impl::Destroy()
{
	DestroyPieces();
}

int Mesh_Impl::GetNumPieces() const
{
	return (int)m_vMeshPiece.size();
}

IMeshPiece* Mesh_Impl::GetPiece(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_vMeshPiece.size()) return NULL;
	return m_vMeshPiece[nIndex];
}

IMeshPiece* Mesh_Impl::FindPiece(const tstring& strName) const
{
	for (TV_MESH_PIECE::const_iterator it = m_vMeshPiece.begin(); it != m_vMeshPiece.end(); ++it)
	{
		if ((*it)->GetName() == strName) return (*it);
	}

	return NULL;
}

const Vector3& Mesh_Impl::GetBoundingBoxMin() const
{
	return m_vBoundingBoxMin;
}

const Vector3& Mesh_Impl::GetBoundingBoxMax() const
{
	return m_vBoundingBoxMax;
}

bool Mesh_Impl::CreatePieces(const tstring& strFile)
{
	DestroyPieces();

	IFile* pFile = FileUtil::LoadFile(strFile);
	if (!pFile) return false;

	FmtMesh::FILE_HEADER header;
	pFile->Read(&header, sizeof(header));

	if (header.nMagicNumber != FmtMesh::MAGIC_NUMBER
		|| header.nVersion != FmtMesh::CURRENT_VERSION)
	{
		SAFE_RELEASE(pFile);
		return false;
	}

	m_vBoundingBoxMin.Reset(header.fBoundingBoxMin[0], header.fBoundingBoxMin[1], header.fBoundingBoxMin[2]);
	m_vBoundingBoxMax.Reset(header.fBoundingBoxMax[0], header.fBoundingBoxMax[1], header.fBoundingBoxMax[2]);

	// read piece info
	std::vector<FmtMesh::MESH_PIECE> vPieces;
	if (header.nNumPieces > 0)
	{
		vPieces.resize(header.nNumPieces);
		pFile->Read(&vPieces[0], sizeof(FmtMesh::MESH_PIECE)*header.nNumPieces);
	}

	// create pieces
	for (int i = 0; i < header.nNumPieces; ++i)
	{
		MeshPiece_Impl* pPiece = new MeshPiece_Impl(vPieces[i], pFile);
		if (!pPiece || !pPiece->IsOk())
		{
			SAFE_RELEASE(pFile);
			DestroyPieces();
			return false;
		}
		m_vMeshPiece.push_back(pPiece);
	}

	SAFE_RELEASE(pFile);
	SetOk(true);
	return true;
}

void Mesh_Impl::DestroyPieces()
{
	for (TV_MESH_PIECE::iterator it = m_vMeshPiece.begin(); it != m_vMeshPiece.end(); ++it)
	{
		MeshPiece_Impl* pPiece = (*it);
		SAFE_RELEASE(pPiece);
	}
	m_vMeshPiece.clear();
}
