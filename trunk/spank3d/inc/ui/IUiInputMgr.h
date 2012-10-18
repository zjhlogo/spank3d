/*!
 * \file IUiInputMgr.h
 * \date 10-10-2012 9:29:12
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IUIINPUTMGR_H__
#define __IUIINPUTMGR_H__

#include "../core/ISingleton.h"
#include "../math/Vector2.h"
#include "controls/IWindow.h"

class IUiInputMgr : public ISingleton
{
public:
	RTTI_DEF(IUiInputMgr, ISingleton);

	IUiInputMgr() {};
	virtual ~IUiInputMgr() {};

	virtual bool IsKeyDown(uchar keyCode) const = 0;
	virtual bool IsLButtonDown() const = 0;
	virtual bool IsMButtonDown() const = 0;
	virtual bool IsRButtonDown() const = 0;

	virtual const Vector2& GetMousePos() const = 0;

	virtual bool CaptureMouse(IWindow* pWindow) = 0;
	virtual bool ReleaseMouse() = 0;

};
#endif // __IUIINPUTMGR_H__
