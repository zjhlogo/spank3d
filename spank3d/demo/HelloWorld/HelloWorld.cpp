/*!
 * \file HelloWorld.cpp
 * \date 4-14-2012 21:46:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "HelloWorld.h"
#include <util/AppUtil.h>
#include <event/EventIds.h>

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
	m_pShader = g_pResMgr->CreateShader("default_shader.xml");
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh("teapot.mesh");
	if (!m_pMesh) return false;

	m_pTexture = g_pResMgr->CreateTexture("grid16.png");
	if (!m_pTexture) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC_ZERO);

	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, FUNC_HANDLER(&HelloWorld::OnMouseEvent));

	return true;
}

void HelloWorld::Terminate()
{
	SAFE_DELETE(m_pTargetCameraCtrl);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
}

void HelloWorld::Update(float dt)
{
	m_pShader->Commit();

	const Matrix4x4& matView = m_pCamera->GetViewMatrix();

	Matrix4x4 matProj;
 	Math::BuildPerspectiveFovMatrix(matProj, 45.0f, g_pDevice->GetWindowWidth(), g_pDevice->GetWindowHeight(), 0.1f, 100.0f);

	Matrix4x4 matWorldViewProj = matProj*matView;
	m_pShader->SetMatrix4x4(matWorldViewProj, "u_matModelViewProj");

	m_pShader->SetTexture(m_pTexture, "u_texture");

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}
}

bool HelloWorld::OnMouseEvent(MouseEvent& mouseEvent)
{
	return m_pTargetCameraCtrl->HandleMouseEvent(mouseEvent);
}
