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

	m_pLblZoom = NULL;
	m_zoom = 1.0f;

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

	// zoom
	PushButton* pBtnDecZoom = new PushButton(pMainScreen);
	pBtnDecZoom->SetPosition(10.0f, 60.0f);
	pBtnDecZoom->SetSize(20.0f, 20.0f);
	pBtnDecZoom->SetLabel(_T("-"));
	pBtnDecZoom->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnBtnDecZoomDown);

	m_pLblZoom = new Label(pMainScreen);
	m_pLblZoom->SetPosition(40.0f, 60.0f);
	m_pLblZoom->SetSize(100.0f, 20.0f);

	PushButton* pBtnIncZoom = new PushButton(pMainScreen);
	pBtnIncZoom->SetPosition(150.0f, 60.0f);
	pBtnIncZoom->SetSize(20.0f, 20.0f);
	pBtnIncZoom->SetLabel(_T("+"));
	pBtnIncZoom->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnBtnIncZoomDown);

	UpdateIterations(m_maxIterations);
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

bool Mandelbrot::OnBtnDecZoomDown(MouseEvent& event)
{
	UpdateZoom(m_zoom/2.0f);
	return true;
}

bool Mandelbrot::OnBtnIncZoomDown(MouseEvent& event)
{
	UpdateZoom(m_zoom*2.0f);
	return true;
}

void Mandelbrot::UpdateZoom(float zoom)
{
	m_zoom = zoom;

	tstring strLabel;
	StringUtil::strformat(strLabel, _T("Zoom: %f"), m_zoom);
	m_pLblZoom->SetLabel(strLabel);
}
