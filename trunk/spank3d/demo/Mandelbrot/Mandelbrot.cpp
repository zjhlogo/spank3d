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
#include <ui/controls/HContainer.h>
#include <util/StringUtil.h>

IMPLEMENT_APP(Mandelbrot);

Mandelbrot::Mandelbrot()
{
	m_pShader = NULL;

	m_pLblMaxIterations = NULL;
	m_maxIterations = 128;

	m_pLblZoom = NULL;
	m_pLblCenterPos = NULL;
	m_pVContainer = NULL;

	m_zoom = 1.0f;
	m_destZoom = 1.0f;
	m_zoomSpeed = 0.0f;
	m_bZooming = false;

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

	m_pVContainer = new VContainer(pMainScreen);
	m_pVContainer->SetPosition(10.0f, 30.0f);

	// max iterations
	HContainer* pHContanerIteration = new HContainer(m_pVContainer);
	PushButton* pBtnDecIterations = new PushButton(pHContanerIteration);
	pBtnDecIterations->SetSize(20.0f, 20.0f);
	pBtnDecIterations->SetLabel(_T("-"));
	pBtnDecIterations->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnBtnDecIterationsDown);
	m_pLblMaxIterations = new Label(pHContanerIteration, _T("Max Iterations: 00000"));
	PushButton* pBtnIncIterations = new PushButton(pHContanerIteration);
	pBtnIncIterations->SetSize(20.0f, 20.0f);
	pBtnIncIterations->SetLabel(_T("+"));
	pBtnIncIterations->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&Mandelbrot::OnBtnIncIterationsDown);
	pHContanerIteration->ReLayout();

	// zoom
	m_pLblZoom = new Label(m_pVContainer);

	// center pos
	m_pLblCenterPos = new Label(m_pVContainer);

	UpdateIterations(m_maxIterations);
	UpdateUi();

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
	if (m_bZooming)
	{
		m_zoom += (m_zoomSpeed*dt);
		m_centerPos += (m_centerPosSpeed*dt);

		if (m_zoomSpeed > 0.0f)
		{
			if (m_zoom >= m_destZoom) m_bZooming = false;
		}
		else
		{
			if (m_zoom <= m_destZoom) m_bZooming = false;
		}

		if (!m_bZooming)
		{
			m_zoomSpeed = 0.0f;
			m_zoom = m_destZoom;

			m_centerPosSpeed.Reset();
			m_centerPos = m_destCenterPos;
		}
	}
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
	m_pShader->SetFloat(_("u_zoom"), m_zoom);
	m_pShader->SetVector2(_("u_centerPos"), m_centerPos);

	m_pShader->DrawTriangleList(s_Verts, 4, s_Indis, 6);

	m_pShader->EndRender();
}

bool Mandelbrot::OnBtnDecIterationsDown(MouseEvent& event)
{
	UpdateIterations(m_maxIterations-1);
	UpdateUi();
	return true;
}

bool Mandelbrot::OnBtnIncIterationsDown(MouseEvent& event)
{
	UpdateIterations(m_maxIterations+1);
	UpdateUi();
	return true;
}

void Mandelbrot::UpdateIterations(int iterations)
{
	m_maxIterations = iterations;
	if (m_maxIterations < 1) m_maxIterations = 1;
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
		m_destCenterPos.x -= (offset.x / g_pDevice->GetSize().y * m_zoom * 3.75f);
		m_destCenterPos.y += (offset.y / g_pDevice->GetSize().y * m_zoom * 3.75f);
		m_centerPos = m_destCenterPos;
		UpdateUi();
	}

	return true;
}

bool Mandelbrot::OnMouseWheel(MouseEvent& event)
{
	if (event.GetWheelDetail() < 0)
	{
		m_destZoom *= 2.0f;
		m_bZooming = true;
	}
	else
	{
		m_destZoom *= 0.5f;
		m_bZooming = true;
	}
	m_zoomSpeed = m_destZoom - m_zoom;

	Vector2 mouseOffset = event.GetPosition() - g_pDevice->GetSize()*0.5f;
	m_destCenterPos.x = m_centerPos.x + mouseOffset.x / g_pDevice->GetSize().y * m_zoom * 3.75f;
	m_destCenterPos.y = m_centerPos.y - mouseOffset.y / g_pDevice->GetSize().y * m_zoom * 3.75f;
	m_centerPosSpeed = m_destCenterPos - m_centerPos;

	UpdateUi();
	return true;
}

void Mandelbrot::UpdateUi()
{
	tstring strLabel;
	StringUtil::strformat(strLabel, _T("Max Iterations: %d"), m_maxIterations);
	m_pLblMaxIterations->SetLabel(strLabel);

	StringUtil::strformat(strLabel, _T("Use Mouse Wheel To Zoom In / Zoom Out, Current Zoom: %f"), m_destZoom);
	m_pLblZoom->SetLabel(strLabel);

	StringUtil::strformat(strLabel, _T("Use Mouse Left Button To Drag, Current Center Pos: (%f, %f)"), m_destCenterPos.x, m_destCenterPos.y);
	m_pLblCenterPos->SetLabel(strLabel);

	m_pVContainer->ReLayout();
}
