/*!
 * \file ShadowMap.h
 * \date 11-02-2012 11:20:48
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __SHADOWMAP_H__
#define __SHADOWMAP_H__

#include <Spank3d.h>
#include <camera/TargetCameraControl.h>

class ShadowMap : public IApp
{
	enum CONST_DEFINE
	{
		RENDER_TARGET_WIDTH = 256,
		RENDER_TARGET_HEIGHT = 256,
	};

public:
	RTTI_DEF(ShadowMap, IApp);

	ShadowMap();
	virtual ~ShadowMap();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	bool OnMouseEvent(MouseEvent& mouseEvent);
	void RenderDepth();
	void RenderScene();

private:
	IShader* m_pShaderDepth;
	IShader* m_pShaderShadowMap;
	IMesh* m_pMesh;

	Matrix4x4 m_matDepthBias;
	Matrix4x4 m_matDepthBiasModelViewProj;
	IRenderTarget* m_pRenderTarget;

	ITexture* m_pDepthTexture;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __SHADOWMAP_H__
