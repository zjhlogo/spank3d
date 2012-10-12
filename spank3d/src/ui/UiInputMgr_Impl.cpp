/*!
 * \file UiInputMgr_Impl.cpp
 * \date 10-10-2012 9:32:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiInputMgr_Impl.h"
#include <util/SystemUtil.h>
#include <Spank3d.h>

UiInputMgr_Impl::UiInputMgr_Impl()
{
	SystemUtil::ZeroMemory(m_KeyMap, sizeof(m_KeyMap));
	g_pUiInputMgr = this;
}

UiInputMgr_Impl::~UiInputMgr_Impl()
{
	g_pUiInputMgr = NULL;
}

UiInputMgr_Impl& UiInputMgr_Impl::GetInstance()
{
	static UiInputMgr_Impl s_UiInputMgr_Impl;
	return s_UiInputMgr_Impl;
}

bool UiInputMgr_Impl::Initialize()
{
	g_pDevice->RegisterEvent(EID_MOUSE_EVENT, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(EID_KEYBOARD_EVENT, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
	return true;
}

void UiInputMgr_Impl::Terminate()
{
	g_pDevice->UnregisterEvent(EID_MOUSE_EVENT, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(EID_KEYBOARD_EVENT, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
}

bool UiInputMgr_Impl::IsKeyDown(uchar keyCode) const
{
	return m_KeyMap[keyCode];
}

bool UiInputMgr_Impl::IsLButtonDown() const
{
	return m_KeyMap[VK_LBUTTON];
}

bool UiInputMgr_Impl::IsMButtonDown() const
{
	return m_KeyMap[VK_MBUTTON];
}

bool UiInputMgr_Impl::IsRButtonDown() const
{
	return m_KeyMap[VK_RBUTTON];
}

const Vector2& UiInputMgr_Impl::GetMousePos() const
{
	return m_MousePos;
}

bool UiInputMgr_Impl::OnMouseEvent(MouseEvent& event)
{
	switch (event.GetMouseEventType())
	{
	case MouseEvent::MET_LBUTTON_DOWN:
		m_KeyMap[VK_LBUTTON] = true;
		break;
	case MouseEvent::MET_LBUTTON_UP:
		m_KeyMap[VK_LBUTTON] = false;
		break;
	case MouseEvent::MET_MBUTTON_DOWN:
		m_KeyMap[VK_MBUTTON] = true;
		break;
	case MouseEvent::MET_MBUTTON_UP:
		m_KeyMap[VK_MBUTTON] = false;
		break;
	case MouseEvent::MET_RBUTTON_DOWN:
		m_KeyMap[VK_RBUTTON] = true;
		break;
	case MouseEvent::MET_RBUTTON_UP:
		m_KeyMap[VK_RBUTTON] = false;
		break;
	}

	m_MousePos = event.GetPosition();
	Screen* pCurrScreen = g_pUiSystemMgr->GetCurrScreen();
	pCurrScreen->SystemMouseEvent(event);
	return true;
}

bool UiInputMgr_Impl::OnKeyboardEvent(KeyboardEvent& event)
{
	switch (event.GetKeyboardEventType())
	{
	case KeyboardEvent::KET_KEY_DOWN:
		m_KeyMap[event.GetKeyCode()] = true;
		break;
	case KeyboardEvent::KET_KEY_UP:
		m_KeyMap[event.GetKeyCode()] = false;
		break;
	}

	Screen* pCurrScreen = g_pUiSystemMgr->GetCurrScreen();
	pCurrScreen->SystemKeyboardEvent(event);
	return true;
}
