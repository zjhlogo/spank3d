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

RenderTarget_Impl::RenderTarget_Impl(ITexture* pColorTexture, ITexture* pDepthTexture)
{
	InitMember();
	SetOk(CreateRenderTarget(pColorTexture, pDepthTexture));
}

RenderTarget_Impl::~RenderTarget_Impl()
{
	DestroyRenderTarget();
	DispatchEvent(Event(EID_OBJECT_DESTROYED));
}

void RenderTarget_Impl::InitMember()
{
	m_nFrameBuffer = 0;
	m_nDepthBuffer = 0;
	m_nClearType = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	m_pColorTexture = NULL;
	m_pDepthTexture = NULL;
}

ITexture* RenderTarget_Impl::GetColorTexture()
{
	return m_pColorTexture;
}

ITexture* RenderTarget_Impl::GetDepthTexture()
{
	return m_pDepthTexture;
}

bool RenderTarget_Impl::BeginRender()
{
	if (m_nFrameBuffer == 0) return false;
	if (!m_pColorTexture) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);
	glViewport(0, 0, m_pColorTexture->GetWidth(), m_pColorTexture->GetHeight());

	glClear(m_nClearType);	// Clear Screen And Depth Buffer

	return true;
}

void RenderTarget_Impl::EndRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, g_pDevice->GetWidth(), g_pDevice->GetHeight());
}

bool RenderTarget_Impl::CreateRenderTarget(ITexture* pColorTexture, ITexture* pDepthTexture)
{
	if (pColorTexture && pDepthTexture)
	{
		return CreateColorDepthTextureBuffer(pColorTexture, pDepthTexture);
	}
	else if (pColorTexture && !pDepthTexture)
	{
		return CreateColorTextureBuffer(pColorTexture);
	}
	else if (!pColorTexture && pDepthTexture)
	{
		return CreateDepthTextureBuffer(pDepthTexture);
	}

	return false;
}

void RenderTarget_Impl::DestroyRenderTarget()
{
	if (m_nFrameBuffer == 0) return;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	if (m_nDepthBuffer != 0)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glDeleteFramebuffers(1, &m_nDepthBuffer);
		m_nDepthBuffer = 0;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	m_pColorTexture = NULL;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	m_pDepthTexture = NULL;

	glDeleteFramebuffers(1, &m_nFrameBuffer);
	m_nFrameBuffer = 0;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RenderTarget_Impl::CreateColorTextureBuffer(ITexture* pColorTexture)
{
	glGenFramebuffers(1, &m_nFrameBuffer);
	if (m_nFrameBuffer == 0) return false;

	if (pColorTexture->GetType() != TEXTURE_TYPE::TT_TEXTURE_2D) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	// color texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pColorTexture->GetHandler(), 0);

	// depth buffer
	glGenRenderbuffers(1, &m_nDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, pColorTexture->GetWidth(), pColorTexture->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBuffer);

	// draw color attachment
	GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBufs);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pColorTexture = pColorTexture;
	m_nClearType = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	return true;
}

bool RenderTarget_Impl::CreateDepthTextureBuffer(ITexture* pDepthTexture)
{
	glGenFramebuffers(1, &m_nFrameBuffer);
	if (m_nFrameBuffer == 0) return false;

	if (pDepthTexture->GetType() != TEXTURE_TYPE::TT_TEXTURE_2D) return false;
	if (pDepthTexture->GetFormat() != TEXTURE_FORMAT::TF_DEPTH) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	// depth texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pDepthTexture->GetHandler(), 0);

	// draw none
	GLenum drawBufs[] = {GL_NONE};
	glDrawBuffers(1, drawBufs);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pDepthTexture = pDepthTexture;
	m_nClearType = GL_DEPTH_BUFFER_BIT;
	return true;
}

bool RenderTarget_Impl::CreateColorDepthTextureBuffer(ITexture* pColorTexture, ITexture* pDepthTexture)
{
	glGenFramebuffers(1, &m_nFrameBuffer);
	if (m_nFrameBuffer == 0) return false;

	if (pColorTexture->GetType() != TEXTURE_TYPE::TT_TEXTURE_2D) return false;
	if (pDepthTexture->GetType() != TEXTURE_TYPE::TT_TEXTURE_2D) return false;
	if (pDepthTexture->GetFormat() != TEXTURE_FORMAT::TF_DEPTH) return false;

	if (pColorTexture->GetWidth() != pDepthTexture->GetWidth()
		|| pColorTexture->GetHeight() != pDepthTexture->GetHeight()) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	// color texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pColorTexture->GetHandler(), 0);

	// depth texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pDepthTexture->GetHandler(), 0);

	// draw color attachment
	GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBufs);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pColorTexture = pColorTexture;
	m_pDepthTexture = pDepthTexture;
	m_nClearType = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	return true;
}
