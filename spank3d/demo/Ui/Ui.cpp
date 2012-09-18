/*!
 * \file Ui.cpp
 * \date 9-11-2012 20:56:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Ui.h"
#include <util/AppUtil.h>
#include <event/EventIds.h>

IMPLEMENT_APP(Ui);

Ui::Ui()
{
	m_pTexture = NULL;
}

Ui::~Ui()
{
	// TODO: 
}

bool Ui::Initialize()
{
	m_pTexture = g_pResMgr->CreateTexture(_("sun.png"));
	if (!m_pTexture) return false;

	m_Size.Reset(float(m_pTexture->GetWidth()), float(m_pTexture->GetHeight()));
	m_Speed.Reset(Math::Random(150.0f, 200.0f), Math::Random(150.0f, 200.0f));

	return true;
}

void Ui::Terminate()
{
	SAFE_RELEASE(m_pTexture);
}

void Ui::Update(float dt)
{
	UpdatePosition(dt);
	g_pRendererUi->DrawRect(m_Position, m_Size, m_pTexture);
	g_pRendererUi->FlushAll();
}

void Ui::UpdatePosition(float dt)
{
	m_Position += m_Speed*dt;

	float windowWidth = float(g_pDevice->GetWindowWidth());
	float windowHeight = float(g_pDevice->GetWindowHeight());

	if (m_Position.x < 0.0f)
	{
		m_Position.x = 0.0f;
		m_Speed.x = -m_Speed.x;
	}

	if (m_Position.x + m_Size.x > windowWidth)
	{
		m_Position.x = windowWidth - m_Size.x;
		m_Speed.x = -m_Speed.x;
	}

	if (m_Position.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Speed.y = -m_Speed.y;
	}

	if (m_Position.y + m_Size.y > windowHeight)
	{
		m_Position.y = windowHeight - m_Size.y;
		m_Speed.y = -m_Speed.y;
	}
}
