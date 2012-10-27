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
	m_pShader = g_pResMgr->CreateShader(_("default_shader.xml"));
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh(_("teapot.mesh"));
	if (!m_pMesh) return false;

	m_pTexture = g_pResMgr->CreateTexture(_("grid16.png"));
	if (!m_pTexture) return false;

	m_pCamera = new ICamera();
	m_pTargetCameraCtrl = new TargetCameraControl(m_pCamera, Vector3(0.0f, 0.0f, 10.0f), Math::VEC3_ZERO);

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
	// TODO: 
}

void HelloWorld::Render()
{
	m_pShader->BeginRender();

	const Matrix4x4& matView = m_pCamera->GetViewMatrix();
	const Matrix4x4& matProj = m_pCamera->GetProjectionMatrix();

	Matrix4x4 matWorldViewProj = matProj*matView;
	m_pShader->SetMatrix4x4(_("u_matModelViewProj"), matWorldViewProj);

	m_pShader->SetTexture(_("u_texture"), m_pTexture);

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_pShader->EndRender();
}

bool HelloWorld::OnMouseEvent(MouseEvent& mouseEvent)
{
	return m_pTargetCameraCtrl->HandleMouseEvent(mouseEvent);
}
