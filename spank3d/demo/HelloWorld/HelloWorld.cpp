/*!
 * \file HelloWorld.cpp
 * \date 4-14-2012 21:46:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "HelloWorld.h"
#include <util/AppUtil.h>
#include <gl/glew.h>

IMPLEMENT_APP(HelloWorld);

HelloWorld::HelloWorld()
{
	m_fRotY = 0.0f;
	m_pShader = NULL;
	m_pMesh = NULL;
}

HelloWorld::~HelloWorld()
{
	// TODO: 
}

bool HelloWorld::Initialize()
{
	m_pShader = g_pRenderInterface->CreateShader("default_shader.xml");
	if (!m_pShader) return false;

	m_pMesh = g_pResMgr->CreateMesh("box.mesh");
	if (!m_pMesh) return false;

	return true;
}

void HelloWorld::Terminate()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
}

void HelloWorld::Update(float dt)
{
 	m_pShader->Commit();

	Matrix4x4 matWorld;
	Math::BuildRotationYMatrix(matWorld, m_fRotY);

	Matrix4x4 matView;
	Math::BuildLookAtMatrix(matView, Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));

	Matrix4x4 matProj;
	Math::BuildPerspectiveFovMatrix(matProj, Math::Radians(45.0f), (float)1024/(float)768, 0.1f, 100.0f);

	Matrix4x4 matWorldViewProj = matProj*matView*matWorld;
	m_pShader->SetMatrix4x4(matWorldViewProj, "u_matModelViewProj");

	for (int i = 0; i < m_pMesh->GetNumPieces(); ++i)
	{
		IMeshPiece* pMeshPiece = m_pMesh->GetPiece(i);
		m_pShader->DrawTriangleList(pMeshPiece->GetVerts(), pMeshPiece->GetNumVerts(), pMeshPiece->GetIndis(), pMeshPiece->GetNumIndis());
	}

	m_fRotY += dt;
}
