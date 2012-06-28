/*!
 * \file ModelExporter.cpp
 * \date 1-3-2010 21:00:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelExporter.h"
#include "DlgOption.h"
#include <util/StringUtil.h>

DWORD WINAPI DummyFunc(LPVOID arg)
{
	return 0;
}

ModelExporter::ModelExporter()
{
	Init();
}

ModelExporter::~ModelExporter()
{
	Destroy();
}

void ModelExporter::Init()
{
	m_pInterface = NULL;
}

void ModelExporter::Destroy()
{
	// TODO: 
}

int ModelExporter::ExtCount()
{
	return 1;
}

const TCHAR* ModelExporter::Ext(int n)
{
	return _("mesh");
}

const TCHAR* ModelExporter::LongDesc()
{
	return _("Export to Spank3d Model file format");
}

const TCHAR* ModelExporter::ShortDesc()
{
	return _("Spank3d Model");
}

const TCHAR* ModelExporter::AuthorName()
{
	return _("zjhlogo");
}

const TCHAR* ModelExporter::CopyrightMessage()
{
	return _("Copyright (c) 2009, zjhlogo All rights reserved");
}

const TCHAR* ModelExporter::OtherMessage1()
{
	return _("Other message1");
}

const TCHAR* ModelExporter::OtherMessage2()
{
	return _("Other message2");
}

unsigned int ModelExporter::Version()
{
	return CURRENT_VERSION;
}

void ModelExporter::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, _("Spank3d Model Exporter"), _("Copyright (c) 2009, zjhlogo All rights reserved"), MB_OK);
}

int ModelExporter::DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts /* = FALSE */, DWORD options /* = 0 */)
{
	// show the option dialog
	DlgOption dlgOption(i->GetMAXHWnd());
	if (dlgOption.Show() != IDOK) return TRUE;

	Cleanup();
	m_pInterface = i;

	// initialize
	m_pInterface->ProgressStart(_("Initialize IGame interfaces"), TRUE, DummyFunc, NULL);
	IGameScene* pIGame = GetIGameInterface();
	IGameConversionManager* pGameConvMgr = GetConversionManager();
	//pGameConvMgr->SetCoordSystem(IGameConversionManager::IGAME_MAX);
	pIGame->InitialiseIGame();

	// collect usefull nodes
	m_pInterface->ProgressStart(_("Collection nodes"), TRUE, DummyFunc, NULL);
	int nNodeCount = pIGame->GetTopLevelNodeCount();
	for (int i = 0; i < nNodeCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nNodeCount);
		IGameNode* pGameNode = pIGame->GetTopLevelNode(i);
		CollectNodes(pGameNode);
	}

	// build bone info
	m_pInterface->ProgressStart(_("Build animation information"), TRUE, DummyFunc, NULL);
	BuildBonesInfo();

	// build mesh info
	m_pInterface->ProgressStart(_("Build mesh information"), TRUE, DummyFunc, NULL);
	BuildMeshsInfo();

	// build material info
	m_pInterface->ProgressStart(_("Build material information"), TRUE, DummyFunc, NULL);
	BuildMaterialsInfo();

	// save to file
	tstring strFile;
	StringUtil::GetFileNameWithoutExt(strFile, name);
	if (dlgOption.IsExportMesh()) SaveMeshFile(strFile + _(".mesh"), dlgOption.GetVertexAttributes());
	if (dlgOption.IsExportSkeleton()) SaveSkeletonFile(strFile + _(".skel"));
	SaveMaterialsFile(strFile + _(".xml"), strFile + _(".mesh"), strFile + _(".skel"));

	pIGame->ReleaseIGame();
	pIGame = NULL;

	m_pInterface->ProgressEnd();
	return TRUE;
}

void ModelExporter::Cleanup()
{
	m_vMeshNode.clear();
	m_vBoneNode.clear();
	m_vMeshData.clear();
	m_vBoneInfo.clear();
	m_vBoneInfoMap.clear();
	m_vMaterial.clear();
	m_vBoundingBoxMin.Reset(Math::FLOAT_MAX, Math::FLOAT_MAX, Math::FLOAT_MAX);
	m_vBoundingBoxMax.Reset(Math::FLOAT_MIN, Math::FLOAT_MIN, Math::FLOAT_MIN);
	m_pInterface = NULL;
}

bool ModelExporter::CollectNodes(IGameNode* pGameNode, IGameNode* pParentGameNode /* = NULL */)
{
	if (!pGameNode) return false;

	IGameObject* pGameObject = pGameNode->GetIGameObject();
	switch (pGameObject->GetIGameType())
	{
	case IGameObject::IGAME_MESH:
		{
			NODE_INFO NodeInfo;
			NodeInfo.pGameNode = pGameNode;
			NodeInfo.pParentGameNode = pParentGameNode;
			m_vMeshNode.push_back(NodeInfo);
		}
		break;
	case IGameObject::IGAME_BONE:
		{
			NODE_INFO NodeInfo;
			NodeInfo.pGameNode = pGameNode;
			NodeInfo.pParentGameNode = pParentGameNode;
			m_vBoneNode.push_back(NodeInfo);
		}
		break;
	}

	// export child
	int nChildCount = pGameNode->GetChildCount();
	for (int i = 0; i < nChildCount; ++i)
	{
		bool bOK = CollectNodes(pGameNode->GetNodeChild(i), pGameNode);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool ModelExporter::BuildBonesInfo()
{
	int nBoneCount = m_vBoneNode.size();
	for (int i = 0; i < nBoneCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nBoneCount);

		NODE_INFO& NodeInfo = m_vBoneNode[i];

		BONE_INFO BoneInfo;
		BoneInfo.strName = NodeInfo.pGameNode->GetName();
		BoneInfo.pNode = NodeInfo.pGameNode;
		BoneInfo.pParentNode = NodeInfo.pParentGameNode;
		BoneInfo.nId = (int)m_vBoneInfo.size();
		BoneInfo.nParentBoneId = FmtSkeleton::INVALID_BONE_ID;
		MaxMat2SpankMat(BoneInfo.matLocal, NodeInfo.pGameNode->GetLocalTM());
		bool bOK = DumpController(BoneInfo, NodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vBoneInfo.push_back(BoneInfo);
		m_vBoneInfoMap.insert(std::make_pair(BoneInfo.pNode, BoneInfo.nId));
	}

	int nBoneInfoCount = m_vBoneInfo.size();
	for (int i = 0; i < nBoneInfoCount; ++i)
	{
		BONE_INFO& BoneInfo = m_vBoneInfo[i];
		if (BoneInfo.pParentNode == NULL) continue;

		TM_BONE_INFO::iterator itfound = m_vBoneInfoMap.find(BoneInfo.pParentNode);
		if (itfound != m_vBoneInfoMap.end())
		{
			BoneInfo.nParentBoneId = itfound->second;
		}
		else
		{
			assert(false);
			tstring strNodeName = BoneInfo.pNode->GetName();
		}
	}

	return true;
}

bool ModelExporter::BuildMeshsInfo()
{
	int nMeshCount = m_vMeshNode.size();
	for (int i = 0; i < nMeshCount; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/nMeshCount);

		NODE_INFO& NodeInfo = m_vMeshNode[i];
		MESH_DATA MeshData;
		MeshData.strName = NodeInfo.pGameNode->GetName();
		MeshData.matLocal = NodeInfo.pGameNode->GetLocalTM();
		MeshData.pMaterial = NULL;
		MeshData.nMaterialId = -1;

		bool bOK = DumpMesh(MeshData, NodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vMeshData.push_back(MeshData);
	}

	return true;
}

bool ModelExporter::BuildMaterialsInfo()
{
	for (TV_MESH_DATA::iterator it = m_vMeshData.begin(); it != m_vMeshData.end(); ++it)
	{
		MESH_DATA& MeshData = (*it);
		IGameMaterial* pMaterial = MeshData.pMaterial;
		if (!pMaterial) continue;
		if (FindMaterial(pMaterial)) continue;

		MATERIAL Material;
		Material.pMaterial = pMaterial;
		Material.nId = m_vMaterial.size();
		int nNumTexture = pMaterial->GetNumberOfTextureMaps();
		for (int i = 0; i < nNumTexture; ++i)
		{
			IGameTextureMap* pMap = pMaterial->GetIGameTextureMap(i);
			tstring strMapFile = pMap->GetBitmapFileName();
			Material.vTextureMap.push_back(strMapFile);
		}

		m_vMaterial.push_back(Material);
		MeshData.nMaterialId = Material.nId;
	}

	return true;
}

bool ModelExporter::FindMaterial(IGameMaterial* pMaterial)
{
	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
	{
		MATERIAL& Material = (*it);
		if (pMaterial == Material.pMaterial) return true;
	}

	return false;
}

bool ModelExporter::SaveMeshFile(const tstring& strFileName, uint vertAttrs)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, strFileName.c_str(), "wb");
	if (!pFile) return false;

	// write header
	FmtMesh::FILE_HEADER Header;
	Header.nMagicNumber = FmtMesh::MAGIC_NUMBER;
	Header.nVersion = FmtMesh::CURRENT_VERSION;

	// 左右手坐标转化
	Header.fBoundingBoxMin[0] = m_vBoundingBoxMin.x;
	Header.fBoundingBoxMin[1] = m_vBoundingBoxMin.z;
	Header.fBoundingBoxMin[2] = m_vBoundingBoxMin.y;
	Header.fBoundingBoxMax[0] = m_vBoundingBoxMax.x;
	Header.fBoundingBoxMax[1] = m_vBoundingBoxMax.z;
	Header.fBoundingBoxMax[2] = m_vBoundingBoxMax.y;

	Header.nNumPieces = (int)m_vMeshData.size();
	fwrite(&Header, sizeof(Header), 1, pFile);

	// make room for piece list
	uint nPieceListPos = ftell(pFile);
	std::vector<FmtMesh::MESH_PIECE> vPiece;
	if (Header.nNumPieces > 0)
	{
		vPiece.resize(Header.nNumPieces);
		memset(&vPiece[0], 0, sizeof(FmtMesh::MESH_PIECE)*Header.nNumPieces);
		fwrite(&vPiece[0], sizeof(FmtMesh::MESH_PIECE), Header.nNumPieces, pFile);
	}

	// write mesh
	m_pInterface->ProgressStart(_("Save mesh data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < Header.nNumPieces; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/Header.nNumPieces);

		FmtMesh::MESH_PIECE& Piece = vPiece[i];
		MESH_DATA& MeshData = m_vMeshData[i];

		std::string strName;
		StringUtil::tchar2char(strName, MeshData.strName.c_str());
		strncpy_s(Piece.szName, FmtMesh::PIECE_NAME_SIZE, strName.c_str(), _TRUNCATE);

		Piece.nPieceMask = FmtMesh::PM_VISIBLE;
		Piece.nVertexAttributes = vertAttrs;
		Piece.nMaterialId = MeshData.nMaterialId;

		// write vertex data offset
		Piece.nNumVerts = (int)MeshData.vVertSlots.size();
		Piece.nOffVerts = ftell(pFile);

		// write vertex data
		int nNumVerts = (int)MeshData.vVertSlots.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX_SLOT& VertSlot = MeshData.vVertSlots[j];

			if (Piece.nVertexAttributes & FmtMesh::VA_POSITION)
			{
				// position 左右手坐标转化
				float pfPos[3] = {VertSlot.pos.x, VertSlot.pos.z, VertSlot.pos.y};
				fwrite(pfPos, sizeof(pfPos), 1, pFile);
			}

			if (Piece.nVertexAttributes & FmtMesh::VA_TEXCOORD0)
			{
				// uv
				float pfUV[2] = {VertSlot.tex.x, 1.0f - VertSlot.tex.y};
				fwrite(pfUV, sizeof(pfUV), 1, pFile);
			}

			if (Piece.nVertexAttributes & FmtMesh::VA_NORMAL)
			{
				// normal 左右手坐标转化
				float pfNormal[3] = {VertSlot.normal.x, VertSlot.normal.z, VertSlot.normal.y};
				fwrite(pfNormal, sizeof(pfNormal), 1, pFile);
			}

			if (Piece.nVertexAttributes & FmtMesh::VA_TANGENT)
			{
				// tangent 左右手坐标转化
				float pfTangent[3] = {VertSlot.tangent.x, VertSlot.tangent.z, VertSlot.tangent.y};
				fwrite(pfTangent, sizeof(pfTangent), 1, pFile);
			}

			if (Piece.nVertexAttributes & FmtMesh::VA_SKELETON)
			{
				uchar uchIndex[4] = {0};
				float pfWeight[4] = {0.0f};

				// bone weight, index
				int nSkinCount = (int)VertSlot.Skins.size();
				if (nSkinCount > 4) nSkinCount = 4;
				for (int k = 0; k < nSkinCount; ++k)
				{
					uchIndex[k] = VertSlot.Skins[k].nBoneIndex;
					pfWeight[k] = VertSlot.Skins[k].fWeight;
				}

				fwrite(uchIndex, sizeof(uchIndex), 1, pFile);
				fwrite(pfWeight, sizeof(pfWeight), 1, pFile);
			}
		}

		// write index data offset
		Piece.nNumIndis = (int)MeshData.vFaces.size()*3;
		Piece.nOffIndis = ftell(pFile);

		// write index data
		int nNumFaces = (int)MeshData.vFaces.size();
		for (int j = 0; j < nNumFaces; ++j)
		{
			//  左右手坐标转化
			ushort nIndex[3];
			nIndex[0] = (ushort)MeshData.vFaces[j].nVertIndex[0];
			nIndex[1] = (ushort)MeshData.vFaces[j].nVertIndex[2];
			nIndex[2] = (ushort)MeshData.vFaces[j].nVertIndex[1];
			fwrite(nIndex, sizeof(nIndex), 1, pFile);
		}
	}

	// really write piece list
	fseek(pFile, nPieceListPos, SEEK_SET);
	if (Header.nNumPieces > 0)
	{
		fwrite(&vPiece[0], sizeof(FmtMesh::MESH_PIECE), Header.nNumPieces, pFile);
	}

	fclose(pFile);
	return true;
}

bool ModelExporter::SaveSkeletonFile(const tstring& strFileName)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, strFileName.c_str(), "wb");
	if (!pFile) return false;

	// write header
	FmtSkeleton::FILE_HEADER Header;
	Header.nMagicNumber = FmtSkeleton::MAGIC_NUMBER;
	Header.nVersion = FmtSkeleton::CURRENT_VERSION;
	Header.nNumBones = (int)m_vBoneInfo.size();
	fwrite(&Header, sizeof(Header), 1, pFile);

	// make room for bone list
	uint nBoneListPos = ftell(pFile);
	std::vector<FmtSkeleton::BONE> vBone;
	if (Header.nNumBones > 0)
	{
		vBone.resize(Header.nNumBones);
		memset(&vBone[0], 0, sizeof(FmtSkeleton::BONE)*Header.nNumBones);
		fwrite(&vBone[0], sizeof(FmtSkeleton::BONE), Header.nNumBones, pFile);
	}

	// write bone data
	m_pInterface->ProgressStart(_("Save bone data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < Header.nNumBones; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/Header.nNumBones);

		FmtSkeleton::BONE& Bone = vBone[i];
		BONE_INFO& BoneInfo = m_vBoneInfo[i];

		// bone list info
		std::string strName;
		StringUtil::tchar2char(strName, BoneInfo.strName.c_str());
		strncpy_s(Bone.szName, FmtSkeleton::BONE_NAME_SIZE, strName.c_str(), _TRUNCATE);
		Bone.nParentIndex = BoneInfo.nParentBoneId;
		memcpy(Bone.matLocal, BoneInfo.matLocal.e, sizeof(float)*16);

		Bone.fTimeLength = 0.0f;
		Bone.nNumFrameRot = (int)BoneInfo.mapFrameRot.size();
		Bone.nNumFramePos = (int)BoneInfo.mapFramePos.size();
		Bone.nNumFrameScale = (int)BoneInfo.mapFrameScale.size();

		// frame rot
		Bone.nOffFrameRot = ftell(pFile);
		for (TM_KEYFRAME_ROT::iterator it = BoneInfo.mapFrameRot.begin(); it != BoneInfo.mapFrameRot.end(); ++it)
		{
			KEYFRAME_ROT& FrameRot = it->second;

			FmtSkeleton::FRAME_ROT Frame;
			Frame.fTime = TicksToSec(FrameRot.time);
			Frame.qRot[0] = FrameRot.qRot.x;
			Frame.qRot[1] = FrameRot.qRot.y;
			Frame.qRot[2] = FrameRot.qRot.z;
			Frame.qRot[3] = FrameRot.qRot.w;
			fwrite(&Frame, sizeof(Frame), 1, pFile);

			if (Bone.fTimeLength < Frame.fTime) Bone.fTimeLength = Frame.fTime;
		}

		// frame pos
		Bone.nOffFramePos = ftell(pFile);
		for (TM_KEYFRAME_POS::iterator it = BoneInfo.mapFramePos.begin(); it != BoneInfo.mapFramePos.end(); ++it)
		{
			KEYFRAME_POS& FramePos = it->second;

			FmtSkeleton::FRAME_POS Frame;
			Frame.fTime = TicksToSec(FramePos.time);
			Frame.vPos[0] = FramePos.vPos.x;
			Frame.vPos[1] = FramePos.vPos.y;
			Frame.vPos[2] = FramePos.vPos.z;
			fwrite(&Frame, sizeof(Frame), 1, pFile);

			if (Bone.fTimeLength < Frame.fTime) Bone.fTimeLength = Frame.fTime;
		}

		// frame scale
		Bone.nOffFrameScale = ftell(pFile);
		for (TM_KEYFRAME_SCALE::iterator it = BoneInfo.mapFrameScale.begin(); it != BoneInfo.mapFrameScale.end(); ++it)
		{
			KEYFRAME_SCALE& FrameScale = it->second;

			FmtSkeleton::FRAME_SCALE Frame;
			Frame.fTime = TicksToSec(FrameScale.time);
			Frame.vScale[0] = FrameScale.vScale.x;
			Frame.vScale[1] = FrameScale.vScale.y;
			Frame.vScale[2] = FrameScale.vScale.z;
			fwrite(&Frame, sizeof(Frame), 1, pFile);

			if (Bone.fTimeLength < Frame.fTime) Bone.fTimeLength = Frame.fTime;
		}
	}

	// really write bone list
	fseek(pFile, nBoneListPos, SEEK_SET);
	if (Header.nNumBones > 0)
	{
		fwrite(&vBone[0], sizeof(FmtSkeleton::BONE), Header.nNumBones, pFile);
	}

	fclose(pFile);
	return true;
}

bool ModelExporter::SaveMaterialsFile(const tstring& strFileName, const tstring& strMeshFile, const tstring& strSkeletonFile)
{
// 	IOEXmlDocument* pXmlDocument = g_pOEXmlMgr->CreateDocument();
// 	if (!pXmlDocument) return false;
// 
// 	IOEXmlNode* pXmlModel = pXmlDocument->InsertRootNode(TS("Model"));
// 	IOEXmlNode* pXmlRenderData = pXmlModel->InsertChild(TS("RenderData"));
// 
// 	if (m_pDlgModelExpOption->IsExportMesh())
// 	{
// 		IOEXmlNode* pXmlMesh = pXmlRenderData->InsertChild(TS("Mesh"));
// 		tstring strName;
// 		COEOS::GetFileNameWithExt(strName, strMeshFile);
// 		pXmlMesh->SetText(strName);
// 	}
// 
// 	if (m_pDlgModelExpOption->IsExportSkelecton())
// 	{
// 		IOEXmlNode* pXmlSkeleton = pXmlRenderData->InsertChild(TS("Skeleton"));
// 		tstring strName;
// 		COEOS::GetFileNameWithExt(strName, strSkeletonFile);
// 		pXmlSkeleton->SetText(strName);
// 	}
// 
// 	IOEXmlNode* pXmlMaterials = pXmlRenderData->InsertChild(TS("Materials"));
// 	pXmlMaterials->SetAttribute(TS("count"), (int)m_vMaterial.size());
// 	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
// 	{
// 		MATERIAL& Material = (*it);
// 		IOEXmlNode* pXmlMaterial = pXmlMaterials->InsertChild(TS("Material"));
// 		pXmlMaterial->SetAttribute(TS("id"), Material.nID);
// 		pXmlMaterial->SetAttribute(TS("vertdecl"), m_pDlgModelExpOption->GetVertexFlag());
// 		pXmlMaterial->SetAttribute(TS("shader"), EMPTY_STRING);
// 
// 		int nTexIndex = 0;
// 		for (TV_STRING::iterator ittex = Material.vTextureMap.begin(); ittex != Material.vTextureMap.end(); ++ittex)
// 		{
// 			tstring& strMap = (*ittex);
// 			tstring strName;
// 			COEOS::GetFileNameWithExt(strName, strMap);
// 
// 			const tstring& strKey = GetTextureKey((MATERIAL_TEXTURE_TYPE)nTexIndex);
// 			pXmlMaterial->SetAttribute(strKey, strName);
// 			++nTexIndex;
// 		}
// 	}
// 
// 	pXmlDocument->SaveFile(strFileName);
// 	SAFE_RELEASE(pXmlDocument);

	return true;
}

const tstring& ModelExporter::GetTextureKey(MATERIAL_TEXTURE_TYPE eType)
{
	static const tstring s_EmptyTexture;
	static const tstring s_TextureKey[MTT_NUM] =
	{
		_("texdiffuse"),
		_("texnormal"),
		_("texture2"),
		_("texture3"),
		_("texture4"),
		_("texture5"),
		_("texture6"),
		_("texture7"),
	};
	assert(MTT_NUM == 8);
	if (eType < 0 || eType >= MTT_NUM) return s_EmptyTexture;
	return s_TextureKey[eType];
}

bool ModelExporter::DumpMesh(MESH_DATA& MeshDataOut, IGameNode* pGameNode)
{
	IGameObject* pGameObject = pGameNode->GetIGameObject();
	if (pGameObject->GetIGameType() != IGameObject::IGAME_MESH)
	{
		assert(false);
		return false;
	}

	IGameMesh* pGameMesh = (IGameMesh*)pGameObject;

	if (!pGameMesh->InitializeData())
	{
		assert(false);
		return false;
	}

	// init vertex slots
	int nNumVerts = pGameMesh->GetNumberOfVerts();
	VERTEX_SLOT EmptySlot;
	EmptySlot.bUsed = false;
	EmptySlot.nVertIndex = 0;
	EmptySlot.nTexIndex = 0;
	EmptySlot.vClone.clear();
	EmptySlot.pos.Set(0.0f, 0.0f, 0.0f);
	EmptySlot.tex.Set(0.0f, 0.0f);
	EmptySlot.Skins.clear();

	// make room for verts
	for (int i = 0; i < nNumVerts; ++i)
	{
		MeshDataOut.vVertSlots.push_back(EmptySlot);
	}

	// faces
	int nNumFaces = pGameMesh->GetNumberOfFaces();
	if (nNumFaces > 0)
	{
		FaceEx* pFace = pGameMesh->GetFace(0);
		MeshDataOut.pMaterial = pGameMesh->GetMaterialFromFace(pFace);
	}

	for (int i = 0; i < nNumFaces; ++i)
	{
		FaceEx* pFace = pGameMesh->GetFace(i);

		FACE Face;

		for (int j = 0; j < 3; ++j)
		{
			uint nVertIndex = pFace->vert[j];
			uint nTexIndex = pFace->texCoord[j];

			if (MeshDataOut.vVertSlots[nVertIndex].bUsed)
			{
				bool bAddNew = true;
				if (MeshDataOut.vVertSlots[nVertIndex].nTexIndex == nTexIndex)
				{
					bAddNew = false;
				}
				else
				{
					TV_INT& vClone = MeshDataOut.vVertSlots[nVertIndex].vClone;
					for (TV_INT::iterator it = vClone.begin(); it != vClone.end(); ++it)
					{
						if (MeshDataOut.vVertSlots[*it].nTexIndex == nTexIndex)
						{
							bAddNew = false;
							nVertIndex = (*it);
							break;
						}
					}
				}

				MeshDataOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				MeshDataOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				if (bAddNew)
				{
					// add new slot
					int nNewVertIndex = MeshDataOut.vVertSlots.size();
					MeshDataOut.vVertSlots.push_back(EmptySlot);
					MeshDataOut.vVertSlots[nNewVertIndex].bUsed = true;
					MeshDataOut.vVertSlots[nNewVertIndex].nVertIndex = nVertIndex;
					MeshDataOut.vVertSlots[nNewVertIndex].nTexIndex = nTexIndex;

					MeshDataOut.vVertSlots[nVertIndex].vClone.push_back(nNewVertIndex);
					nVertIndex = nNewVertIndex;
				}
			}
			else
			{
				MeshDataOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				MeshDataOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				// set this slot
				MeshDataOut.vVertSlots[nVertIndex].bUsed = true;
				MeshDataOut.vVertSlots[nVertIndex].nVertIndex = nVertIndex;
				MeshDataOut.vVertSlots[nVertIndex].nTexIndex = nTexIndex;
			}

			Face.nVertIndex[j] = nVertIndex;
		}

		MeshDataOut.vFaces.push_back(Face);
	}

	// setup vertex data
	for (int i = 0; i < nNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = MeshDataOut.vVertSlots[i];

		int nNumNormal = 0;
		Point3 vNormal(0.0f, 0.0f, 0.0f);
		for (TS_INT::iterator it = LocalSlot.vNormalIndex.begin(); it != LocalSlot.vNormalIndex.end(); ++it)
		{
			vNormal += pGameMesh->GetNormal(*it, false);
			++nNumNormal;
		}

		assert(nNumNormal);
		LocalSlot.normal = vNormal.Normalize();

		int nNumTangent = 0;
		Point3 vTangent(0.0f, 0.0f, 0.0f);
		for (TS_INT::iterator it = LocalSlot.vTangentIndex.begin(); it != LocalSlot.vTangentIndex.end(); ++it)
		{
			vTangent += pGameMesh->GetTangent(*it);
			++nNumTangent;
		}

		assert(nNumTangent);
		LocalSlot.tangent = vTangent.Normalize();
	}

	int nNewNumVerts = MeshDataOut.vVertSlots.size();
	for (int i = 0; i < nNewNumVerts; ++i)
	{
		VERTEX_SLOT& LocalSlot = MeshDataOut.vVertSlots[i];

		// get position
		pGameMesh->GetVertex(LocalSlot.nVertIndex, LocalSlot.pos, false);

		if (LocalSlot.pos.x < m_vBoundingBoxMin.x) m_vBoundingBoxMin.x = LocalSlot.pos.x;
		if (LocalSlot.pos.x > m_vBoundingBoxMax.x) m_vBoundingBoxMax.x = LocalSlot.pos.x;

		if (LocalSlot.pos.y < m_vBoundingBoxMin.y) m_vBoundingBoxMin.y = LocalSlot.pos.y;
		if (LocalSlot.pos.y > m_vBoundingBoxMax.y) m_vBoundingBoxMax.y = LocalSlot.pos.y;

		if (LocalSlot.pos.z < m_vBoundingBoxMin.z) m_vBoundingBoxMin.z = LocalSlot.pos.z;
		if (LocalSlot.pos.z > m_vBoundingBoxMax.z) m_vBoundingBoxMax.z = LocalSlot.pos.z;

		// get normal and tangent
		if (i != LocalSlot.nVertIndex)
		{
			LocalSlot.normal = MeshDataOut.vVertSlots[LocalSlot.nVertIndex].normal;
			LocalSlot.tangent = MeshDataOut.vVertSlots[LocalSlot.nVertIndex].tangent;
		}

		// get uv
		pGameMesh->GetTexVertex(LocalSlot.nTexIndex, LocalSlot.tex);
	}

	// skins
	int nNumModifier = pGameObject->GetNumModifiers();
	for (int i = 0; i < nNumModifier; ++i)
	{
		IGameModifier* pGameModifier = pGameObject->GetIGameModifier(i);
		if (!pGameModifier->IsSkin()) continue;
		IGameSkin* pGameSkin = (IGameSkin*)pGameModifier;

		bool bOK = DumpSkin(MeshDataOut, pGameSkin);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool ModelExporter::DumpSkin(MESH_DATA& MeshDataOut, IGameSkin* pGameSkin)
{
	int nNumSkinVerts = pGameSkin->GetNumOfSkinnedVerts();

	for (int i = 0; i < nNumSkinVerts; ++i)
	{
		int nNumBone = pGameSkin->GetNumberOfBones(i);
		for (int j = 0; j < nNumBone; ++j)
		{
			IGameNode* pBoneNode = pGameSkin->GetIGameBone(i, j);
			TM_BONE_INFO::iterator it = m_vBoneInfoMap.find(pBoneNode);
			if (it == m_vBoneInfoMap.end())
			{
				assert(false);
				continue;
			}

			SKIN Skin;
			Skin.nBoneIndex = it->second;
			Skin.fWeight = pGameSkin->GetWeight(i, j);
			MeshDataOut.vVertSlots[i].Skins.push_back(Skin);
		}

		SortSkin(MeshDataOut.vVertSlots[i].Skins);
	}

	int nNumAdditionVerts = MeshDataOut.vVertSlots.size();
	for (int i = nNumSkinVerts; i < nNumAdditionVerts; ++i)
	{
		int nOldIndex = MeshDataOut.vVertSlots[i].nVertIndex;
		MeshDataOut.vVertSlots[i].Skins = MeshDataOut.vVertSlots[nOldIndex].Skins;
	}

	return true;
}

bool ModelExporter::DumpController(BONE_INFO& BoneInfo, IGameNode* pGameNode)
{
	IGameControl* pGameControl = pGameNode->GetIGameControl();

	bool bBiped = false;

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS))
	{
		if (pGameControl->GetControlType(IGAME_POS) == IGameControl::IGAME_BIPED)
		{
			// dump sample keys
			DumpSampleKey(BoneInfo, pGameControl, IGAME_TM);
			bBiped = true;
		}
		else
		{
			DumpPositionController(BoneInfo, pGameControl);
		}
	}

	// export rotation controller
	if (pGameControl->IsAnimated(IGAME_ROT))
	{
		if (pGameControl->GetControlType(IGAME_ROT) == IGameControl::IGAME_BIPED)
		{
			if (!bBiped)
			{
				// dump sample keys
				DumpSampleKey(BoneInfo, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpRotationController(BoneInfo, pGameControl);
		}
	}

	// export scale controller
	if (pGameControl->IsAnimated(IGAME_SCALE))
	{
		if (pGameControl->GetControlType(IGAME_SCALE) == IGameControl::IGAME_BIPED)
		{
			if (!bBiped)
			{
				// dump sample keys
				DumpSampleKey(BoneInfo, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpScaleController(BoneInfo, pGameControl);
		}
	}

	for (TM_KEYFRAME_ROT::iterator it = BoneInfo.mapFrameRot.begin(); it != BoneInfo.mapFrameRot.end(); ++it)
	{
		KEYFRAME_ROT& KeyFrame = it->second;

		if (KeyFrame.nMask & KFM_ROT)
		{
			if (KeyFrame.nMask & KFM_QUAT)
			{
				// TODO: logout
				assert(false);
			}

			Math::BuildQuaternionFromEulerXYZ(KeyFrame.qRot, KeyFrame.vRot.x, KeyFrame.vRot.y, KeyFrame.vRot.z);
		}
		else if (KeyFrame.nMask & KFM_QUAT)
		{
			if (KeyFrame.nMask & KFM_ROT)
			{
				// TODO: logout
				assert(false);
			}
		}
	}

	return true;
}

bool ModelExporter::DumpPositionController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_POS);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std pos key
			DumpMaxStdPosKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// export independent controller
			DumpIndependentPosKey(BoneInfo, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool ModelExporter::DumpRotationController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std rot key
			DumpMaxStdRotKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// export Euler controller
			DumpEulerRotKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(BoneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(BoneInfo, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool ModelExporter::DumpScaleController(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_SCALE);

	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export scale key
			DumpMaxStdScaleKey(BoneInfo, pGameControl);
		}
		break;
	default:
		{
			// TODO: 
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool ModelExporter::DumpMaxStdPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS, PosKey[i].bezierKey.pval);
		}
	}
	else if (pGameControl->GetLinearKeys(PosKey, IGAME_POS))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS, PosKey[i].linearKey.pval);
		}
	}

	return true;
}

bool ModelExporter::DumpIndependentPosKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab PosKey;
	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_X))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_X, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_X))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_X, PosKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Y))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Y, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Y))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Y, PosKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(PosKey, IGAME_POS_Z))
	{
		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Z, PosKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(PosKey, IGAME_POS_Z))
	{
		int nCount = PosKey.Count();

		for (int i = 0; i < PosKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, PosKey[i].t, KFM_POS_Z, PosKey[i].linearKey.fval);
		}
	}

	return true;
}

bool ModelExporter::DumpMaxStdRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;
	if (pGameControl->GetBezierKeys(RotKey, IGAME_ROT))
	{
		// export Bezier Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, RotKey[i].bezierKey.qval);
		}
	}
	else if (pGameControl->GetLinearKeys(RotKey, IGAME_ROT))
	{
		// export Linear Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, RotKey[i].linearKey.qval);
		}
	}
	else if (pGameControl->GetTCBKeys(RotKey, IGAME_ROT))
	{
		// export TCB Keys
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			Quat qValue = QFromAngAxis(RotKey[i].tcbKey.aval.angle, RotKey[i].tcbKey.aval.axis);
			InsertKeyFrame(BoneInfo, RotKey[i].t, qValue);
		}
	}

	return true;
}

bool ModelExporter::DumpEulerRotKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab RotKey;

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_X, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_X, RotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Y, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Y, RotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(RotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Z, RotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(RotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < RotKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, RotKey[i].t, KFM_ROT_Z, RotKey[i].linearKey.fval);
		}
	}

	return true;
}

bool ModelExporter::DumpMaxStdScaleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	IGameKeyTab ScaleKey;
	if (pGameControl->GetBezierKeys(ScaleKey, IGAME_SCALE))
	{
		for (int i = 0; i < ScaleKey.Count(); ++i)
		{
			InsertKeyFrame(BoneInfo, ScaleKey[i].t, KFM_SCALE, ScaleKey[i].bezierKey.sval.s);
		}
	}

	return true;
}

bool ModelExporter::DumpConstraintKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	// TODO: 
	assert(false);

	//IGameConstraint* pGameConstraint = pGameControl->GetConstraint(IGAME_POS);

	//int nConstraintCount = pGameConstraint->NumberOfConstraintNodes();
	//for(int i = 0; i < nConstraintCount; ++i)
	//{
	//	float fWeight = pGameConstraint->GetConstraintWeight(i);
	//	int nNodeID = pGameConstraint->GetConstraintNodes(i)->GetNodeID();

	//	// TODO: export constraint
	//}

	//IPropertyContainer* pPropertyContainer = pGameConstraint->GetIPropertyContainer();
	//int nPropertyCount = pPropertyContainer->GetNumberOfProperties();

	//for(int i = 0; i < nPropertyCount; ++i)
	//{
	//	// TODO: dump property
	//}

	return true;
}

bool ModelExporter::DumpListKey(BONE_INFO& BoneInfo, IGameControl* pGameControl)
{
	int nCount = pGameControl->GetNumOfListSubControls(IGAME_POS);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_POS);
		bool bOK = DumpPositionController(BoneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_ROT);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_ROT);
		bool bOK = DumpRotationController(BoneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_SCALE);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_SCALE);
		bool bOK = DumpScaleController(BoneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool ModelExporter::DumpSampleKey(BONE_INFO& BoneInfo, IGameControl* pGameControl, IGameControlType eType)
{
	IGameKeyTab Keys;
	if (pGameControl->GetFullSampledKeys(Keys, 1, eType, true))
	{
		for (int i = 0; i < Keys.Count(); ++i)
		{
			switch (eType)
			{
			case IGAME_TM:
				{
					InsertKeyFrame(BoneInfo, Keys[i].t, Keys[i].sampleKey.gval);
				}
				break;
			default:
				{
					assert(false);
				}
				break;
			}
		}

		return true;
	}

	return false;
}

ModelExporter::KEYFRAME_POS* ModelExporter::GetKeyFrame(TM_KEYFRAME_POS& FramePos, TimeValue time)
{
	KEYFRAME_POS* pKeyFrame = NULL;

	TM_KEYFRAME_POS::iterator itfound = FramePos.find(time);
	if (itfound != FramePos.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_POS EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		FramePos.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = FramePos.find(time);
		assert(itfound != FramePos.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

ModelExporter::KEYFRAME_SCALE* ModelExporter::GetKeyFrame(TM_KEYFRAME_SCALE& FrameScale, TimeValue time)
{
	KEYFRAME_SCALE* pKeyFrame = NULL;

	TM_KEYFRAME_SCALE::iterator itfound = FrameScale.find(time);
	if (itfound != FrameScale.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_SCALE EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		EmptyKeyFrame.vScale.Reset(1.0f, 1.0f, 1.0f);
		FrameScale.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = FrameScale.find(time);
		assert(itfound != FrameScale.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

ModelExporter::KEYFRAME_ROT* ModelExporter::GetKeyFrame(TM_KEYFRAME_ROT& FrameRot, TimeValue time)
{
	KEYFRAME_ROT* pKeyFrame = NULL;

	TM_KEYFRAME_ROT::iterator itfound = FrameRot.find(time);
	if (itfound != FrameRot.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_ROT EmptyKeyFrame;
		EmptyKeyFrame.time = time;
		EmptyKeyFrame.nMask = KFM_UNKNOWN;
		FrameRot.insert(std::make_pair(EmptyKeyFrame.time, EmptyKeyFrame));

		itfound = FrameRot.find(time);
		assert(itfound != FrameRot.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

bool ModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, float fValue)
{
	switch (eMask)
	{
	case KFM_POS_X:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.mapFramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vPos.x = fValue;			// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_POS_Y:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.mapFramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vPos.z = fValue;			// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_POS_Z:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.mapFramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vPos.y = fValue;			// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE_X:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vScale.x = fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE_Y:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vScale.z = fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE_Z:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vScale.y = fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT_X:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vRot.x = -fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT_Y:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vRot.z = -fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT_Z:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			pKeyFrame->vRot.y = -fValue;		// 左右手坐标转换
			pKeyFrame->nMask |= eMask;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool ModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue)
{
	switch (eMask)
	{
	case KFM_POS:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(BoneInfo.mapFramePos, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			MaxVec2SpankVec(pKeyFrame->vPos, vValue);
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_SCALE:
		{
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameScale, time);
			if (pKeyFrame->nMask & eMask)
			{
				// TODO: logout
				assert(false);
			}
			MaxVec2SpankVec(pKeyFrame->vScale, vValue);
			pKeyFrame->nMask |= eMask;
		}
		break;
	case KFM_ROT:
		{
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameRot, time);
			if (pKeyFrame->nMask & (eMask | KFM_QUAT))
			{
				// TODO: logout
				assert(false);
			}
			MaxEular2SpankEular(pKeyFrame->vRot, vValue);
			pKeyFrame->nMask |= eMask;
		}
		break;
	default:
		{
			assert(false);
			return false;
		}
		break;
	}

	return true;
}

bool ModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const Quat& qValue)
{
	KEYFRAME_ROT* pKeyFrame = GetKeyFrame(BoneInfo.mapFrameRot, time);
	if (pKeyFrame->nMask & KFM_ROT)
	{
		// TODO: logout
		assert(false);
	}

	MaxQuat2SpankQuat(pKeyFrame->qRot, qValue);
	pKeyFrame->nMask |= KFM_QUAT;

	return true;
}

bool ModelExporter::InsertKeyFrame(BONE_INFO& BoneInfo, TimeValue time, const GMatrix& matValue)
{
	KEYFRAME_POS* pKeyFramePos = GetKeyFrame(BoneInfo.mapFramePos, time);
	if (pKeyFramePos->nMask & KFM_POS)
	{
		// TODO: logout
		assert(false);
	}

	KEYFRAME_SCALE* pKeyFrameScale = GetKeyFrame(BoneInfo.mapFrameScale, time);
	if (pKeyFrameScale->nMask & KFM_SCALE)
	{
		// TODO: logout
		assert(false);
	}

	KEYFRAME_ROT* pKeyFrameRot = GetKeyFrame(BoneInfo.mapFrameRot, time);
	if (pKeyFrameRot->nMask & (KFM_ROT | KFM_QUAT))
	{
		// TODO: logout
		assert(false);
	}

	Matrix4x4 matTrans;
	MaxMat2SpankMat(matTrans, matValue);

	Math::GetMatrixTranslate(pKeyFramePos->vPos, matTrans);
	Math::GetMatrixScale(pKeyFrameScale->vScale, matTrans);
	Math::BuildQuaternionFromMatrix(pKeyFrameRot->qRot, matTrans);
	pKeyFrameScale->nMask |= (KFM_SCALE | KFM_POS | KFM_QUAT);

	return true;
}

void ModelExporter::MaxMat2SpankMat(Matrix4x4& matOut, const GMatrix& matIn)
{
	matOut.e[0] = matIn[0][0];
	matOut.e[1] = matIn[0][2];
	matOut.e[2] = matIn[0][1];
	matOut.e[3] = 0.0f;

	matOut.e[4] = matIn[2][0];
	matOut.e[5] = matIn[2][2];
	matOut.e[6] = matIn[2][1];
	matOut.e[7] = 0.0f;

	matOut.e[8] = matIn[1][0];
	matOut.e[9] = matIn[1][2];
	matOut.e[10] = matIn[1][1];
	matOut.e[11] = 0.0f;

	matOut.e[12] = matIn[3][0];
	matOut.e[13] = matIn[3][2];
	matOut.e[14] = matIn[3][1];
	matOut.e[15] = 1.0f;
}

void ModelExporter::SpankMat2MaxMat(GMatrix& matOut, const Matrix4x4& matIn)
{
	matOut[0][0] = matIn.e[0];
	matOut[0][1] = matIn.e[2];
	matOut[0][2] = matIn.e[1];
	matOut[0][3] = 0.0f;

	matOut[1][0] = matIn.e[8];
	matOut[1][1] = matIn.e[10];
	matOut[1][2] = matIn.e[9];
	matOut[1][3] = 0.0f;

	matOut[2][0] = matIn.e[4];
	matOut[2][1] = matIn.e[6];
	matOut[2][2] = matIn.e[5];
	matOut[2][3] = 0.0f;

	matOut[3][0] = matIn.e[12];
	matOut[3][1] = matIn.e[14];
	matOut[3][2] = matIn.e[13];
	matOut[3][3] = 1.0f;
}

void ModelExporter::MaxQuat2SpankQuat(Quaternion& qOut, const Quat& qIn)
{
	qOut.x = -qIn.x;
	qOut.y = -qIn.z;
	qOut.z = -qIn.y;
	qOut.w = qIn.w;
}

void ModelExporter::SpankQuat2MaxQuat(Quat& qOut, const Quaternion& qIn)
{
	qOut.x = -qIn.x;
	qOut.y = -qIn.z;
	qOut.z = -qIn.y;
	qOut.w = qIn.w;
}

void ModelExporter::MaxVec2SpankVec(Vector3& vOut, const Point3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void ModelExporter::SpankVec2MaxVec(Point3& vOut, const Vector3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void ModelExporter::MaxEular2SpankEular(Vector3& vOut, const Point3& vIn)
{
	vOut.x = -vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void ModelExporter::SpankEular2MaxEular(Point3& vOut, const Vector3& vIn)
{
	vOut.x = -vIn.x;
	vOut.y = vIn.z;
	vOut.z = vIn.y;
}

void ModelExporter::SortSkin(TV_SKIN& vSkin)
{
	int nNumSkins = vSkin.size();

	for (int i = 0; i < nNumSkins-1; ++i)
	{
		int nChoose = i;
		for (int j = i+1; j < nNumSkins; ++j)
		{
			if (vSkin[j].fWeight > vSkin[nChoose].fWeight) nChoose = j;
		}

		if (nChoose != i)
		{
			SKIN Skin = vSkin[i];
			vSkin[i] = vSkin[nChoose];
			vSkin[nChoose] = Skin;
		}
	}
}
