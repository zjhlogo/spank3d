/*!
 * \file RenderTarget.cpp
 * \date 11-01-2012 14:39:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "RenderTarget.h"
#include <util/AppUtil.h>
#include <event/EventIds.h>
#include <ui/controls/Picture.h>

IMPLEMENT_APP(RenderTarget);

RenderTarget::RenderTarget()
{
	m_pShader = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;

	m_pRenderTarget = NULL;
	m_pRenderTargetTexture = NULL;

	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

RenderTarget::~RenderTarget()
{
	// TODO: 
}

bool RenderTarget::Initialize()
{
	m_pShader = g_pResMgr->CreateShader(_("render_target_shader.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("box.mesh"));
	if (!m_pMesh) return false;

	m_pTexture = g_pResMgr->CreateTexture2D(_("grid16.png"));
	if (!m_pTexture) return false;

	m_pRenderTarget = g_pResMgr->CreateRenderTarget();
	if (!m_pRenderTarget) return false;

	m_pRenderTargetTexture = g_pResMgr->CreateTexture2D(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, 32);
	if (!m_pRenderTargetTexture) return false;

	if (!m_pRenderTarget->SetTargetTexture(m_pRenderTargetTexture)) return false;

	Math::BuildPerspectiveFovMatrix(m_matRenderTargetProj, 45.0f, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, 0.1f, 100.0f);

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);

	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, FUNC_HANDLER(&RenderTarget::OnMouseEvent));

	Picture* pPicture = new Picture(g_pUiSystemMgr->GetCurrScreen());
	pPicture->SetTexture(m_pRenderTargetTexture);

	return true;
}

void RenderTarget::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pTexture);

	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pRenderTargetTexture);

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
}

void RenderTarget::Update(float dt)
{
	// TODO: 
	g_pUiSystemMgr->Update(dt);
}

void RenderTarget::Render()
{
	m_pRenderTarget->BeginRender();
	// render to texture
	RenderScene(m_matRenderTargetProj, m_pTexture);
	m_pRenderTarget->EndRender();

	// render scene
	RenderScene(m_pCamera->GetProjectionMatrix(), m_pRenderTargetTexture);

	g_pUiSystemMgr->Render();
	g_pUiRenderer->FlushAll();
}

bool RenderTarget::OnMouseEvent(MouseEvent& mouseEvent)
{
	return m_pTargetCameraCtrl->HandleMouseEvent(mouseEvent);
}

void RenderTarget::RenderScene(const Matrix4x4& matProj, ITexture* pTexture)
{
	m_pShader->BeginRender();

	const Matrix4x4& matView = m_pCamera->GetViewMatrix();
	Matrix4x4 matWorldViewProj = matProj*matView;
	m_pShader->SetMatrix4x4(_("u_matModelViewProj"), matWorldViewProj);

	m_pShader->SetTexture(_("u_texture"), pTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}
