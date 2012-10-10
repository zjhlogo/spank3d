/*!
 * \file Screen.h
 * \date 10-10-2012 10:31:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "IWindow.h"

class Screen : public IWindow
{
public:
	RTTI_DEF(Screen, IWindow);

	Screen(uint screenIndex);
	virtual ~Screen();

	uint GetScreenIndex() const;
	virtual void Render(uint state);

private:
	uint m_nScreenIndex;

};
#endif // __SCREEN_H__
