/*!
 * \file IResMgr.h
 * \date 4-19-2012 22:54:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IRESMGR_H__
#define __IRESMGR_H__

#include "ISingleton.h"
#include "IMesh.h"
#include "IBitmapData.h"
#include "../render/ITexture.h"
#include "../render/IShader.h"
#include "../render/IRenderTarget.h"

class IResMgr : public ISingleton
{
public:
	RTTI_DEF(IResMgr, ISingleton);

	IResMgr() {};
	virtual ~IResMgr() {};

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;

	virtual IMesh* CreateMesh(const tstring& strFile) = 0;

	virtual IBitmapData* CreateBitmapData(const tstring& id, uint width, uint height, uint bpp = 32) = 0;
	virtual IBitmapData* CreateBitmapData(const tstring& strFile) = 0;

	virtual ITexture* CreateTexture2D(uint width, uint height, uint nTexFormat = TEXTURE_FORMAT::TF_RGBA) = 0;
	virtual ITexture* CreateTexture2D(const tstring& id, const IBitmapData* pBitmapData) = 0;
	virtual ITexture* CreateTexture2D(const tstring& strFile) = 0;
	virtual ITexture* CreateTextureCube(const tstring& strFile) = 0;

	virtual IShader* CreateShader(const tstring& strFile) = 0;

	virtual IRenderTarget* CreateRenderTarget(ITexture* pColorTexture, ITexture* pDepthTexture) = 0;

	virtual bool ReadStringFile(tstring& strOut, const tstring& strFile) = 0;
};

#endif // __IRESMGR_H__
