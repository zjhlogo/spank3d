/*!
 * \file CubeMap.cpp
 * \date 10-29-2012 11:12:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "CubeMap.h"
#include <util/AppUtil.h>

IMPLEMENT_APP(CubeMap);

CubeMap::CubeMap()
{
	m_pShader = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

CubeMap::~CubeMap()
{
	// TODO: 
}

bool CubeMap::Initialize()
{
	if (!IApp::Initialize()) return false;

	m_pShader = g_pResMgr->CreateShader(_("shaders/cube_mapping.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("teapot.mesh"));
	if (!m_pMesh) return false;

	m_pTexture = g_pResMgr->CreateTextureCube(_("brightday2.xml"));
	if (!m_pTexture) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);
	m_pTargetCameraCtrl->BindMouseEvent();

	return true;
}

void CubeMap::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	IApp::Terminate();
}

void CubeMap::Update(float dt)
{
	// TODO: 
}

void CubeMap::Render()
{
	m_pShader->BeginRender();

	const Matrix4x4& matView = m_pCamera->GetViewMatrix();
	const Matrix4x4& matProj = m_pCamera->GetProjectionMatrix();

	Matrix3x3 matModelView;
	Math::GetSubMatrix(matModelView, matView);
	m_pShader->SetMatrix3x3(_("u_matModelView"), matModelView);

	m_pShader->SetMatrix4x4(_("u_matModelViewProj"), matProj*matView);

	Matrix3x3 matNormal;
	Math::GetSubMatrix(matNormal, matView);
	matNormal.Invert().Transpose();
	m_pShader->SetMatrix3x3(_("u_matNormal"), matNormal);

	m_pShader->SetVector3(_("u_vLightPosition"), Vector3(0.0f, 0.0f, 4.0f));

	m_pShader->SetTexture(_("u_texEnvMap"), m_pTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}
