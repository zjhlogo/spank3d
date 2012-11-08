/*!
 * \file RenderTarget.cpp
 * \date 11-01-2012 14:39:21
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "RenderTarget.h"
#include <util/AppUtil.h>
#include <ui/controls/Picture.h>

IMPLEMENT_APP(RenderTarget);

RenderTarget::RenderTarget()
{
	m_pShader = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;

	m_pRenderTarget = NULL;
	m_pColorTexture = NULL;

	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

RenderTarget::~RenderTarget()
{
	// TODO: 
}

bool RenderTarget::Initialize()
{
	if (!IApp::Initialize()) return false;

	m_pShader = g_pResMgr->CreateShader(_("shaders/default.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("box.mesh"));
	if (!m_pMesh) return false;

	m_pTexture = g_pResMgr->CreateTexture2D(_("grid16.png"));
	if (!m_pTexture) return false;

	m_pColorTexture = g_pResMgr->CreateTexture2D(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, TEXTURE_FORMAT::TF_RGBA);
	if (!m_pColorTexture) return false;

	m_pRenderTarget = g_pResMgr->CreateRenderTarget(m_pColorTexture, NULL);
	if (!m_pRenderTarget) return false;

	Math::BuildPerspectiveFovMatrix(m_matRenderTargetProj, 45.0f, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT, 0.1f, 100.0f);

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);
	m_pTargetCameraCtrl->BindMouseEvent();

	Picture* pPicture = new Picture(g_pUiSystemMgr->GetCurrScreen());
	pPicture->SetTexture(m_pColorTexture);

	return true;
}

void RenderTarget::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pTexture);

	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pColorTexture);

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);

	IApp::Terminate();
}

void RenderTarget::Update(float dt)
{

}

void RenderTarget::Render()
{
	m_pRenderTarget->BeginRender();
	// render to texture
	RenderScene(m_matRenderTargetProj, m_pTexture);
	m_pRenderTarget->EndRender();

	// render scene
	RenderScene(m_pCamera->GetProjectionMatrix(), m_pColorTexture);
}

void RenderTarget::RenderScene(const Matrix4x4& matProj, ITexture* pTexture)
{
	m_pShader->BeginRender();

	const Matrix4x4& mat4View = m_pCamera->GetViewMatrix();
	const Matrix4x4& mat4Proj = m_pCamera->GetProjectionMatrix();

	m_pShader->SetMatrix3x3(_("u_matView"), Math::MAT3_IDENTITY);

	Matrix3x3 mat3View;
	Math::GetSubMatrix(mat3View, mat4View);
	m_pShader->SetMatrix3x3(_("u_matModelView"), mat3View);

	Matrix4x4 matWorldViewProj = mat4Proj*mat4View;
	m_pShader->SetMatrix4x4(_("u_matModelViewProj"), matWorldViewProj);

	Matrix3x3 mat3Normal = mat3View;
	mat3Normal.Invert().Transpose();
	m_pShader->SetMatrix3x3(_("u_matNormal"), mat3Normal);

	m_pShader->SetVector3(_("u_vLightPosition"), Vector3(0.0f, 5.0f, 10.0f));

	m_pShader->SetTexture(_("u_texture"), pTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}
