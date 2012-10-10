/*!
 * \file UiInputMgr_Impl.h
 * \date 10-10-2012 9:30:37
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UIINPUTMGR_IMPL_H__
#define __UIINPUTMGR_IMPL_H__

#include <ui/IUiInputMgr.h>
#include <event/MouseEvent.h>
#include <event/KeyboardEvent.h>

class UiInputMgr_Impl : public IUiInputMgr
{
public:
	RTTI_DEF(UiInputMgr_Impl, IUiInputMgr);

	UiInputMgr_Impl();
	virtual ~UiInputMgr_Impl();

	static UiInputMgr_Impl& GetInstance();

	virtual bool Initialize();
	virtual void Terminate();

private:
	bool OnMouseEvent(MouseEvent& event);
	bool OnKeyboardEvent(KeyboardEvent& event);

};
#endif // __UIINPUTMGR_IMPL_H__
