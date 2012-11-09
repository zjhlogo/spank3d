/*!
 * \file RenderTarget.h
 * \date 11-01-2012 14:39:15
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#include <Spank3d.h>
#include <camera/TargetCameraControl.h>

class RenderTarget : public IApp
{
	enum CONST_DEFINE
	{
		RENDER_TARGET_WIDTH = 256,
		RENDER_TARGET_HEIGHT = 256,
	};

public:
	RTTI_DEF(RenderTarget, IApp);

	RenderTarget();
	virtual ~RenderTarget();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	void RenderTextureMap();
	void RenderScene();

private:
	IShader* m_pShaderScene;
	IShader* m_pShaderUvMapping;
	IMesh* m_pMesh;

	IRenderTarget* m_pRenderTarget;
	ITexture* m_pColorTexture;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __RENDERTARGET_H__
