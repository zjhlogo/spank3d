/*!
 * \file ShadowMap.cpp
 * \date 11-02-2012 11:21:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ShadowMap.h"
#include <util/AppUtil.h>
#include <event/EventIds.h>
#include <ui/controls/Picture.h>

IMPLEMENT_APP(ShadowMap);

ShadowMap::ShadowMap()
{
	m_pShaderDepth = NULL;
	m_pShaderShadowMap = NULL;
	m_pMesh = NULL;

	m_pRenderTarget = NULL;
	m_pDepthTexture = NULL;

	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

ShadowMap::~ShadowMap()
{
	// TODO: 
}

bool ShadowMap::Initialize()
{
	m_pShaderDepth = g_pResMgr->CreateShader(_("depth_shader.xml"));
	if (!m_pShaderDepth) return false;

	m_pShaderShadowMap = g_pResMgr->CreateShader(_("shadow_mapping_shader.xml"));
	if (!m_pShaderShadowMap) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("scene.mesh"));
	if (!m_pMesh) return false;

	m_pDepthTexture = g_pResMgr->CreateTexture2D(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, TEXTURE_FORMAT::TF_DEPTH);
	if (!m_pDepthTexture) return false;

	m_pRenderTarget = g_pResMgr->CreateRenderTarget(NULL, m_pDepthTexture);
	if (!m_pRenderTarget) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);

	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, FUNC_HANDLER(&ShadowMap::OnMouseEvent));

	Picture* pPicture = new Picture(g_pUiSystemMgr->GetCurrScreen());
	pPicture->SetTexture(m_pDepthTexture);

	m_matDepthBias.Reset(0.5f, 0.0f, 0.0f, 0.0f,
						 0.0f, 0.5f, 0.0f, 0.0f,
						 0.0f, 0.0f, 0.5f, 0.0f,
						 0.5f, 0.5f, 0.5f, 1.0f);

	return true;
}

void ShadowMap::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);

	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pDepthTexture);

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShaderShadowMap);
	SAFE_RELEASE(m_pShaderDepth);
}

void ShadowMap::Update(float dt)
{
	g_pUiSystemMgr->Update(dt);
}

void ShadowMap::Render()
{
	// render depth to texture
	RenderDepth();

	// render scene
	RenderScene();

	g_pUiSystemMgr->Render();
	g_pUiRenderer->FlushAll();
}

bool ShadowMap::OnMouseEvent(MouseEvent& mouseEvent)
{
	return m_pTargetCameraCtrl->HandleMouseEvent(mouseEvent);
}

void ShadowMap::RenderDepth()
{
	m_pRenderTarget->BeginRender();
	m_pShaderDepth->BeginRender();

	Matrix4x4 matDepthProj;
	Math::BuildOrthoMatrix(matDepthProj, -5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 20.0f);

	Matrix4x4 matView;
	Math::BuildLookAtMatrix(matView, Vector3(0.0f, 5.0f, 10.0f), Math::VEC3_ZERO, Math::VEC3_UP);

	Matrix4x4 matDepthModelViewProj = matDepthProj*matView;

	m_matDepthBiasModelViewProj = m_matDepthBias*matDepthModelViewProj;

	m_pShaderDepth->SetMatrix4x4(_("u_matModelViewProj"), matDepthModelViewProj);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShaderDepth->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShaderDepth->EndRender();
	m_pRenderTarget->EndRender();
}

void ShadowMap::RenderScene()
{
	m_pShaderShadowMap->BeginRender();

	Matrix3x3 matModelView;
	Math::GetSubMatrix(matModelView, m_pCamera->GetViewMatrix());
	m_pShaderShadowMap->SetMatrix3x3(_("u_matModelView"), matModelView);

	Matrix4x4 matModelViewProj = m_pCamera->GetProjectionMatrix()*m_pCamera->GetViewMatrix();
	m_pShaderShadowMap->SetMatrix4x4(_("u_matModelViewProj"), matModelViewProj);

	Matrix3x3 matNormal;
	Math::GetSubMatrix(matNormal, m_pCamera->GetViewMatrix());
	matNormal.Invert().Transpose();
	m_pShaderShadowMap->SetMatrix3x3(_("u_matNormal"), matNormal);

	m_pShaderShadowMap->SetMatrix4x4(_("u_matDepthBiasModelViewProj"), m_matDepthBiasModelViewProj);

	m_pShaderShadowMap->SetTexture(_("u_shadowMap"), m_pDepthTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShaderShadowMap->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShaderShadowMap->EndRender();
}
