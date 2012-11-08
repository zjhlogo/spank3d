/*!
 * \file BasicShader.cpp
 * \date 11-05-2012 10:58:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "BasicShader.h"
#include <util/AppUtil.h>

IMPLEMENT_APP(BasicShader);

BasicShader::BasicShader()
{
	m_pShader = NULL;
	m_pMesh = NULL;
	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

BasicShader::~BasicShader()
{
	// TODO: 
}

bool BasicShader::Initialize()
{
	if (!IApp::Initialize()) return false;

	m_pShader = g_pResMgr->CreateShader(_("shaders/diffuse.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("teapot.mesh"));
	if (!m_pMesh) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);
	m_pTargetCameraCtrl->BindMouseEvent();

	return true;
}

void BasicShader::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	IApp::Terminate();
}

void BasicShader::Update(float dt)
{
	// TODO: 
}

void BasicShader::Render()
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

	m_pShader->SetVector3(_("u_vLightPosition"), Vector3(-10.0f, 10.0f, 10.0f));

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}
