/*!
 * \file Brick.cpp
 * \date 10-25-2012 13:07:35
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Brick.h"
#include <util/AppUtil.h>
#include <event/EventIds.h>

IMPLEMENT_APP(Brick);

Brick::Brick()
{
	m_pShader = NULL;
	m_pMesh = NULL;
	m_pCamera = NULL;
	m_pTargetCameraCtrl = NULL;
}

Brick::~Brick()
{
	// TODO: 
}

bool Brick::Initialize()
{
	m_pShader = g_pResMgr->CreateShader(_("shaders/brick.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("teapot.mesh"));
	if (!m_pMesh) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);

	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, FUNC_HANDLER(&Brick::OnMouseEvent));

	return true;
}

void Brick::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
}

void Brick::Update(float dt)
{
	// TODO: 
}

void Brick::Render()
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

	m_pShader->SetVector3(_("u_vLightPosition"), Vector3(0.0f, 0.0f, 10.0f));

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}

bool Brick::OnMouseEvent(MouseEvent& mouseEvent)
{
	return m_pTargetCameraCtrl->HandleMouseEvent(mouseEvent);
}
