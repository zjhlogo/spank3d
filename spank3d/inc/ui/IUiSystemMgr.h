/*!
 * \file IUiSystemMgr.h
 * \date 10-10-2012 9:13:43
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IUISYSTEMMGR_H__
#define __IUISYSTEMMGR_H__

#include "../core/ISingleton.h"
#include "controls/Screen.h"

class IUiSystemMgr : public ISingleton
{
public:
	RTTI_DEF(IUiSystemMgr, ISingleton);

	IUiSystemMgr() {};
	virtual ~IUiSystemMgr() {};

	virtual Screen* CreateScreen() = 0;
	virtual Screen* GetScreen(uint index) = 0;
	virtual bool SwitchScreen(uint index) = 0;
	virtual Screen* GetCurrScreen() = 0;
	virtual const Rect& GetScreenRect() const = 0;

	virtual IWindow* LoadWindowFromFile(const tstring& strFile, IWindow* pParent) = 0;

	virtual void SetWindowDownState(IWindow* pWindow) = 0;
	virtual void SetWindowHoverState(IWindow* pWindow) = 0;
	virtual void SetWindowFocusState(IWindow* pWindow) = 0;

	virtual IWindow* GetDownWindow() = 0;
	virtual IWindow* GetHoverWindow() = 0;
	virtual IWindow* GetFocusWindow() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

};
#endif // __IUISYSTEMMGR_H__
