/*!
 * \file Mandelbrot.cpp
 * \date 10-27-2012 15:30:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Mandelbrot.h"
#include <util/AppUtil.h>
#include <ui/controls/PushButton.h>
#include <ui/controls/Label.h>
#include <util/StringUtil.h>

IMPLEMENT_APP(Mandelbrot);

Mandelbrot::Mandelbrot()
{
	m_pShader = NULL;

	m_pLblMaxIterations = NULL;
	m_maxIterations = 1;

	m_zoom = 1.0f;
	m_bMouseDown = false;
}

Mandelbrot::~Mandelbrot()
{
	// TODO: 
}

bool Mandelbrot::Initialize()
{
	if (!IApp::Initialize()) return false;

	m_pShader = g_pResMgr->CreateShader(_("shaders/mandelbrot.xml"));
	if (!m_pShader) return false;

	Math::BuildOrthoMatrix(m_matModelViewProj, 5.0f, 3.75f, -100.0f, 100.0f);

	Screen* pMainScreen = g_pUiSystemMgr->GetCurrScreen();

	// max iterations
	PushButton* pBtnDecIterations = new PushButton(pMainScreen);
	pBtnDecIterations->SetPosition(10.0f, 30.0f);
	pBtnDecIterations->SetSize(20.0f, 20.0f);
	pBtnDecIterations->SetLabel(_T("-"));
	pBtnDecIterations->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnBtnDecIterationsDown);

	m_pLblMaxIterations = new Label(pMainScreen);
	m_pLblMaxIterations->SetPosition(40.0f, 30.0f);
	m_pLblMaxIterations->SetSize(100.0f, 20.0f);

	PushButton* pBtnIncIterations = new PushButton(pMainScreen);
	pBtnIncIterations->SetPosition(150.0f, 30.0f);
	pBtnIncIterations->SetSize(20.0f, 20.0f);
	pBtnIncIterations->SetLabel(_T("+"));
	pBtnIncIterations->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnBtnIncIterationsDown);

	UpdateIterations(m_maxIterations);

	g_pDevice->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnMouseDown);
	g_pDevice->RegisterEvent(MouseEvent::LBUTTON_UP, this, (FUNC_HANDLER)&Mandelbrot::OnMouseUp);
	g_pDevice->RegisterEvent(MouseEvent::MOUSE_MOVE, this, (FUNC_HANDLER)&Mandelbrot::OnMouseMove);
	g_pDevice->RegisterEvent(MouseEvent::MOUSE_WHEEL, this, (FUNC_HANDLER)&Mandelbrot::OnMouseWheel);

	return true;
}

void Mandelbrot::Terminate()
{
	SAFE_RELEASE(m_pShader);
	IApp::Terminate();
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

	m_pShader->SetFloat(_("u_maxIterations"), float(m_maxIterations));
	m_pShader->SetFloat(_("u_zoom"), float(m_zoom));
	m_pShader->SetVector2(_("u_centerPos"), m_centerPos);

	m_pShader->DrawTriangleList(s_Verts, 4, s_Indis, 6);

	m_pShader->EndRender();
}

bool Mandelbrot::OnBtnDecIterationsDown(MouseEvent& event)
{
	UpdateIterations(m_maxIterations-1);
	return true;
}

bool Mandelbrot::OnBtnIncIterationsDown(MouseEvent& event)
{
	UpdateIterations(m_maxIterations+1);
	return true;
}

void Mandelbrot::UpdateIterations(int iterations)
{
	m_maxIterations = iterations;
	if (m_maxIterations < 1) m_maxIterations = 1;

	tstring strLabel;
	StringUtil::strformat(strLabel, _T("Max Iterations: %d"), m_maxIterations);
	m_pLblMaxIterations->SetLabel(strLabel);
}

bool Mandelbrot::OnMouseDown(MouseEvent& event)
{
	m_bMouseDown = true;
	return true;
}

bool Mandelbrot::OnMouseUp(MouseEvent& event)
{
	m_bMouseDown = false;
	return true;
}

bool Mandelbrot::OnMouseMove(MouseEvent& event)
{
	if (m_bMouseDown)
	{
		const Vector2& offset = event.GetOffset();
		m_centerPos.x -= (offset.x / g_pDevice->GetSize().y * m_zoom * 3.75f);
		m_centerPos.y += (offset.y / g_pDevice->GetSize().y * m_zoom * 3.75f);
	}

	return true;
}

bool Mandelbrot::OnMouseWheel(MouseEvent& event)
{
	if (event.GetWheelDetail() < 0)
	{
		m_zoom *= 2.0f;
	}
	else
	{
		m_zoom *= 0.5f;
	}

	return true;
}
