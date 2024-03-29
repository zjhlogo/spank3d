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
#include <set>
#include "Mesh_Impl.h"

class ResMgr_Impl : public IResMgr
{
public:
	typedef std::map<tstring, Mesh_Impl*> TM_MESH;
	typedef std::map<tstring, IBitmapData*> TM_BITMAP_DATA;
	typedef std::map<tstring, ITexture*> TM_TEXTURE;
	typedef std::map<tstring, IShader*> TM_SHADER;
	typedef std::set<IRenderTarget*> TS_RENDER_TARGET;

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

	virtual ITexture* CreateTexture2D(uint width, uint height, uint nTexFormat = TEXTURE_FORMAT::TF_RGBA);
	virtual ITexture* CreateTexture2D(const tstring& id, const IBitmapData* pBitmapData);
	virtual ITexture* CreateTexture2D(const tstring& strFile);
	virtual ITexture* CreateTextureCube(const tstring& strFile);

	virtual IShader* CreateShader(const tstring& strFile);

	virtual IRenderTarget* CreateRenderTarget(ITexture* pColorTexture, ITexture* pDepthTexture);

	virtual bool ReadStringFile(tstring& strOut, const tstring& strFile);

private:
	ResMgr_Impl();
	virtual ~ResMgr_Impl();

private:
	bool OnMeshDestroyed(IEvent& event);
	bool OnBitmapDataDestroyed(IEvent& event);
	bool OnTextureDestroyed(IEvent& event);
	bool OnShaderDestroyed(IEvent& event);
	bool OnRenderTargetDestroyed(IEvent& event);

private:
	TM_MESH m_MeshMap;
	TM_BITMAP_DATA m_BitmapDataMap;
	TM_TEXTURE m_TextureMap;
	TM_SHADER m_ShaderMap;
	TS_RENDER_TARGET m_RenderTargetSet;

	tstring m_strDefaultDir;
	uint m_nNumMemoryTextures;

};
#endif // __RESMGR_IMPL_H__
