/*!
 * \file Mandelbrot.cpp
 * \date 10-27-2012 15:30:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Mandelbrot.h"
#include <util/AppUtil.h>

IMPLEMENT_APP(Mandelbrot);

Mandelbrot::Mandelbrot()
{
	m_pShader = NULL;
}

Mandelbrot::~Mandelbrot()
{
	// TODO: 
}

bool Mandelbrot::Initialize()
{
	m_pShader = g_pResMgr->CreateShader(_("mandelbrot_shader.xml"));
	if (!m_pShader) return false;

	Math::BuildOrthoMatrix(m_matModelViewProj, 5.0f, 3.75f, -100.0f, 100.0f);

	return true;
}

void Mandelbrot::Terminate()
{
	SAFE_RELEASE(m_pShader);
}

void Mandelbrot::Update(float dt)
{
	// TODO: 
}

void Mandelbrot::Render()
{
	static const float s_Verts[] = 
	{
		-2.5f, -1.875f, 0.0f,
		-2.5f, +1.875f, 0.0f,
		+2.5f, -1.875f, 0.0f,
		+2.5f, +1.875f, 0.0f,
	};

	static const ushort s_Indis[] = {0, 2, 1, 1, 2, 3};

	m_pShader->BeginRender();

	m_pShader->SetMatrix3x3(_("u_matModelView"), Math::MAT3_IDENTITY);
	m_pShader->SetMatrix4x4(_("u_matModelViewProj"), m_matModelViewProj);

	m_pShader->DrawTriangleList(s_Verts, 4, s_Indis, 6);

	m_pShader->EndRender();
}
