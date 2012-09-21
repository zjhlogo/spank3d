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
#include <ui/UiState.h>

IMPLEMENT_APP(Ui);

Ui::Ui()
{
	m_pTexSun = NULL;
	m_pNinePatchStyle = NULL;
}

Ui::~Ui()
{
	// TODO: 
}

bool Ui::Initialize()
{
	m_pTexSun = g_pResMgr->CreateTexture(_("sun.png"));
	if (!m_pTexSun) return false;

	m_pNinePatchStyle = g_pUiResMgr->FindNinePatchStyle(_("nps_default"));
	if (!m_pNinePatchStyle) return false;

	const Vector2& wndSize = g_pDevice->GetSize();
	const Vector2& sunSize = m_pTexSun->GetSize();

	m_sunSpeed.Reset(Math::Random(150.0f, 200.0f), Math::Random(150.0f, 200.0f));
	m_sunPosition.Reset(Math::Random(0.0f, wndSize.x-sunSize.x), Math::Random(0.0f, wndSize.y-sunSize.y));

	const Vector2& minPatchSize = m_pNinePatchStyle->GetMinSize();
	m_patchSpeed.Reset(Math::Random(150.0f, 200.0f), Math::Random(150.0f, 200.0f));
	m_patchPos.Reset(Math::Random(0.0f, wndSize.x-minPatchSize.x), Math::Random(0.0f, wndSize.y-minPatchSize.y));
	m_patchSize.Reset(minPatchSize.x, minPatchSize.y);

	return true;
}

void Ui::Terminate()
{
	SAFE_RELEASE(m_pTexSun);
}

void Ui::Update(float dt)
{
	UpdatePatchSize(dt);
	UpdatePosition(dt);

	m_pNinePatchStyle->Render(m_patchPos, m_patchSize, UiState::STATE_DEFAULT);
	g_pRendererUi->DrawRect(m_sunPosition, m_pTexSun->GetSize(), m_pTexSun);

	g_pRendererUi->FlushAll();
}

void Ui::UpdatePosition(float dt)
{
	m_sunPosition += m_sunSpeed*dt;

	const Vector2& wndSize = g_pDevice->GetSize();
	const Vector2& sunSize = m_pTexSun->GetSize();

	if (m_sunPosition.x < 0.0f)
	{
		m_sunPosition.x = 0.0f;
		m_sunSpeed.x = -m_sunSpeed.x;
	}

	if (m_sunPosition.x + sunSize.x > wndSize.x)
	{
		m_sunPosition.x = wndSize.x - sunSize.x;
		m_sunSpeed.x = -m_sunSpeed.x;
	}

	if (m_sunPosition.y < 0.0f)
	{
		m_sunPosition.y = 0.0f;
		m_sunSpeed.y = -m_sunSpeed.y;
	}

	if (m_sunPosition.y + sunSize.y > wndSize.y)
	{
		m_sunPosition.y = wndSize.y - sunSize.y;
		m_sunSpeed.y = -m_sunSpeed.y;
	}
}

void Ui::UpdatePatchSize(float dt)
{
	m_patchSize += m_patchSpeed*dt;

	const Vector2& wndSize = g_pDevice->GetSize();
	const Vector2& minPatchSize = m_pNinePatchStyle->GetMinSize();

	if (m_patchSize.x < minPatchSize.x)
	{
		m_patchSize.x = minPatchSize.x;
		m_patchSpeed.x = -m_patchSpeed.x;
	}

	if (m_patchSize.x + m_patchPos.x > wndSize.x)
	{
		m_patchSize.x = wndSize.x - m_patchPos.x;
		m_patchSpeed.x = -m_patchSpeed.x;
	}

	if (m_patchSize.y < minPatchSize.y)
	{
		m_patchSize.y = minPatchSize.y;
		m_patchSpeed.y = -m_patchSpeed.y;
	}

	if (m_patchSize.y + m_patchPos.y > wndSize.y)
	{
		m_patchSize.y = wndSize.y - m_patchPos.y;
		m_patchSpeed.y = -m_patchSpeed.y;
	}
}