/*!
 * \file MeshPiece_Impl.h
 * \date 4-19-2012 22:48:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MESHPIECE_IMPL_H__
#define __MESHPIECE_IMPL_H__

#include <core/IMeshPiece.h>
#include <type/FmtMesh.h>
#include <util/IFile.h>

class MeshPiece_Impl : public IMeshPiece
{
public:
	RTTI_DEF(MeshPiece_Impl, IMeshPiece);

	MeshPiece_Impl(const FmtMesh::MESH_PIECE& meshPiece, IFile* pFile);
	virtual ~MeshPiece_Impl();

	virtual const tstring& GetName() const;

	virtual int GetMaterialId() const;
	virtual int GetVertDeclMask() const;

	virtual int GetNumVerts() const;
	virtual int GetNumIndis() const;
	virtual void* GetVerts() const;
	virtual ushort* GetIndis() const;

private:
	void Init();
	void Destroy();

	bool Create(const FmtMesh::MESH_PIECE& meshPiece, IFile* pFile);

private:
	tstring m_strName;
	int m_nMaterialId;
	int m_nVertexAttributes;

	int m_nVerts;
	int m_nIndis;
	void* m_pVerts;
	ushort* m_pIndis;

};
#endif // __MESHPIECE_IMPL_H__
