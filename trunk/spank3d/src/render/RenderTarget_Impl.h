/*!
 * \file RenderTarget_Impl.h
 * \date 10-31-2012 13:43:47
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __RENDERTARGET_IMPL_H__
#define __RENDERTARGET_IMPL_H__

#include <render/IRenderTarget.h>

class RenderTarget_Impl : public IRenderTarget
{
public:
	RTTI_DEF(RenderTarget_Impl, IRenderTarget);

	RenderTarget_Impl();
	virtual ~RenderTarget_Impl();

	virtual bool SetTargetTexture(ITexture* pTexture);
	virtual ITexture* GetTargetTexture();

	virtual bool BeginRender();
	virtual void EndRender();

private:
	bool CreateRenderTarget();
	void DestroyRenderTarget();

	bool CreateDepthBuffer(uint width, uint height);
	void DestroyDepthBuffer();

private:
	uint m_nFrameBuffer;

	uint m_nDepthBuffer;
	uint m_nDepthBufferWidth;
	uint m_nDepthBufferHeight;

	ITexture* m_pTargetTexture;

};
#endif // __RENDERTARGET_IMPL_H__
