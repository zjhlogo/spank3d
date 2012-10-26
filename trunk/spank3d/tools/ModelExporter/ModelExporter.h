/*!
 * \file ModelExporter.h
 * \date 1-3-2010 21:00:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELEXPORTER_H__
#define __MODELEXPORTER_H__

#include <math/Math.h>
#include <max.h>
#include <IGame/IGame.h>
#include <IGame/IGameModifier.h>

#include <type/BaseType.h>

#include <vector>
#include <set>
#include <map>

class ModelExporter : public SceneExport
{
public:
	enum CONST_DEFINE
	{
		CURRENT_VERSION = 100,
	};

	enum KEY_FRAME_MASK
	{
		KFM_UNKNOWN		= 0,
		KFM_POS_X		= 0x00000001,
		KFM_POS_Y		= 0x00000002,
		KFM_POS_Z		= 0x00000004,
		KFM_SCALE_X		= 0x00000008,
		KFM_SCALE_Y		= 0x00000010,
		KFM_SCALE_Z		= 0x00000020,
		KFM_ROT_X		= 0x00000040,
		KFM_ROT_Y		= 0x00000080,
		KFM_ROT_Z		= 0x00000100,
		KFM_QUAT		= 0x00000200,
		KFM_POS			= KFM_POS_X | KFM_POS_Y | KFM_POS_Z,
		KFM_SCALE		= KFM_SCALE_X | KFM_SCALE_Y | KFM_SCALE_Z,
		KFM_ROT			= KFM_ROT_X | KFM_ROT_Y | KFM_ROT_Z,
	};

	enum MATERIAL_TEXTURE_TYPE
	{
		MTT_TEXTURE0 = 0,
		MTT_TEXTURE1,
		MTT_TEXTURE2,
		MTT_TEXTURE3,
		MTT_TEXTURE4,
		MTT_TEXTURE5,
		MTT_TEXTURE6,
		MTT_TEXTURE7,
		MTT_NUM,
	};

	typedef struct NODE_INFO_tag
	{
		IGameNode* pGameNode;
		IGameNode* pParentGameNode;
	} NODE_INFO;

	typedef std::vector<NODE_INFO> TV_NODE_INFO;

	typedef struct SKIN_tag
	{
		int nBoneIndex;
		float fWeight;
	} SKIN;

	typedef std::vector<SKIN> TV_SKIN;

	typedef std::vector<int> TV_INT;
	typedef std::set<int> TS_INT;

	typedef struct VERTEX_SLOT_tag
	{
		bool bUsed;
		uint nVertIndex;
		uint nTexIndex;
		TV_INT vClone;

		Point3 pos;
		TS_INT vNormalIndex;
		TS_INT vTangentIndex;
		Point3 normal;
		Point3 tangent;
		Point2 tex;
		TV_SKIN Skins;
	} VERTEX_SLOT;

	typedef std::vector<VERTEX_SLOT> TV_VERTEX_SLOT;

	typedef struct FACE_tag
	{
		int nVertIndex[3];
	} FACE;

	typedef std::vector<FACE> TV_FACE;

	typedef struct MESH_DATA_tag
	{
		tstring strName;
		GMatrix matLocal;
		TV_VERTEX_SLOT vVertSlots;
		TV_FACE vFaces;
		IGameMaterial* pMaterial;
		int nMaterialId;
	} MESH_DATA;

	typedef std::vector<MESH_DATA> TV_MESH_DATA;

	// bone information
	typedef struct KEYFRAME_ROT_tag
	{
		TimeValue time;
		uint nMask;
		Vector3 vRot;
		Quaternion qRot;
	} KEYFRAME_ROT;

	typedef struct KEYFRAME_POS_tag
	{
		TimeValue time;
		uint nMask;
		Vector3 vPos;
	} KEYFRAME_POS;

	typedef struct KEYFRAME_SCALE_tag
	{
		TimeValue time;
		uint nMask;
		Vector3 vScale;
	} KEYFRAME_SCALE;

	typedef std::map<TimeValue, KEYFRAME_ROT> TM_KEYFRAME_ROT;
	typedef std::map<TimeValue, KEYFRAME_POS> TM_KEYFRAME_POS;
	typedef std::map<TimeValue, KEYFRAME_SCALE> TM_KEYFRAME_SCALE;

	typedef struct BONE_INFO_tag
	{
		tstring strName;
		IGameNode* pNode;
		IGameNode* pParentNode;

		int nId;
		int nParentBoneId;

		Matrix4x4 matLocal;

		TM_KEYFRAME_ROT mapFrameRot;
		TM_KEYFRAME_POS mapFramePos;
		TM_KEYFRAME_SCALE mapFrameScale;
	} BONE_INFO;

	typedef std::vector<BONE_INFO> TV_BONE_INFO;
	typedef std::map<IGameNode*, int> TM_BONE_INFO;

	typedef std::vector<tstring> TV_STRING;

	typedef struct MATERIAL_tag
	{
		int nId;
		IGameMaterial* pMaterial;
		TV_STRING vTextureMap;
	} MATERIAL;

	typedef std::vector<MATERIAL> TV_MATERIAL;

public:
	ModelExporter();
	virtual ~ModelExporter();

	virtual int ExtCount();						// Number of extensions supported
	virtual const TCHAR* Ext(int n);			// Extension #n (i.e. "3DS")
	virtual const TCHAR* LongDesc();			// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR* ShortDesc();			// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR* AuthorName();			// ASCII Author name
	virtual const TCHAR* CopyrightMessage();	// ASCII Copyright message
	virtual const TCHAR* OtherMessage1();		// Other message #1
	virtual const TCHAR* OtherMessage2();		// Other message #2
	virtual unsigned int Version();				// Version number * 100 (i.e. v3.01 = 301)
	virtual void ShowAbout(HWND hWnd);			// Show DLL's "About..." box
	virtual int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts = FALSE, DWORD options = 0);	// Export file

private:
	void Init();
	void Destroy();

	void Cleanup();

	bool CollectNodes(IGameNode* pGameNode, IGameNode* pParentGameNode = NULL);

	bool BuildBonesInfo();
	bool BuildMeshsInfo();
	bool BuildMaterialsInfo();
	bool FindMaterial(IGameMaterial* pMaterial);

	bool SaveMeshFile(const tstring& strFileName, uint vertAttrs);
	bool SaveSkeletonFile(const tstring& strFileName);
	bool SaveMaterialsFile(const tstring& strFileName, const tstring& strMeshFile, const tstring& strSkeletonFile, uint vertAttrs);
	const tstring& GetTextureKey(MATERIAL_TEXTURE_TYPE eType);

	bool DumpMesh(MESH_DATA& meshDataOut, IGameNode* pGameNode);
	bool DumpSkin(MESH_DATA& meshDataOut, IGameSkin* pGameSkin);

	bool DumpController(BONE_INFO& boneInfo, IGameNode* pGameNode);
	bool DumpPositionController(BONE_INFO& boneInfo, IGameControl* pGameControl);
	bool DumpRotationController(BONE_INFO& boneInfo, IGameControl* pGameControl);
	bool DumpScaleController(BONE_INFO& boneInfo, IGameControl* pGameControl);

	bool DumpMaxStdPosKey(BONE_INFO& boneInfo, IGameControl* pGameControl);
	bool DumpIndependentPosKey(BONE_INFO& boneInfo, IGameControl* pGameControl);

	bool DumpMaxStdRotKey(BONE_INFO& boneInfo, IGameControl* pGameControl);
	bool DumpEulerRotKey(BONE_INFO& boneInfo, IGameControl* pGameControl);

	bool DumpMaxStdScaleKey(BONE_INFO& boneInfo, IGameControl* pGameControl);

	bool DumpConstraintKey(BONE_INFO& boneInfo, IGameControl* pGameControl);
	bool DumpListKey(BONE_INFO& boneInfo, IGameControl* pGameControl);
	bool DumpSampleKey(BONE_INFO& boneInfo, IGameControl* pGameControl, IGameControlType eType);

	KEYFRAME_POS* GetKeyFrame(TM_KEYFRAME_POS& framePos, TimeValue time);
	KEYFRAME_SCALE* GetKeyFrame(TM_KEYFRAME_SCALE& frameScale, TimeValue time);
	KEYFRAME_ROT* GetKeyFrame(TM_KEYFRAME_ROT& frameRot, TimeValue time);

	bool InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, KEY_FRAME_MASK eMask, float fValue);
	bool InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, KEY_FRAME_MASK eMask, const Point3& vValue);
	bool InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, const Quat& qValue);
	bool InsertKeyFrame(BONE_INFO& boneInfo, TimeValue time, const GMatrix& matValue);

	void MaxMat2SpankMat(Matrix4x4& matOut, const GMatrix& matIn);
	void MaxQuat2SpankQuat(Quaternion& qOut, const Quat& qIn);
	void MaxVec2SpankVec(Vector3& vOut, const Point3& vIn);
	void MaxVec2SpankVec(float* fOut3, const Point3& vIn);
	void MaxEular2SpankEular(Vector3& vOut, const Point3& vIn);
	void MaxUV2SpankUV(float* fOut2, const Point2& uv);
	void MaxFaceIndex2SpankFaceIndex(ushort* nOut3, const int* indis);

private:
	TV_NODE_INFO m_vMeshNode;
	TV_NODE_INFO m_vBoneNode;
	TV_MESH_DATA m_vMeshData;
	TV_BONE_INFO m_vBoneInfo;
	TM_BONE_INFO m_vBoneInfoMap;
	TV_MATERIAL m_vMaterial;
	Point3 m_vBoundingBoxMin;
	Point3 m_vBoundingBoxMax;
	Interface* m_pInterface;

};
#endif // __MODELEXPORTER_H__
