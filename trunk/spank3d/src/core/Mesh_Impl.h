/*!
 * \file Mesh_Impl.h
 * \date 4-19-2012 22:48:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MESH_IMPL_H__
#define __MESH_IMPL_H__

#include <core/IMesh.h>
#include "MeshPiece_Impl.h"
#include <vector>

class Mesh_Impl : public IMesh
{
public:
	typedef std::vector<MeshPiece_Impl*> TV_MESH_PIECE;

public:
	RTTI_DEF(Mesh_Impl, IObject);

	Mesh_Impl(const tstring& strFullPath);
	virtual ~Mesh_Impl();

	virtual const tstring& GetId() const;

	virtual int GetNumPieces() const;
	virtual IMeshPiece* GetPiece(int nIndex) const;
	virtual IMeshPiece* FindPiece(const tstring& strName) const;

	virtual const Vector3& GetBoundingBoxMin() const;
	virtual const Vector3& GetBoundingBoxMax() const;

private:
	void Init();
	void Destroy();

	bool CreatePieces(const tstring& strFullPath);
	void DestroyPieces();

private:
	tstring m_strId;
	TV_MESH_PIECE m_vMeshPiece;
	Vector3 m_vBoundingBoxMin;
	Vector3 m_vBoundingBoxMax;

};
#endif // __MESH_IMPL_H__
