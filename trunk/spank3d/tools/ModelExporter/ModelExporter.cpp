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
#include <tinyxml-2.6.2/tinyxml.h>
#include <algorithm>

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
	SaveMaterialsFile(strFile + _(".xml"), strFile + _(".mesh"), strFile + _(".skel"), dlgOption.GetVertexAttributes());

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

	m_vBoundingBoxMin.Set(Math::FLOAT_MAX, Math::FLOAT_MAX, Math::FLOAT_MAX);
	m_vBoundingBoxMax.Set(Math::FLOAT_MIN, Math::FLOAT_MIN, Math::FLOAT_MIN);

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
			NODE_INFO nodeInfo;
			nodeInfo.pGameNode = pGameNode;
			nodeInfo.pParentGameNode = pParentGameNode;
			m_vMeshNode.push_back(nodeInfo);
		}
		break;
	case IGameObject::IGAME_BONE:
		{
			NODE_INFO nodeInfo;
			nodeInfo.pGameNode = pGameNode;
			nodeInfo.pParentGameNode = pParentGameNode;
			m_vBoneNode.push_back(nodeInfo);
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

		NODE_INFO& nodeInfo = m_vBoneNode[i];

		BONE_INFO boneInfo;
		boneInfo.strName = nodeInfo.pGameNode->GetName();
		boneInfo.pNode = nodeInfo.pGameNode;
		boneInfo.pParentNode = nodeInfo.pParentGameNode;
		boneInfo.nId = (int)m_vBoneInfo.size();
		boneInfo.nParentBoneId = FmtSkeleton::INVALID_BONE_ID;
		MaxMat2SpankMat(boneInfo.matLocal, nodeInfo.pGameNode->GetLocalTM());
		bool bOK = DumpController(boneInfo, nodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vBoneInfo.push_back(boneInfo);
		m_vBoneInfoMap.insert(std::make_pair(boneInfo.pNode, boneInfo.nId));
	}

	int nBoneInfoCount = m_vBoneInfo.size();
	for (int i = 0; i < nBoneInfoCount; ++i)
	{
		BONE_INFO& boneInfo = m_vBoneInfo[i];
		if (boneInfo.pParentNode == NULL) continue;

		TM_BONE_INFO::iterator itfound = m_vBoneInfoMap.find(boneInfo.pParentNode);
		if (itfound != m_vBoneInfoMap.end())
		{
			boneInfo.nParentBoneId = itfound->second;
		}
		else
		{
			assert(false);
			tstring strNodeName = boneInfo.pNode->GetName();
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

		NODE_INFO& nodeInfo = m_vMeshNode[i];
		MESH_DATA meshData;
		meshData.strName = nodeInfo.pGameNode->GetName();
		meshData.matLocal = nodeInfo.pGameNode->GetLocalTM();
		meshData.pMaterial = NULL;
		meshData.nMaterialId = -1;

		bool bOK = DumpMesh(meshData, nodeInfo.pGameNode);
		assert(bOK);
		// TODO: check bOK
		m_vMeshData.push_back(meshData);
	}

	return true;
}

bool ModelExporter::BuildMaterialsInfo()
{
	for (TV_MESH_DATA::iterator it = m_vMeshData.begin(); it != m_vMeshData.end(); ++it)
	{
		MESH_DATA& meshData = (*it);
		IGameMaterial* pMaterial = meshData.pMaterial;
		if (!pMaterial) continue;
		if (FindMaterial(pMaterial)) continue;

		MATERIAL material;
		material.pMaterial = pMaterial;
		material.nId = m_vMaterial.size();
		int nNumTexture = pMaterial->GetNumberOfTextureMaps();
		for (int i = 0; i < nNumTexture; ++i)
		{
			IGameTextureMap* pMap = pMaterial->GetIGameTextureMap(i);
			tstring strMapFile = pMap->GetBitmapFileName();
			material.vTextureMap.push_back(strMapFile);
		}

		m_vMaterial.push_back(material);
		meshData.nMaterialId = material.nId;
	}

	return true;
}

bool ModelExporter::FindMaterial(IGameMaterial* pMaterial)
{
	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
	{
		MATERIAL& material = (*it);
		if (pMaterial == material.pMaterial) return true;
	}

	return false;
}

bool ModelExporter::SaveMeshFile(const tstring& strFileName, uint vertAttrs)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, strFileName.c_str(), "wb");
	if (!pFile) return false;

	// write header
	FmtMesh::FILE_HEADER header;
	header.nMagicNumber = FmtMesh::MAGIC_NUMBER;
	header.nVersion = FmtMesh::CURRENT_VERSION;

	MaxVec2SpankVec(header.fBoundingBoxMin, m_vBoundingBoxMin);
	MaxVec2SpankVec(header.fBoundingBoxMin, m_vBoundingBoxMax);

	header.nNumPieces = (int)m_vMeshData.size();
	fwrite(&header, sizeof(header), 1, pFile);

	// make room for piece list
	uint nPieceListPos = ftell(pFile);
	std::vector<FmtMesh::MESH_PIECE> vPiece;
	if (header.nNumPieces > 0)
	{
		vPiece.resize(header.nNumPieces);
		memset(&vPiece[0], 0, sizeof(FmtMesh::MESH_PIECE)*header.nNumPieces);
		fwrite(&vPiece[0], sizeof(FmtMesh::MESH_PIECE), header.nNumPieces, pFile);
	}

	// write mesh
	m_pInterface->ProgressStart(_("Save mesh data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < header.nNumPieces; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/header.nNumPieces);

		FmtMesh::MESH_PIECE& piece = vPiece[i];
		MESH_DATA& meshData = m_vMeshData[i];

		std::string strName;
		StringUtil::tchar2char(strName, meshData.strName.c_str());
		strncpy_s(piece.szName, FmtMesh::PIECE_NAME_SIZE, strName.c_str(), _TRUNCATE);

		piece.nPieceMask = FmtMesh::PM_VISIBLE;
		piece.nVertexAttributes = vertAttrs;
		piece.nMaterialId = meshData.nMaterialId;

		// write vertex data offset
		piece.nNumVerts = (int)meshData.vVertSlots.size();
		piece.nOffVerts = ftell(pFile);

		// write vertex data
		int nNumVerts = (int)meshData.vVertSlots.size();
		for (int j = 0; j < nNumVerts; ++j)
		{
			VERTEX_SLOT& vertSlot = meshData.vVertSlots[j];

			if (piece.nVertexAttributes & FmtMesh::VA_POSITION)
			{
				// position
				float pfPos[3];
				MaxVec2SpankVec(pfPos, vertSlot.pos);
				fwrite(pfPos, sizeof(pfPos), 1, pFile);
			}

			if (piece.nVertexAttributes & FmtMesh::VA_TEXCOORD0)
			{
				// uv
				float pfUV[2];
				MaxUV2SpankUV(pfUV, vertSlot.tex);
				fwrite(pfUV, sizeof(pfUV), 1, pFile);
			}

			if (piece.nVertexAttributes & FmtMesh::VA_NORMAL)
			{
				// normal
				float pfNormal[3];
				MaxVec2SpankVec(pfNormal, vertSlot.normal);
				fwrite(pfNormal, sizeof(pfNormal), 1, pFile);
			}

			if (piece.nVertexAttributes & FmtMesh::VA_TANGENT)
			{
				// tangent
				float pfTangent[3];
				MaxVec2SpankVec(pfTangent, vertSlot.tangent);
				fwrite(pfTangent, sizeof(pfTangent), 1, pFile);
			}

			if (piece.nVertexAttributes & FmtMesh::VA_SKELETON)
			{
				uchar uchIndex[4] = {0};
				float pfWeight[4] = {0.0f};

				// bone weight, index
				int nSkinCount = (int)vertSlot.Skins.size();
				if (nSkinCount > 4) nSkinCount = 4;
				for (int k = 0; k < nSkinCount; ++k)
				{
					uchIndex[k] = vertSlot.Skins[k].nBoneIndex;
					pfWeight[k] = vertSlot.Skins[k].fWeight;
				}

				fwrite(uchIndex, sizeof(uchIndex), 1, pFile);
				fwrite(pfWeight, sizeof(pfWeight), 1, pFile);
			}
		}

		// write index data offset
		piece.nNumIndis = (int)meshData.vFaces.size()*3;
		piece.nOffIndis = ftell(pFile);

		// write index data
		int nNumFaces = (int)meshData.vFaces.size();
		for (int j = 0; j < nNumFaces; ++j)
		{
			ushort nIndex[3];
			MaxFaceIndex2SpankFaceIndex(nIndex, meshData.vFaces[j].nVertIndex);
			fwrite(nIndex, sizeof(nIndex), 1, pFile);
		}

		// set size info
		piece.nSizeOfVerts = piece.nOffIndis - piece.nOffVerts;
		piece.nSizeOfIndis = piece.nNumIndis * sizeof(ushort);
	}

	// really write piece list
	fseek(pFile, nPieceListPos, SEEK_SET);
	if (header.nNumPieces > 0)
	{
		fwrite(&vPiece[0], sizeof(FmtMesh::MESH_PIECE), header.nNumPieces, pFile);
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
	FmtSkeleton::FILE_HEADER header;
	header.nMagicNumber = FmtSkeleton::MAGIC_NUMBER;
	header.nVersion = FmtSkeleton::CURRENT_VERSION;
	header.nNumBones = (int)m_vBoneInfo.size();
	fwrite(&header, sizeof(header), 1, pFile);

	// make room for bone list
	uint nBoneListPos = ftell(pFile);
	std::vector<FmtSkeleton::BONE> vBone;
	if (header.nNumBones > 0)
	{
		vBone.resize(header.nNumBones);
		memset(&vBone[0], 0, sizeof(FmtSkeleton::BONE)*header.nNumBones);
		fwrite(&vBone[0], sizeof(FmtSkeleton::BONE), header.nNumBones, pFile);
	}

	// write bone data
	m_pInterface->ProgressStart(_("Save bone data"), TRUE, DummyFunc, NULL);
	for (int i = 0; i < header.nNumBones; ++i)
	{
		m_pInterface->ProgressUpdate(i*100/header.nNumBones);

		FmtSkeleton::BONE& bone = vBone[i];
		BONE_INFO& boneInfo = m_vBoneInfo[i];

		// bone list info
		std::string strName;
		StringUtil::tchar2char(strName, boneInfo.strName.c_str());
		strncpy_s(bone.szName, FmtSkeleton::BONE_NAME_SIZE, strName.c_str(), _TRUNCATE);
		bone.nParentIndex = boneInfo.nParentBoneId;
		memcpy(bone.matLocal, boneInfo.matLocal.e, sizeof(float)*16);

		bone.fTimeLength = 0.0f;
		bone.nNumFrameRot = (int)boneInfo.mapFrameRot.size();
		bone.nNumFramePos = (int)boneInfo.mapFramePos.size();
		bone.nNumFrameScale = (int)boneInfo.mapFrameScale.size();

		// frame rot
		bone.nOffFrameRot = ftell(pFile);
		for (TM_KEYFRAME_ROT::iterator it = boneInfo.mapFrameRot.begin(); it != boneInfo.mapFrameRot.end(); ++it)
		{
			KEYFRAME_ROT& frameRot = it->second;

			FmtSkeleton::FRAME_ROT frame;
			frame.fTime = TicksToSec(frameRot.time);
			frame.qRot[0] = frameRot.qRot.x;
			frame.qRot[1] = frameRot.qRot.y;
			frame.qRot[2] = frameRot.qRot.z;
			frame.qRot[3] = frameRot.qRot.w;
			fwrite(&frame, sizeof(frame), 1, pFile);

			if (bone.fTimeLength < frame.fTime) bone.fTimeLength = frame.fTime;
		}

		// frame pos
		bone.nOffFramePos = ftell(pFile);
		for (TM_KEYFRAME_POS::iterator it = boneInfo.mapFramePos.begin(); it != boneInfo.mapFramePos.end(); ++it)
		{
			KEYFRAME_POS& framePos = it->second;

			FmtSkeleton::FRAME_POS frame;
			frame.fTime = TicksToSec(framePos.time);
			frame.vPos[0] = framePos.vPos.x;
			frame.vPos[1] = framePos.vPos.y;
			frame.vPos[2] = framePos.vPos.z;
			fwrite(&frame, sizeof(frame), 1, pFile);

			if (bone.fTimeLength < frame.fTime) bone.fTimeLength = frame.fTime;
		}

		// frame scale
		bone.nOffFrameScale = ftell(pFile);
		for (TM_KEYFRAME_SCALE::iterator it = boneInfo.mapFrameScale.begin(); it != boneInfo.mapFrameScale.end(); ++it)
		{
			KEYFRAME_SCALE& frameScale = it->second;

			FmtSkeleton::FRAME_SCALE frame;
			frame.fTime = TicksToSec(frameScale.time);
			frame.vScale[0] = frameScale.vScale.x;
			frame.vScale[1] = frameScale.vScale.y;
			frame.vScale[2] = frameScale.vScale.z;
			fwrite(&frame, sizeof(frame), 1, pFile);

			if (bone.fTimeLength < frame.fTime) bone.fTimeLength = frame.fTime;
		}
	}

	// really write bone list
	fseek(pFile, nBoneListPos, SEEK_SET);
	if (header.nNumBones > 0)
	{
		fwrite(&vBone[0], sizeof(FmtSkeleton::BONE), header.nNumBones, pFile);
	}

	fclose(pFile);
	return true;
}

bool ModelExporter::SaveMaterialsFile(const tstring& strFileName, const tstring& strMeshFile, const tstring& strSkeletonFile, uint vertAttrs)
{
	TiXmlDocument doc;

	TiXmlDeclaration* pXmlDecl = new TiXmlDeclaration(_("1.0"), _("utf-8"), _("yes"));
	doc.LinkEndChild(pXmlDecl);

	TiXmlElement* pXmlMaterials = new TiXmlElement(_("Materials"));
	doc.LinkEndChild(pXmlMaterials);

	pXmlMaterials->SetAttribute(_("count"), (int)m_vMaterial.size());
	for (TV_MATERIAL::iterator it = m_vMaterial.begin(); it != m_vMaterial.end(); ++it)
	{
		MATERIAL& material = (*it);

		TiXmlElement* pXmlMaterial = new TiXmlElement(_("Material"));
		pXmlMaterials->LinkEndChild(pXmlMaterial);

		pXmlMaterial->SetAttribute(_("id"), material.nId);
		pXmlMaterial->SetAttribute(_("vertattr"), vertAttrs);

		int nTexIndex = 0;
		for (TV_STRING::iterator ittex = material.vTextureMap.begin(); ittex != material.vTextureMap.end(); ++ittex)
		{
			tstring& strMap = (*ittex);
			tstring strName;
			StringUtil::GetFileNameWithExt(strName, strMap);

			const tstring& strKey = GetTextureKey((MATERIAL_TEXTURE_TYPE)nTexIndex);
			pXmlMaterial->SetAttribute(strKey.c_str(), strName.c_str());
			++nTexIndex;
		}
	}

	doc.SaveFile(strFileName.c_str());

	return true;
}

const tstring& ModelExporter::GetTextureKey(MATERIAL_TEXTURE_TYPE eType)
{
	static const tstring s_EmptyTexture;
	static const tstring s_TextureKey[MTT_NUM] =
	{
		_("texture0"),
		_("texture1"),
		_("texture2"),
		_("texture3"),
		_("texture4"),
		_("texture5"),
		_("texture6"),
		_("texture7")
	};
	if (eType < 0 || eType >= MTT_NUM) return s_EmptyTexture;
	return s_TextureKey[eType];
}

bool ModelExporter::DumpMesh(MESH_DATA& meshDataOut, IGameNode* pGameNode)
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
	VERTEX_SLOT emptySlot;
	emptySlot.bUsed = false;
	emptySlot.nVertIndex = 0;
	emptySlot.nTexIndex = 0;
	emptySlot.vClone.clear();
	emptySlot.pos.Set(0.0f, 0.0f, 0.0f);
	emptySlot.tex.Set(0.0f, 0.0f);
	emptySlot.Skins.clear();

	// make room for verts
	for (int i = 0; i < nNumVerts; ++i)
	{
		meshDataOut.vVertSlots.push_back(emptySlot);
	}

	// faces
	int nNumFaces = pGameMesh->GetNumberOfFaces();
	if (nNumFaces > 0)
	{
		FaceEx* pFace = pGameMesh->GetFace(0);
		meshDataOut.pMaterial = pGameMesh->GetMaterialFromFace(pFace);
	}

	for (int i = 0; i < nNumFaces; ++i)
	{
		FaceEx* pFace = pGameMesh->GetFace(i);

		FACE face;

		for (int j = 0; j < 3; ++j)
		{
			uint nVertIndex = pFace->vert[j];
			uint nTexIndex = pFace->texCoord[j];

			if (meshDataOut.vVertSlots[nVertIndex].bUsed)
			{
				bool bAddNew = true;
				if (meshDataOut.vVertSlots[nVertIndex].nTexIndex == nTexIndex)
				{
					bAddNew = false;
				}
				else
				{
					TV_INT& vClone = meshDataOut.vVertSlots[nVertIndex].vClone;
					for (TV_INT::iterator it = vClone.begin(); it != vClone.end(); ++it)
					{
						if (meshDataOut.vVertSlots[*it].nTexIndex == nTexIndex)
						{
							bAddNew = false;
							nVertIndex = (*it);
							break;
						}
					}
				}

				meshDataOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				meshDataOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				if (bAddNew)
				{
					// add new slot
					int nNewVertIndex = meshDataOut.vVertSlots.size();
					meshDataOut.vVertSlots.push_back(emptySlot);
					meshDataOut.vVertSlots[nNewVertIndex].bUsed = true;
					meshDataOut.vVertSlots[nNewVertIndex].nVertIndex = nVertIndex;
					meshDataOut.vVertSlots[nNewVertIndex].nTexIndex = nTexIndex;

					meshDataOut.vVertSlots[nVertIndex].vClone.push_back(nNewVertIndex);
					nVertIndex = nNewVertIndex;
				}
			}
			else
			{
				meshDataOut.vVertSlots[nVertIndex].vNormalIndex.insert(pFace->norm[j]);
				int nTangentIndex = pGameMesh->GetFaceVertexTangentBinormal(i, j);
				meshDataOut.vVertSlots[nVertIndex].vTangentIndex.insert(nTangentIndex);

				// set this slot
				meshDataOut.vVertSlots[nVertIndex].bUsed = true;
				meshDataOut.vVertSlots[nVertIndex].nVertIndex = nVertIndex;
				meshDataOut.vVertSlots[nVertIndex].nTexIndex = nTexIndex;
			}

			face.nVertIndex[j] = nVertIndex;
		}

		meshDataOut.vFaces.push_back(face);
	}

	// setup vertex data
	for (int i = 0; i < nNumVerts; ++i)
	{
		VERTEX_SLOT& localSlot = meshDataOut.vVertSlots[i];

		int nNumNormal = 0;
		Point3 vNormal(0.0f, 0.0f, 0.0f);
		for (TS_INT::iterator it = localSlot.vNormalIndex.begin(); it != localSlot.vNormalIndex.end(); ++it)
		{
			vNormal += pGameMesh->GetNormal(*it, false);
			++nNumNormal;
		}

		assert(nNumNormal);
		localSlot.normal = vNormal.Normalize();

		int nNumTangent = 0;
		Point3 vTangent(0.0f, 0.0f, 0.0f);
		for (TS_INT::iterator it = localSlot.vTangentIndex.begin(); it != localSlot.vTangentIndex.end(); ++it)
		{
			vTangent += pGameMesh->GetTangent(*it);
			++nNumTangent;
		}

		assert(nNumTangent);
		localSlot.tangent = vTangent.Normalize();
	}

	int nNewNumVerts = meshDataOut.vVertSlots.size();
	for (int i = 0; i < nNewNumVerts; ++i)
	{
		VERTEX_SLOT& localSlot = meshDataOut.vVertSlots[i];

		// get position
		pGameMesh->GetVertex(localSlot.nVertIndex, localSlot.pos, false);

		if (localSlot.pos.x < m_vBoundingBoxMin.x) m_vBoundingBoxMin.x = localSlot.pos.x;
		if (localSlot.pos.x > m_vBoundingBoxMax.x) m_vBoundingBoxMax.x = localSlot.pos.x;

		if (localSlot.pos.y < m_vBoundingBoxMin.y) m_vBoundingBoxMin.y = localSlot.pos.y;
		if (localSlot.pos.y > m_vBoundingBoxMax.y) m_vBoundingBoxMax.y = localSlot.pos.y;

		if (localSlot.pos.z < m_vBoundingBoxMin.z) m_vBoundingBoxMin.z = localSlot.pos.z;
		if (localSlot.pos.z > m_vBoundingBoxMax.z) m_vBoundingBoxMax.z = localSlot.pos.z;

		// get normal and tangent
		if (i != localSlot.nVertIndex)
		{
			localSlot.normal = meshDataOut.vVertSlots[localSlot.nVertIndex].normal;
			localSlot.tangent = meshDataOut.vVertSlots[localSlot.nVertIndex].tangent;
		}

		// get uv
		pGameMesh->GetTexVertex(localSlot.nTexIndex, localSlot.tex);
	}

	// skins
	int nNumModifier = pGameObject->GetNumModifiers();
	for (int i = 0; i < nNumModifier; ++i)
	{
		IGameModifier* pGameModifier = pGameObject->GetIGameModifier(i);
		if (!pGameModifier->IsSkin()) continue;
		IGameSkin* pGameSkin = (IGameSkin*)pGameModifier;

		bool bOK = DumpSkin(meshDataOut, pGameSkin);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool SkinComparator(const ModelExporter::SKIN& a, const ModelExporter::SKIN& b)
{
	return b.fWeight > a.fWeight;
}

bool ModelExporter::DumpSkin(MESH_DATA& meshDataOut, IGameSkin* pGameSkin)
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

			SKIN skin;
			skin.nBoneIndex = it->second;
			skin.fWeight = pGameSkin->GetWeight(i, j);
			meshDataOut.vVertSlots[i].Skins.push_back(skin);
		}

		std::sort(meshDataOut.vVertSlots[i].Skins.begin(), meshDataOut.vVertSlots[i].Skins.end(), SkinComparator);
	}

	int nNumAdditionVerts = meshDataOut.vVertSlots.size();
	for (int i = nNumSkinVerts; i < nNumAdditionVerts; ++i)
	{
		int nOldIndex = meshDataOut.vVertSlots[i].nVertIndex;
		meshDataOut.vVertSlots[i].Skins = meshDataOut.vVertSlots[nOldIndex].Skins;
	}

	return true;
}

bool ModelExporter::DumpController(BONE_INFO& boneInfo, IGameNode* pGameNode)
{
	IGameControl* pGameControl = pGameNode->GetIGameControl();

	bool bBiped = false;

	// export position controller
	if (pGameControl->IsAnimated(IGAME_POS))
	{
		if (pGameControl->GetControlType(IGAME_POS) == IGameControl::IGAME_BIPED)
		{
			// dump sample keys
			DumpSampleKey(boneInfo, pGameControl, IGAME_TM);
			bBiped = true;
		}
		else
		{
			DumpPositionController(boneInfo, pGameControl);
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
				DumpSampleKey(boneInfo, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpRotationController(boneInfo, pGameControl);
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
				DumpSampleKey(boneInfo, pGameControl, IGAME_TM);
				bBiped = true;
			}
		}
		else
		{
			DumpScaleController(boneInfo, pGameControl);
		}
	}

	for (TM_KEYFRAME_ROT::iterator it = boneInfo.mapFrameRot.begin(); it != boneInfo.mapFrameRot.end(); ++it)
	{
		KEYFRAME_ROT& keyFrame = it->second;

		if (keyFrame.nMask & KFM_ROT)
		{
			if (keyFrame.nMask & KFM_QUAT)
			{
				// TODO: logout
				assert(false);
			}

			Math::BuildQuaternionFromEulerXYZ(keyFrame.qRot, keyFrame.vRot.x, keyFrame.vRot.y, keyFrame.vRot.z);
		}
		else if (keyFrame.nMask & KFM_QUAT)
		{
			if (keyFrame.nMask & KFM_ROT)
			{
				// TODO: logout
				assert(false);
			}
		}
	}

	return true;
}

bool ModelExporter::DumpPositionController(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_POS);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std pos key
			DumpMaxStdPosKey(boneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_POS_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(boneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(boneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_INDEPENDENT_POS:
		{
			// export independent controller
			DumpIndependentPosKey(boneInfo, pGameControl);
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

bool ModelExporter::DumpRotationController(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_ROT);
	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export std rot key
			DumpMaxStdRotKey(boneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_EULER:
		{
			// export Euler controller
			DumpEulerRotKey(boneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_ROT_CONSTRAINT:
		{
			// export constraint controller
			DumpConstraintKey(boneInfo, pGameControl);
		}
		break;
	case IGameControl::IGAME_LIST:
		{
			// export list controller
			DumpListKey(boneInfo, pGameControl);
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

bool ModelExporter::DumpScaleController(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameControl::MaxControlType eControlType = pGameControl->GetControlType(IGAME_SCALE);

	switch (eControlType)
	{
	case IGameControl::IGAME_MAXSTD:
		{
			// export scale key
			DumpMaxStdScaleKey(boneInfo, pGameControl);
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

bool ModelExporter::DumpMaxStdPosKey(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameKeyTab posKey;
	if (pGameControl->GetBezierKeys(posKey, IGAME_POS))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS, posKey[i].bezierKey.pval);
		}
	}
	else if (pGameControl->GetLinearKeys(posKey, IGAME_POS))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS, posKey[i].linearKey.pval);
		}
	}

	return true;
}

bool ModelExporter::DumpIndependentPosKey(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameKeyTab posKey;
	if (pGameControl->GetBezierKeys(posKey, IGAME_POS_X))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS_X, posKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(posKey, IGAME_POS_X))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS_X, posKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(posKey, IGAME_POS_Y))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS_Y, posKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(posKey, IGAME_POS_Y))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS_Y, posKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(posKey, IGAME_POS_Z))
	{
		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS_Z, posKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(posKey, IGAME_POS_Z))
	{
		int nCount = posKey.Count();

		for (int i = 0; i < posKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, posKey[i].t, KFM_POS_Z, posKey[i].linearKey.fval);
		}
	}

	return true;
}

bool ModelExporter::DumpMaxStdRotKey(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameKeyTab rotKey;
	if (pGameControl->GetBezierKeys(rotKey, IGAME_ROT))
	{
		// export Bezier Keys
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, rotKey[i].bezierKey.qval);
		}
	}
	else if (pGameControl->GetLinearKeys(rotKey, IGAME_ROT))
	{
		// export Linear Keys
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, rotKey[i].linearKey.qval);
		}
	}
	else if (pGameControl->GetTCBKeys(rotKey, IGAME_ROT))
	{
		// export TCB Keys
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			Quat qValue = QFromAngAxis(rotKey[i].tcbKey.aval.angle, rotKey[i].tcbKey.aval.axis);
			InsertKeyFrame(boneInfo, rotKey[i].t, qValue);
		}
	}

	return true;
}

bool ModelExporter::DumpEulerRotKey(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	IGameKeyTab rotKey;

	if (pGameControl->GetBezierKeys(rotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, KFM_ROT_X, rotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(rotKey, IGAME_EULER_X))
	{
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, KFM_ROT_X, rotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(rotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, KFM_ROT_Y, rotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(rotKey, IGAME_EULER_Y))
	{
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, KFM_ROT_Y, rotKey[i].linearKey.fval);
		}
	}

	if (pGameControl->GetBezierKeys(rotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, KFM_ROT_Z, rotKey[i].bezierKey.fval);
		}
	}

	if (pGameControl->GetLinearKeys(rotKey, IGAME_EULER_Z))
	{
		for (int i = 0; i < rotKey.Count(); ++i)
		{
			InsertKeyFrame(boneInfo, rotKey[i].t, KFM_ROT_Z, rotKey[i].linearKey.fval);
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

bool ModelExporter::DumpConstraintKey(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	// TODO: 
	assert(false);

// 	IGameConstraint* pGameConstraint = pGameControl->GetConstraint(IGAME_POS);
// 
// 	int nConstraintCount = pGameConstraint->NumberOfConstraintNodes();
// 	for(int i = 0; i < nConstraintCount; ++i)
// 	{
// 		float fWeight = pGameConstraint->GetConstraintWeight(i);
// 		int nNodeID = pGameConstraint->GetConstraintNodes(i)->GetNodeID();
// 
// 		// TODO: export constraint
// 	}
// 
// 	IPropertyContainer* pPropertyContainer = pGameConstraint->GetIPropertyContainer();
// 	int nPropertyCount = pPropertyContainer->GetNumberOfProperties();
// 
// 	for(int i = 0; i < nPropertyCount; ++i)
// 	{
// 		// TODO: dump property
// 	}

	return true;
}

bool ModelExporter::DumpListKey(BONE_INFO& boneInfo, IGameControl* pGameControl)
{
	int nCount = pGameControl->GetNumOfListSubControls(IGAME_POS);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_POS);
		bool bOK = DumpPositionController(boneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_ROT);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_ROT);
		bool bOK = DumpRotationController(boneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	nCount = pGameControl->GetNumOfListSubControls(IGAME_SCALE);
	for (int i = 0; i < nCount; ++i)
	{
		IGameControl* pSubGameControl = pGameControl->GetListSubControl(i, IGAME_SCALE);
		bool bOK = DumpScaleController(boneInfo, pSubGameControl);
		assert(bOK);
		// TODO: check bOK
	}

	return true;
}

bool ModelExporter::DumpSampleKey(BONE_INFO& boneInfo, IGameControl* pGameControl, IGameControlType eType)
{
	IGameKeyTab keys;
	if (pGameControl->GetFullSampledKeys(keys, 1, eType, true))
	{
		for (int i = 0; i < keys.Count(); ++i)
		{
			switch (eType)
			{
			case IGAME_TM:
				{
					InsertKeyFrame(boneInfo, keys[i].t, keys[i].sampleKey.gval);
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

ModelExporter::KEYFRAME_POS* ModelExporter::GetKeyFrame(TM_KEYFRAME_POS& framePos, TimeValue time)
{
	KEYFRAME_POS* pKeyFrame = NULL;

	TM_KEYFRAME_POS::iterator itfound = framePos.find(time);
	if (itfound != framePos.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_POS emptyKeyFrame;
		emptyKeyFrame.time = time;
		emptyKeyFrame.nMask = KFM_UNKNOWN;
		framePos.insert(std::make_pair(emptyKeyFrame.time, emptyKeyFrame));

		itfound = framePos.find(time);
		assert(itfound != framePos.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

ModelExporter::KEYFRAME_SCALE* ModelExporter::GetKeyFrame(TM_KEYFRAME_SCALE& frameScale, TimeValue time)
{
	KEYFRAME_SCALE* pKeyFrame = NULL;

	TM_KEYFRAME_SCALE::iterator itfound = frameScale.find(time);
	if (itfound != frameScale.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_SCALE emptyKeyFrame;
		emptyKeyFrame.time = time;
		emptyKeyFrame.nMask = KFM_UNKNOWN;
		emptyKeyFrame.vScale.Reset(1.0f, 1.0f, 1.0f);
		frameScale.insert(std::make_pair(emptyKeyFrame.time, emptyKeyFrame));

		itfound = frameScale.find(time);
		assert(itfound != frameScale.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

ModelExporter::KEYFRAME_ROT* ModelExporter::GetKeyFrame(TM_KEYFRAME_ROT& frameRot, TimeValue time)
{
	KEYFRAME_ROT* pKeyFrame = NULL;

	TM_KEYFRAME_ROT::iterator itfound = frameRot.find(time);
	if (itfound != frameRot.end())
	{
		pKeyFrame = &itfound->second;
	}
	else
	{
		KEYFRAME_ROT emptyKeyFrame;
		emptyKeyFrame.time = time;
		emptyKeyFrame.nMask = KFM_UNKNOWN;
		frameRot.insert(std::make_pair(emptyKeyFrame.time, emptyKeyFrame));

		itfound = frameRot.find(time);
		assert(itfound != frameRot.end());
		pKeyFrame = &itfound->second;
	}

	return pKeyFrame;
}

bool ModelExporter::InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, KEY_FRAME_MASK eMask, float fValue)
{
	switch (eMask)
	{
	case KFM_POS_X:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(boneInfo.mapFramePos, time);
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
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(boneInfo.mapFramePos, time);
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
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(boneInfo.mapFramePos, time);
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
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(boneInfo.mapFrameScale, time);
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
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(boneInfo.mapFrameScale, time);
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
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(boneInfo.mapFrameScale, time);
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
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(boneInfo.mapFrameRot, time);
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
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(boneInfo.mapFrameRot, time);
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
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(boneInfo.mapFrameRot, time);
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

bool ModelExporter::InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue)
{
	switch (eMask)
	{
	case KFM_POS:
		{
			KEYFRAME_POS* pKeyFrame = GetKeyFrame(boneInfo.mapFramePos, time);
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
			KEYFRAME_SCALE* pKeyFrame = GetKeyFrame(boneInfo.mapFrameScale, time);
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
			KEYFRAME_ROT* pKeyFrame = GetKeyFrame(boneInfo.mapFrameRot, time);
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

bool ModelExporter::InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, const Quat& qValue)
{
	KEYFRAME_ROT* pKeyFrame = GetKeyFrame(boneInfo.mapFrameRot, time);
	if (pKeyFrame->nMask & KFM_ROT)
	{
		// TODO: logout
		assert(false);
	}

	MaxQuat2SpankQuat(pKeyFrame->qRot, qValue);
	pKeyFrame->nMask |= KFM_QUAT;

	return true;
}

bool ModelExporter::InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, const GMatrix& matValue)
{
	KEYFRAME_POS* pKeyFramePos = GetKeyFrame(boneInfo.mapFramePos, time);
	if (pKeyFramePos->nMask & KFM_POS)
	{
		// TODO: logout
		assert(false);
	}

	KEYFRAME_SCALE* pKeyFrameScale = GetKeyFrame(boneInfo.mapFrameScale, time);
	if (pKeyFrameScale->nMask & KFM_SCALE)
	{
		// TODO: logout
		assert(false);
	}

	KEYFRAME_ROT* pKeyFrameRot = GetKeyFrame(boneInfo.mapFrameRot, time);
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
	matOut.e[1] = matIn[0][1];
	matOut.e[2] = matIn[0][2];
	matOut.e[3] = matIn[0][3];

	matOut.e[4] = matIn[1][0];
	matOut.e[5] = matIn[1][1];
	matOut.e[6] = matIn[1][2];
	matOut.e[7] = matIn[1][3];

	matOut.e[8] = matIn[2][0];
	matOut.e[9] = matIn[2][1];
	matOut.e[10] = matIn[2][2];
	matOut.e[11] = matIn[2][3];

	matOut.e[12] = matIn[3][0];
	matOut.e[13] = matIn[3][1];
	matOut.e[14] = matIn[3][2];
	matOut.e[15] = matIn[3][3];
}

void ModelExporter::MaxQuat2SpankQuat(Quaternion& qOut, const Quat& qIn)
{
	qOut.x = qIn.x;
	qOut.y = qIn.y;
	qOut.z = qIn.z;
	qOut.w = qIn.w;
}

void ModelExporter::MaxVec2SpankVec(Vector3& vOut, const Point3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.y;
	vOut.z = vIn.z;
}

void ModelExporter::MaxVec2SpankVec(float* fOut3, const Point3& vIn)
{
	fOut3[0] = vIn.x;
	fOut3[1] = vIn.y;
	fOut3[2] = vIn.z;
}

void ModelExporter::MaxEular2SpankEular(Vector3& vOut, const Point3& vIn)
{
	vOut.x = vIn.x;
	vOut.y = vIn.y;
	vOut.z = vIn.z;
}

void ModelExporter::MaxUV2SpankUV(float* fOut2, const Point2& uv)
{
	fOut2[0] = uv.x;
	fOut2[1] = uv.y;
}

void ModelExporter::MaxFaceIndex2SpankFaceIndex(ushort* nOut3, const int* indis)
{
	nOut3[0] = (ushort)indis[0];
	nOut3[1] = (ushort)indis[1];
	nOut3[2] = (ushort)indis[2];
}
