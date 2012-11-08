/*!
 * \file ShadowMap.cpp
 * \date 11-02-2012 11:21:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "ShadowMap.h"
#include <util/AppUtil.h>
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
	if (!IApp::Initialize()) return false;

	m_pShaderDepth = g_pResMgr->CreateShader(_("shaders/depth_mapping.xml"));
	if (!m_pShaderDepth) return false;

	m_pShaderShadowMap = g_pResMgr->CreateShader(_("shaders/shadow_mapping.xml"));
	if (!m_pShaderShadowMap) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("scene.mesh"));
	if (!m_pMesh) return false;

	m_pDepthTexture = g_pResMgr->CreateTexture2D(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, TEXTURE_FORMAT::TF_DEPTH);
	if (!m_pDepthTexture) return false;

	m_pRenderTarget = g_pResMgr->CreateRenderTarget(NULL, m_pDepthTexture);
	if (!m_pRenderTarget) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);
	m_pTargetCameraCtrl->BindMouseEvent();

	Picture* pPicture = new Picture(g_pUiSystemMgr->GetCurrScreen());
	pPicture->SetTexture(m_pDepthTexture);
	pPicture->SetSize(256.0f, 256.0f);

	m_mat4DepthBias.Reset(0.5f, 0.0f, 0.0f, 0.0f,
						 0.0f, 0.5f, 0.0f, 0.0f,
						 0.0f, 0.0f, 0.5f, 0.0f,
						 0.5f, 0.5f, 0.5f, 1.0f);

	m_vLightPos.Reset(10.0f, 5.0f, 10.0f);
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
	IApp::Terminate();
}

void ShadowMap::Update(float dt)
{
	static float s_TotalTime = 0.0f;

	s_TotalTime += dt*0.5f;
	if (s_TotalTime > Math::PIX2) s_TotalTime -= Math::PIX2;

	m_vLightPos.x = sinf(s_TotalTime)*10.0f;
	m_vLightPos.z = cosf(s_TotalTime)*10.0f;

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

void ShadowMap::RenderDepth()
{
	g_pDevice->SetCullMode(CULL_MODE::CM_FRONT);

	m_pRenderTarget->BeginRender();
	m_pShaderDepth->BeginRender();

	Matrix4x4 mat4DepthProj;
	Math::BuildOrthoMatrix(mat4DepthProj, -5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 20.0f);

	Matrix4x4 mat4View;
	Math::BuildLookAtMatrix(mat4View, m_vLightPos, Math::VEC3_ZERO, Math::VEC3_UP);

	Matrix4x4 mat4DepthModelViewProj = mat4DepthProj*mat4View;

	m_mat4DepthBiasModelViewProj = m_mat4DepthBias*mat4DepthModelViewProj;

	m_pShaderDepth->SetMatrix4x4(_("u_matDepthModelViewProj"), mat4DepthModelViewProj);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShaderDepth->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShaderDepth->EndRender();
	m_pRenderTarget->EndRender();

	g_pDevice->SetCullMode(CULL_MODE::CM_BACK);
}

void ShadowMap::RenderScene()
{
	m_pShaderShadowMap->BeginRender();

	const Matrix4x4& mat4View = m_pCamera->GetViewMatrix();
	const Matrix4x4& mat4Proj = m_pCamera->GetProjectionMatrix();

	m_pShaderShadowMap->SetMatrix3x3(_("u_matView"), Math::MAT3_IDENTITY);

	Matrix3x3 mat3View;
	Math::GetSubMatrix(mat3View, mat4View);
	m_pShaderShadowMap->SetMatrix3x3(_("u_matModelView"), mat3View);

	Matrix4x4 matWorldViewProj = mat4Proj*mat4View;
	m_pShaderShadowMap->SetMatrix4x4(_("u_matModelViewProj"), matWorldViewProj);

	Matrix3x3 mat3Normal = mat3View;
	mat3Normal.Invert().Transpose();
	m_pShaderShadowMap->SetMatrix3x3(_("u_matNormal"), mat3Normal);

	m_pShaderShadowMap->SetMatrix4x4(_("u_matDepthBiasModelViewProj"), m_mat4DepthBiasModelViewProj);

	m_pShaderShadowMap->SetVector3(_("u_vLightPosition"), m_vLightPos);

	m_pShaderShadowMap->SetTexture(_("u_shadowMap"), m_pDepthTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShaderShadowMap->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShaderShadowMap->EndRender();
}
