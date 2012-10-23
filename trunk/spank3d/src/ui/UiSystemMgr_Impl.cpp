/*!
 * \file UiSystemMgr_Impl.cpp
 * \date 10-10-2012 9:20:30
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "UiSystemMgr_Impl.h"
#include "UiRenderer_Impl.h"
#include "UiResMgr_Impl.h"
#include "UiInputMgr_Impl.h"
#include <util/LogUtil.h>
#include <ui/UiState.h>
#include <Spank3d.h>

#include <ui/parsers/PushButtonParser.h>

UiSystemMgr_Impl::UiSystemMgr_Impl()
{
	g_pUiSystemMgr = this;
	m_pCurrScreen = NULL;

	m_pDownWindow = NULL;
	m_pHoverWindow = NULL;
	m_pFocusWindow = NULL;
}

UiSystemMgr_Impl::~UiSystemMgr_Impl()
{
	g_pUiSystemMgr = NULL;
}

UiSystemMgr_Impl& UiSystemMgr_Impl::GetInstance()
{
	static UiSystemMgr_Impl s_UiSystemMgr_Impl;
	return s_UiSystemMgr_Impl;
}

bool UiSystemMgr_Impl::Initialize()
{
	if (!UiRenderer_Impl::GetInstance().Initialize()) return false;
	if (!UiResMgr_Impl::GetInstance().Initialize()) return false;
	if (!UiInputMgr_Impl::GetInstance().Initialize()) return false;
	if (!InitParsers()) return false;

	const Vector2& screenSize = g_pDevice->GetSize();
	m_ScreenRect.Reset(0.0f, 0.0f, screenSize.x, screenSize.y);

	SwitchScreen(CreateScreen()->GetScreenIndex());
	return true;
}

void UiSystemMgr_Impl::Terminate()
{
	DestroyScreens();
	FreeParsers();
	UiInputMgr_Impl::GetInstance().Terminate();
	UiResMgr_Impl::GetInstance().Terminate();
	UiRenderer_Impl::GetInstance().Terminate();
}

Screen* UiSystemMgr_Impl::CreateScreen()
{
	Screen* pScreen = new Screen(m_vScreen.size());
	pScreen->SetSize(Vector2(m_ScreenRect.width, m_ScreenRect.height));

	m_vScreen.push_back(pScreen);
	return pScreen;
}

Screen* UiSystemMgr_Impl::GetScreen(uint index)
{
	if (index >= m_vScreen.size()) return NULL;
	return m_vScreen[index];
}

bool UiSystemMgr_Impl::SwitchScreen(uint index)
{
	Screen* pScreen = GetScreen(index);
	if (!pScreen) return false;

	m_pCurrScreen = pScreen;
	return true;
}

Screen* UiSystemMgr_Impl::GetCurrScreen()
{
	return m_pCurrScreen;
}

const Rect& UiSystemMgr_Impl::GetScreenRect() const
{
	return m_ScreenRect;
}

IWindow* UiSystemMgr_Impl::LoadWindowFromFile(const tstring& strFile, IWindow* pParent)
{
	tstring strXmlData;
	if (!g_pResMgr->ReadStringFile(strXmlData, strFile)) return NULL;

	TiXmlDocument doc;
	doc.Parse(strXmlData.c_str());
	if (doc.Error()) return NULL;

	TiXmlElement* pXmlRoot = doc.RootElement();
	return ParseWindowFromXml(pXmlRoot, pParent);
}

void UiSystemMgr_Impl::Update(float dt)
{
	// TODO: 
}

void UiSystemMgr_Impl::Render()
{
	Vector2 basePos(0.0f, 0.0f);
	Rect clipRect(0.0f, 0.0f, m_pCurrScreen->GetSize().x, m_pCurrScreen->GetSize().y);

	m_pCurrScreen->SystemRender(basePos, clipRect, UiState::STATE_DEFAULT);
}

void UiSystemMgr_Impl::DestroyScreens()
{
	for (TV_SCREEN::iterator it = m_vScreen.begin(); it != m_vScreen.end(); ++it)
	{
		Screen* pScreen = (*it);
		SAFE_DELETE(pScreen);
	}
	m_vScreen.clear();
	m_pCurrScreen = NULL;
}

void UiSystemMgr_Impl::SetWindowDownState(IWindow* pWindow)
{
	if (m_pDownWindow == pWindow) return;

	if (m_pDownWindow)
	{
		m_pDownWindow->SetWindowState(IWindow::WS_MOUSE_DOWN, false);
		m_pDownWindow->UnregisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnDownWindowDestroyed);
// 		LOG(_T("Mouse down leave %s"), m_pDownWindow->GetRtti()->GetClassName().c_str());
	}

	m_pDownWindow = pWindow;

	if (m_pDownWindow)
	{
		m_pDownWindow->SetWindowState(IWindow::WS_MOUSE_DOWN, true);
		m_pDownWindow->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnDownWindowDestroyed);
// 		LOG(_T("Mouse down enter %s"), m_pDownWindow->GetRtti()->GetClassName().c_str());
	}
}

void UiSystemMgr_Impl::SetWindowHoverState(IWindow* pWindow)
{
	if (m_pHoverWindow == pWindow) return;

	if (m_pHoverWindow)
	{
		m_pHoverWindow->SetWindowState(IWindow::WS_MOUSE_HOVER, false);
		m_pHoverWindow->UnregisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnHoverWindowDestroyed);
// 		LOG(_T("Mouse hover leave %s"), m_pHoverWindow->GetRtti()->GetClassName().c_str());
	}

	m_pHoverWindow = pWindow;

	if (m_pHoverWindow)
	{
		m_pHoverWindow->SetWindowState(IWindow::WS_MOUSE_HOVER, true);
		m_pHoverWindow->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnHoverWindowDestroyed);
// 		LOG(_T("Mouse hover enter %s"), m_pHoverWindow->GetRtti()->GetClassName().c_str());
	}
}

void UiSystemMgr_Impl::SetWindowFocusState(IWindow* pWindow)
{
	if (m_pFocusWindow == pWindow) return;

	if (m_pFocusWindow)
	{
		m_pFocusWindow->SetWindowState(IWindow::WS_FOCUS, false);
		m_pFocusWindow->UnregisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnFocusWindowDestroyed);
// 		LOG(_T("Mouse focus leave %s"), m_pFocusWindow->GetRtti()->GetClassName().c_str());
	}

	m_pFocusWindow = pWindow;

	if (m_pFocusWindow && m_pFocusWindow->CheckWindowState(IWindow::WS_FOCUS_ENABLE))
	{
		m_pFocusWindow->SetWindowState(IWindow::WS_FOCUS, true);
		m_pFocusWindow->RegisterEvent(EID_OBJECT_DESTROYED, this, (FUNC_HANDLER)&UiSystemMgr_Impl::OnFocusWindowDestroyed);
// 		LOG(_T("Mouse focus enter %s"), m_pFocusWindow->GetRtti()->GetClassName().c_str());
	}
	else
	{
		m_pFocusWindow = NULL;
	}
}

IWindow* UiSystemMgr_Impl::GetDownWindow()
{
	return m_pDownWindow;
}

IWindow* UiSystemMgr_Impl::GetHoverWindow()
{
	return m_pHoverWindow;
}

IWindow* UiSystemMgr_Impl::GetFocusWindow()
{
	return m_pFocusWindow;
}

bool UiSystemMgr_Impl::OnDownWindowDestroyed(Event& event)
{
	m_pDownWindow = NULL;
	return true;
}

bool UiSystemMgr_Impl::OnHoverWindowDestroyed(Event& event)
{
	m_pHoverWindow = NULL;
	return true;
}

bool UiSystemMgr_Impl::OnFocusWindowDestroyed(Event& event)
{
	m_pFocusWindow = NULL;
	return true;
}

IWindow* UiSystemMgr_Impl::ParseWindowFromXml(TiXmlElement* pXmlWindow, IWindow* pParent)
{
	// find parser
	IWindowParser* pParser = FindParser(pXmlWindow->Value());
	if (!pParser) return NULL;

	// parse window
	IWindow* pWindow = pParser->Parse(pXmlWindow, pParent);
	if (!pWindow) return NULL;

	// parse child
	for (TiXmlElement* pXmlChild = pXmlWindow->FirstChildElement(); pXmlChild != NULL; pXmlChild = pXmlChild->NextSiblingElement())
	{
		ParseWindowFromXml(pXmlChild, pWindow);
	}

	return pWindow;
}

bool UiSystemMgr_Impl::InitParsers()
{
	AddParser(new PushButtonParser());

	return true;
}

void UiSystemMgr_Impl::FreeParsers()
{
	for (TM_PARSER::iterator it = m_ParsersMap.begin(); it != m_ParsersMap.end(); ++it)
	{
		IWindowParser* pParser = it->second;
		SAFE_DELETE(pParser);
	}
	m_ParsersMap.clear();
}

bool UiSystemMgr_Impl::AddParser(IWindowParser* pParser)
{
	if (FindParser(pParser->GetClassName()))
	{
		SAFE_DELETE(pParser);
		return false;
	}

	m_ParsersMap.insert(std::make_pair(pParser->GetClassName(), pParser));
	return true;
}

IWindowParser* UiSystemMgr_Impl::FindParser(const tstring& strClassName)
{
	TM_PARSER::iterator itFound = m_ParsersMap.find(strClassName);
	if (itFound == m_ParsersMap.end()) return NULL;
	return itFound->second;
}
