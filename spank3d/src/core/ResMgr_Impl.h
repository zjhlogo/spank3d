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

public:
	RTTI_DEF(ResMgr_Impl, IResMgr);

	ResMgr_Impl();
	virtual ~ResMgr_Impl();

	static ResMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

	virtual IMesh* CreateMesh(const tstring& strFile);

	virtual IBitmapData* CreateBitmapData(uint width, uint height, uint bpp = 32);
	virtual IBitmapData* CreateBitmapData(const tstring& strFile);

	virtual ITexture* CreateTexture(const IBitmapData* pBitmapData);
	virtual ITexture* CreateTexture(const tstring& strFile);

private:
	TM_MESH m_MeshMap;
	tstring m_strDefaultDir;

};
#endif // __RESMGR_IMPL_H__
