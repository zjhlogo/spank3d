/*!
 * \file UiSystemMgr_Impl.h
 * \date 10-10-2012 9:20:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UISYSTEMMGR_IMPL_H__
#define __UISYSTEMMGR_IMPL_H__

#include <ui/IUiSystemMgr.h>

class UiSystemMgr_Impl : public IUiSystemMgr
{
public:
	typedef std::vector<Screen*> TV_SCREEN;

public:
	RTTI_DEF(UiSystemMgr_Impl, IUiSystemMgr);

	UiSystemMgr_Impl();
	virtual ~UiSystemMgr_Impl();

	static UiSystemMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

	virtual Screen* CreateScreen();
	virtual Screen* GetScreen(uint index);
	virtual bool SwitchScreen(uint index);
	virtual Screen* GetCurrScreen();

	virtual void SetWindowDownState(IWindow* pWindow);
	virtual void SetWindowHoverState(IWindow* pWindow);
	virtual void SetWindowFocusState(IWindow* pWindow);

	virtual IWindow* GetDownWindow();
	virtual IWindow* GetHoverWindow();
	virtual IWindow* GetFocusWindow();

	virtual void Update(float dt);
	virtual void Render();

private:
	void DestroyScreens();
	bool OnDownWindowDestroyed(Event& event);
	bool OnHoverWindowDestroyed(Event& event);
	bool OnFocusWindowDestroyed(Event& event);

private:
	TV_SCREEN m_vScreen;
	Screen* m_pCurrScreen;

	IWindow* m_pDownWindow;
	IWindow* m_pHoverWindow;
	IWindow* m_pFocusWindow;

};
#endif // __UISYSTEMMGR_IMPL_H__
