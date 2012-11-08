/*!
 * \file HelloWorld.cpp
 * \date 4-14-2012 21:46:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "HelloWorld.h"
#include <util/AppUtil.h>

IMPLEMENT_APP(HelloWorld);

HelloWorld::HelloWorld()
{
	m_pShader = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

HelloWorld::~HelloWorld()
{
	// TODO: 
}

bool HelloWorld::Initialize()
{
	if (!IApp::Initialize()) return false;

	m_pShader = g_pResMgr->CreateShader(_("shaders/default.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("test.mesh"));
	if (!m_pMesh) return false;

	m_pTexture = g_pResMgr->CreateTexture2D(_("white256.png"));
	if (!m_pTexture) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);
	m_pTargetCameraCtrl->BindMouseEvent();

	return true;
}

void HelloWorld::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	IApp::Terminate();
}

void HelloWorld::Update(float dt)
{
	// TODO: 
}

void HelloWorld::Render()
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

	m_pShader->SetTexture(_("u_texture"), m_pTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}
