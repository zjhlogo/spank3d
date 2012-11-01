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
public:
	RTTI_DEF(RenderTarget, IApp);

	RenderTarget();
	virtual ~RenderTarget();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float dt);
	virtual void Render();

private:
	bool OnMouseEvent(MouseEvent& mouseEvent);
	void RenderScene(const Matrix4x4& matProj, ITexture* pTexture);

private:
	IShader* m_pShader;
	IMesh* m_pMesh;
	ITexture* m_pTexture;

	Matrix4x4 m_matRenderTargetProj;
	IRenderTarget* m_pRenderTarget;
	ITexture* m_pRenderTargetTexture;

	ICamera* m_pCamera;
	TargetCameraControl* m_pTargetCameraCtrl;

};
#endif // __RENDERTARGET_H__
