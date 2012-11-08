/*!
 * \file CubeMap.h
 * \date 10-29-2012 11:12:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

#include <Spank3d.h>
#include <camera/TargetCameraControl.h>

class CubeMap : public IApp
{
public:
	RTTI_DEF(CubeMap, IApp);

	CubeMap();
	virtual ~CubeMap();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	IShader* m_pShader;
	IMesh* m_pMesh;
	ITexture* m_pTexture;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __CUBEMAP_H__
