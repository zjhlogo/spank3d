/*!
 * \file AppUtil.h
 * \date 4-14-2012 21:43:49
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __APPUTIL_H__
#define __APPUTIL_H__

#include <windows.h>

#define IMPLEMENT_APP(appclass)																	\
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)		\
{																								\
	appclass* pApp = new appclass();															\
	pApp->Run();																				\
	delete pApp;																				\
	return 0;																					\
}

#endif // __APPUTIL_H__
