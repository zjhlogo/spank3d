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
	m_pTexture = g_pResMgr->CreateTexture(_("common0.png"));
	if (!m_pTexture) return false;

	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, FUNC_HANDLER(&Ui::OnMouseEvent));

	return true;
}

void Ui::Terminate()
{
	SAFE_RELEASE(m_pTexture);
}

void Ui::Update(float dt)
{
	g_pRendererUi->DrawRect(10.0f, 10.0f, 200.0f, 200.0f, m_pTexture);
	g_pRendererUi->FlushAll();
}

bool Ui::OnMouseEvent(MouseEvent& mouseEvent)
{
	return true;
}