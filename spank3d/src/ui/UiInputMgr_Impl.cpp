/*!
 * \file UiInputMgr_Impl.cpp
 * \date 10-10-2012 9:32:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiInputMgr_Impl.h"
#include <event/PreMouseEvent.h>
#include <util/SystemUtil.h>
#include <util/LogUtil.h>
#include <Spank3d.h>

UiInputMgr_Impl::UiInputMgr_Impl()
{
	SystemUtil::ZeroMemory(m_KeyMap, sizeof(m_KeyMap));
	m_pCaptureWindow = NULL;
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
	g_pDevice->RegisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::LBUTTON_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::MBUTTON_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::MBUTTON_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::RBUTTON_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::RBUTTON_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::MOUSE_MOVE, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->RegisterEvent(MouseEvent::MOUSE_WHEEL, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);

	g_pDevice->RegisterEvent(KeyboardEvent::KEY_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
	g_pDevice->RegisterEvent(KeyboardEvent::KEY_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
	g_pDevice->RegisterEvent(KeyboardEvent::KEY_CHAR, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);

	return true;
}

void UiInputMgr_Impl::Terminate()
{
	g_pDevice->UnregisterEvent(MouseEvent::LBUTTON_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::LBUTTON_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::MBUTTON_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::MBUTTON_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::RBUTTON_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::RBUTTON_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::MOUSE_MOVE, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);
	g_pDevice->UnregisterEvent(MouseEvent::MOUSE_WHEEL, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnMouseEvent);

	g_pDevice->UnregisterEvent(KeyboardEvent::KEY_DOWN, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
	g_pDevice->UnregisterEvent(KeyboardEvent::KEY_UP, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
	g_pDevice->UnregisterEvent(KeyboardEvent::KEY_CHAR, this, (FUNC_HANDLER)&UiInputMgr_Impl::OnKeyboardEvent);
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

bool UiInputMgr_Impl::CaptureMouse(IWindow* pWindow)
{
	if (m_pCaptureWindow)
	{
		LOG(_T("already capture the window %s(%s), release the capture first"), m_pCaptureWindow->GetRtti()->GetClassName().c_str(), m_pCaptureWindow->GetTag().c_str());
		return false;
	}

	m_pCaptureWindow = pWindow;
	return true;
}

bool UiInputMgr_Impl::ReleaseMouse()
{
	if (!m_pCaptureWindow) return false;
	m_pCaptureWindow = NULL;
	return true;
}

bool UiInputMgr_Impl::OnMouseEvent(MouseEvent& event)
{
	if (event.GetId() == MouseEvent::LBUTTON_DOWN)
	{
		m_KeyMap[VK_LBUTTON] = true;
	}
	else if (event.GetId() == MouseEvent::LBUTTON_UP)
	{
		m_KeyMap[VK_LBUTTON] = false;
	}
	else if (event.GetId() == MouseEvent::MBUTTON_DOWN)
	{
		m_KeyMap[VK_MBUTTON] = true;
	}
	else if (event.GetId() == MouseEvent::MBUTTON_UP)
	{
		m_KeyMap[VK_MBUTTON] = false;
	}
	else if (event.GetId() == MouseEvent::RBUTTON_DOWN)
	{
		m_KeyMap[VK_RBUTTON] = true;
	}
	else if (event.GetId() == MouseEvent::RBUTTON_UP)
	{
		m_KeyMap[VK_RBUTTON] = false;
	}

	m_MousePos = event.GetPosition();

	// find window to process
	IWindow* pWindowToProcess = m_pCaptureWindow;
	if (!pWindowToProcess) pWindowToProcess = g_pUiSystemMgr->GetCurrScreen()->FindWindowUnderPoint(m_MousePos);
	if (!pWindowToProcess) return false;

	// get parent list
	TV_WINDOW vParent;
	IWindow* pParent = pWindowToProcess;
	while (pParent)
	{
		vParent.push_back(pParent);
		pParent = pParent->GetParent();
	}

	// go throw each parent to pre-process the mouse event
	Vector2 localPos = m_MousePos;
	for (TV_WINDOW::reverse_iterator it = vParent.rbegin(); it != vParent.rend(); ++it)
	{
		IWindow* pWindowPreProcess = (*it);

		localPos -= pWindowPreProcess->GetPosition();
		localPos += pWindowPreProcess->GetScroll();

		// if (!pre process window) return false;
		PreMouseEvent preMouseEvent(PreMouseEvent::Convert2PreMouseEventType(event.GetId()));
		preMouseEvent.SetPosition(localPos);
		preMouseEvent.SetOffset(event.GetOffset());
		preMouseEvent.SetWheelDetail(event.GetWheelDetail());
		pWindowPreProcess->DispatchEvent(preMouseEvent);
		if (preMouseEvent.IsStopProcessing()) return false;
	}

	// process the window
	if (event.GetId() == MouseEvent::LBUTTON_DOWN)
	{
		// set down state
		g_pUiSystemMgr->SetWindowDownState(pWindowToProcess);
		// set focus state
		g_pUiSystemMgr->SetWindowFocusState(pWindowToProcess);
	}
	else if (event.GetId() == MouseEvent::LBUTTON_UP)
	{
		// clear down state
		if (g_pUiSystemMgr->GetDownWindow() == pWindowToProcess) g_pUiSystemMgr->SetWindowDownState(NULL);
	}
	else if (event.GetId() == MouseEvent::MOUSE_MOVE)
	{
		// set hover state
		g_pUiSystemMgr->SetWindowHoverState(pWindowToProcess);
		// set down state
		if (g_pUiInputMgr->IsLButtonDown()) g_pUiSystemMgr->SetWindowDownState(pWindowToProcess);
	}

	MouseEvent mouseEvent(event.GetId());
	mouseEvent.SetPosition(event.GetPosition());
	mouseEvent.SetOffset(event.GetOffset());
	mouseEvent.SetWheelDetail(event.GetWheelDetail());
	return pWindowToProcess->DispatchEvent(mouseEvent);
}

bool UiInputMgr_Impl::OnKeyboardEvent(KeyboardEvent& event)
{
// 	if (event.GetId() == KeyboardEvent::KEY_DOWN)
// 	{
// 		m_KeyMap[event.GetKeyCode()] = true;
// 	}
// 	else if (event.GetId() == KeyboardEvent::KEY_UP)
// 	{
// 		m_KeyMap[event.GetKeyCode()] = false;
// 	}
// 
// 	Screen* pCurrScreen = g_pUiSystemMgr->GetCurrScreen();
// 	pCurrScreen->SystemKeyboardEvent(event);
	return true;
}
