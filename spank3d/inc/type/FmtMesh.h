/*!
 * \file FmtMesh.h
 * \date 4-19-2012 22:14:24
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FMTMESH_H__
#define __FMTMESH_H__

#include "BaseType.h"

#pragma pack(push)
#pragma pack(1)

class FmtMesh
{
public:
	enum CONST_DEFINE
	{
		MAGIC_NUMBER = 0x4853454D,		// MESH
		CURRENT_VERSION = 100,
		PIECE_NAME_SIZE = 32,
	};

	enum VERTEX_ATTRIBUTES
	{
		VA_POSITION		= 0x00000001,
		VA_TEXCOORD0	= 0x00000002,
		VA_TEXCOORD1	= 0x00000004,
		VA_TEXCOORD2	= 0x00000008,
		VA_TEXCOORD3	= 0x00000010,
		VA_TEXCOORD4	= 0x00000020,
		VA_TEXCOORD5	= 0x00000040,
		VA_TEXCOORD6	= 0x00000080,
		VA_TEXCOORD7	= 0x00000100,
		VA_COLOR		= 0x00000200,
		VA_NORMAL		= 0x00000400,
		VA_TANGENT		= 0x00000800,
		VA_BINORMAL		= 0x00001000,
		VA_SKELECTON	= 0x00002000,
	};

	enum PIECE_MASK
	{
		PM_VISIBLE		= 0x00000001,
		PM_TRANSPARENT	= 0x00000002,
	};

	// first, the file header
	typedef struct FILE_HEADER_tag
	{
		uint nMagicNumber;				// always MAGIC_NUMBER
		int nVersion;					// CURRENT_VERSION

		float fBoundingBoxMin[3];
		float fBoundingBoxMax[3];

		int nNumPieces;
	} FILE_HEADER;

	// second, the nNumPieces of MESH_PIECE
	typedef struct MESH_PIECE_tag
	{
		char szName[PIECE_NAME_SIZE];
		uint nPieceMask;				// see PIECE_MASK
		uint nVertexAttributes;			// see VERTEX_ATTRIBUTES

		int nMaterialId;				// see material id

		int nNumVerts;
		uint nOffVerts;					// sizeof(VERTEX)*nNumVerts

		int nNumIndis;
		uint nOffIndis;					// sizeof(ushort)*nNumIndis*3;
	} MESH_PIECE;
};

#pragma pack(pop)
#endif // __FMTMESH_H__
