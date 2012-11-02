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

	RenderTarget_Impl(ITexture* pColorTexture, ITexture* pDepthTexture);
	virtual ~RenderTarget_Impl();

	virtual ITexture* GetColorTexture();
	virtual ITexture* GetDepthTexture();

	virtual bool BeginRender();
	virtual void EndRender();

private:
	void InitMember();

	bool CreateRenderTarget(ITexture* pColorTexture, ITexture* pDepthTexture);
	void DestroyRenderTarget();

	bool CreateColorTextureBuffer(ITexture* pColorTexture);
	bool CreateDepthTextureBuffer(ITexture* pDepthTexture);
	bool CreateColorDepthTextureBuffer(ITexture* pColorTexture, ITexture* pDepthTexture);

private:
	uint m_nFrameBuffer;
	uint m_nDepthBuffer;
	uint m_nClearType;

	uint m_nFrameWidth;
	uint m_nFrameHeight;

	ITexture* m_pColorTexture;
	ITexture* m_pDepthTexture;

};
#endif // __RENDERTARGET_IMPL_H__
