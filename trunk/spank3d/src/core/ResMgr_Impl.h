/*!
 * \file ResMgr_Impl.h
 * \date 4-19-2012 22:56:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RESMGR_IMPL_H__
#define __RESMGR_IMPL_H__

#include <core/IResMgr.h>
#include <map>
#include "Mesh_Impl.h"

class ResMgr_Impl : public IResMgr
{
public:
	typedef std::map<tstring, Mesh_Impl*> TM_MESH;
	typedef std::map<tstring, IBitmapData*> TM_BITMAP_DATA;
	typedef std::map<tstring, ITexture*> TM_TEXTURE;
	typedef std::map<tstring, IShader*> TM_SHADER;

public:
	RTTI_DEF(ResMgr_Impl, IResMgr);

	static ResMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

	virtual IMesh* CreateMesh(const tstring& strFile);

	virtual IBitmapData* CreateBitmapData(const tstring& id, uint width, uint height, uint bpp = 32);
	virtual IBitmapData* CreateBitmapData(const tstring& strFile);

	virtual ITexture* CreateTexture(const tstring& id, const IBitmapData* pBitmapData, ITexture::TEXTURE_SAMPLE eSample = ITexture::TS_LINEAR);
	virtual ITexture* CreateTexture(const tstring& strFile, ITexture::TEXTURE_SAMPLE eSample = ITexture::TS_LINEAR);

	virtual IShader* CreateShader(const tstring& strFile);

	virtual bool ReadStringFile(tstring& strOut, const tstring& strFile);

private:
	ResMgr_Impl();
	virtual ~ResMgr_Impl();

private:
	ITexture* InternalCreateTexture(const tstring& id, const IBitmapData* pBitmapData, ITexture::TEXTURE_SAMPLE eSample);
	IBitmapData* InternalCreateBitmapData(const tstring& id, uint width, uint height, uint bpp);

	bool OnMeshDestroyed(IEvent& event);
	bool OnBitmapDataDestroyed(IEvent& event);
	bool OnTextureDestroyed(IEvent& event);
	bool OnShaderDestroyed(IEvent& event);

private:
	TM_MESH m_MeshMap;
	TM_BITMAP_DATA m_BitmapDataMap;
	TM_TEXTURE m_TextureMap;
	TM_SHADER m_ShaderMap;

	tstring m_strDefaultDir;

};
#endif // __RESMGR_IMPL_H__
