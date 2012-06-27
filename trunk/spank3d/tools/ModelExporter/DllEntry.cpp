/*!
 * \file DllEntry.cpp
 * \date 1-8-2009 10:25:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "DllEntry.h"
#include "ModelExporterDesc.h"
#include "resource.h"

static bool Initialize(HINSTANCE hInstDLL)
{
	InitCommonControls();

	return true;
}

static void Uninitialize()
{
	// TODO: 
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	static int s_nInitCount = 0;
	static bool s_bInitialized = false;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		{
			++s_nInitCount;
			if (!s_bInitialized && s_nInitCount > 0)
			{
				s_bInitialized = true;
				if (!Initialize(hInstDLL)) return FALSE;
			}
		}
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		{
			--s_nInitCount;
			if (s_bInitialized && s_nInitCount <= 0)
			{
				s_bInitialized = false;
				Uninitialize();
			}
		}
		break;
	}

	return TRUE;
}

__declspec(dllexport) const TCHAR* LibDescription()
{
	return _T("Model Exporter Desc");
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		{
			static ModelExporterDesc s_MeshExporterDesc;
			return &s_MeshExporterDesc;
		}
		break;
	}

	return NULL;
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}
