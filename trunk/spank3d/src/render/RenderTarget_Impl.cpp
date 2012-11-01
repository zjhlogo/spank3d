/*!
 * \file RenderTarget_Impl.cpp
 * \date 10-31-2012 13:45:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "RenderTarget_Impl.h"
#include <gl/glew.h>
#include <Spank3d.h>

RenderTarget_Impl::RenderTarget_Impl()
{
	m_nFrameBuffer = 0;

	m_nDepthBuffer = 0;
	m_nDepthBufferWidth = 0;
	m_nDepthBufferHeight = 0;

	m_pTargetTexture = NULL;

	SetOk(CreateRenderTarget());
}

RenderTarget_Impl::~RenderTarget_Impl()
{
	DestroyRenderTarget();
	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

bool RenderTarget_Impl::SetTargetTexture(ITexture* pTexture)
{
	if (!pTexture) return false;

	if (m_pTargetTexture == pTexture) return true;
	if (pTexture->GetTextureType() != GL_TEXTURE_2D) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->GetTextureId(), 0);
	if (!CreateDepthBuffer(pTexture->GetWidth(), pTexture->GetHeight())) return false;

	GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBufs);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pTargetTexture = pTexture;

	return true;
}

ITexture* RenderTarget_Impl::GetTargetTexture()
{
	return m_pTargetTexture;
}

bool RenderTarget_Impl::BeginRender()
{
	if (m_nFrameBuffer == 0) return false;
	if (!m_pTargetTexture) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);
	glViewport(0, 0, m_pTargetTexture->GetWidth(), m_pTargetTexture->GetHeight());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	return true;
}

void RenderTarget_Impl::EndRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_pDevice->GetWidth(), g_pDevice->GetHeight());
}

bool RenderTarget_Impl::CreateRenderTarget()
{
	glGenFramebuffers(1, &m_nFrameBuffer);
	if (m_nFrameBuffer == 0) return false;

	return true;
}

void RenderTarget_Impl::DestroyRenderTarget()
{
	if (m_nFrameBuffer == 0) return;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	DestroyDepthBuffer();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	m_pTargetTexture = NULL;

	glDeleteFramebuffers(1, &m_nFrameBuffer);
	m_nFrameBuffer = 0;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RenderTarget_Impl::CreateDepthBuffer(uint width, uint height)
{
	if (m_nDepthBufferWidth == width && m_nDepthBufferHeight == height) return true;
	DestroyDepthBuffer();

	m_nDepthBufferWidth = width;
	m_nDepthBufferHeight = height;

	glGenRenderbuffers(1, &m_nDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBuffer);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_nDepthBufferWidth, m_nDepthBufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBuffer);

	return true;
}

void RenderTarget_Impl::DestroyDepthBuffer()
{
	if (m_nDepthBuffer == 0) return;

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glDeleteFramebuffers(1, &m_nDepthBuffer);

	m_nDepthBufferWidth = 0;
	m_nDepthBufferHeight = 0;
}
