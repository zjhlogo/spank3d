/*!
 * \file Device_Impl.cpp
 * \date 4-15-2012 11:59:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Device_Impl.h"
#include <util/LogUtil.h>
#include <Spank3D.h>
#include <Windows.h>
#include <gl/glew.h>
#include <math/Math.h>

#include <event/MouseEvent.h>
#include <event/KeyboardEvent.h>

static HWND g_hWnd = NULL;
static HDC g_hDc = NULL;								// Private GDI Device Context
static HGLRC g_hRc = NULL;								// Permanent Rendering Context

static float GetTime()
{
	static LARGE_INTEGER s_tc, s_t1;
	static BOOL s_tcInit = QueryPerformanceFrequency(&s_tc);
	static BOOL s_t1Init = QueryPerformanceCounter(&s_t1);

	LARGE_INTEGER t2;
	QueryPerformanceCounter(&t2);

	return (float(t2.QuadPart-s_t1.QuadPart)/s_tc.QuadPart);
}

Device_Impl::Device_Impl()
{
	m_fLastTime = 0.0f;
	m_WindowSize.Reset(float(DEFAULT_WINDOW_WIDTH), float(DEFAULT_WINDOW_HEIGHT));

	g_pDevice = this;
}

Device_Impl::~Device_Impl()
{
	g_pDevice = NULL;
}

Device_Impl& Device_Impl::GetInstance()
{
	static Device_Impl s_Device_Impl;
	return s_Device_Impl;
}

bool Device_Impl::Initialize()
{
	// create window
	if (!InternalCreateWindow())
	{
		Terminate();
		return false;
	}

	// create opengl device
	if (!InternalCreateOGL())
	{
		Terminate();
		return false;
	}

	// initialize
	InitializeOGL();

	return true;
}

void Device_Impl::Terminate()
{
	InternalDestroyOGL();
	InternalDestroyWindow();
}

void Device_Impl::StartPerform()
{
	// show window
	ShowWindow(g_hWnd, SW_NORMAL);
	UpdateWindow(g_hWnd);

	// logout start perform
	LOG(_T("StartPerforming ..."));

	// reset time
	m_fLastTime = GetTime();

	// notify start perform
	Event event(EID_START_PERFORM);
	DispatchEvent(event);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float fCurrTime = GetTime();
			float dt = fCurrTime - m_fLastTime;
			PerformOnce(dt);
			m_fLastTime = fCurrTime;
		}
	}

// 	// Create Render signal event
// 	HANDLE hTickEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
// 	ResetEvent(hTickEvent);
// 
// 	// Create time event
// 	UINT nDelay = (UINT)(1000.0f/60.0f);
// 	if (nDelay < 1) nDelay = 1;
// 	MMRESULT hEventTimer = timeSetEvent(nDelay, 1, (LPTIMECALLBACK)hTickEvent, 0, TIME_PERIODIC|TIME_CALLBACK_EVENT_SET);
// 
// 	while (msg.message != WM_QUIT)
// 	{
// 		if(WAIT_OBJECT_0 == MsgWaitForMultipleObjects(1, &hTickEvent, FALSE, 1000, QS_ALLINPUT))
// 		{
// 			// calculate time
// 			float fCurrTime = GetTime();
// 			float dt = fCurrTime - m_fLastTime;
// 			PerformOnce(dt);
// 			m_fLastTime = fCurrTime;
// 		}
// 		else
// 		{
// 			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
// 			{
// 				if(msg.message == WM_QUIT) break;
// 
// 				TranslateMessage(&msg);
// 				DispatchMessage(&msg);
// 			}
// 		}
// 	}
// 
// 	if(hEventTimer)
// 	{
// 		timeKillEvent(hEventTimer);
// 		hEventTimer = NULL;
// 	}
// 
// 	if(hTickEvent)
// 	{
// 		CloseHandle(hTickEvent);
// 		hTickEvent = NULL;
// 	}
}

void Device_Impl::EndPerform()
{
	PostQuitMessage(0);
}

const Vector2& Device_Impl::GetSize() const
{
	return m_WindowSize;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static short s_nLastMousePosX = 0;
	static short s_nLastMousePosY = 0;

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);

			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// dispatch event
			MouseEvent mouseEvent(MouseEvent::MET_LBUTTON_DOWN);
			mouseEvent.SetPosition(x, y);
			g_pDevice->DispatchEvent(mouseEvent);

			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_LBUTTONUP:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// dispatch event
			MouseEvent mouseEvent(MouseEvent::MET_LBUTTON_UP);
			mouseEvent.SetPosition(x, y);
			g_pDevice->DispatchEvent(mouseEvent);

			ReleaseCapture();

			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_MOUSEMOVE:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// dispatch event
			MouseEvent mouseEvent(MouseEvent::MET_MOUSE_MOVE);
			mouseEvent.SetPosition(x, y);
			mouseEvent.SetOffset(float(x-s_nLastMousePosX), float(y-s_nLastMousePosY));
			g_pDevice->DispatchEvent(mouseEvent);

			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);
			short wheel = HIWORD(wParam);

			// dispatch event
			MouseEvent mouseEvent(MouseEvent::MET_MOUSE_WHEEL);
			mouseEvent.SetPosition(x, y);
			mouseEvent.SetWheelDetail(wheel);
			g_pDevice->DispatchEvent(mouseEvent);
		}
		break;
	case WM_KEYDOWN:
		{
			// dispatch event
			KeyboardEvent keyboardEvent(KeyboardEvent::KET_KEY_DOWN);
			keyboardEvent.SetKeyCode(wParam);
			g_pDevice->DispatchEvent(keyboardEvent);
		}
		break;
	case WM_KEYUP:
		{
			// dispatch event
			KeyboardEvent keyboardEvent(KeyboardEvent::KET_KEY_UP);
			keyboardEvent.SetKeyCode(wParam);
			g_pDevice->DispatchEvent(keyboardEvent);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	}

	return 0;
}

bool Device_Impl::InternalCreateWindow()
{
	if (g_hWnd) InternalDestroyWindow();

	HINSTANCE hInst = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEX wc = {sizeof(WNDCLASSEX),	// Specifies the size of this structure
		CS_OWNDC,							// Specifies the class style
		MainWndProc,						// Pointer to the window procedure
		0,									// Specifies the number of extra bytes to allocate following the window-class structure
		0,									// Specifies the number of extra bytes to allocate following the window instance
		hInst,								// Handle to the instance
		LoadIcon(NULL, IDI_APPLICATION),	// Handle to the class icon
		LoadCursor(NULL, IDC_ARROW),		// Handle to the class cursor
		(HBRUSH)COLOR_WINDOW,				// Handle to the class background brush
		NULL,								// resource name of the class menu
		"SPANK3D",							// Pointer to a null-terminated string or is an atom
		LoadIcon(NULL, IDI_APPLICATION)};	// Handle to a small icon that is associated with the window class
	RegisterClassEx(&wc);

	// screen width, height
	uint nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	uint nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	RECT rc = {0, 0, int(m_WindowSize.x), int(m_WindowSize.y)};
	AdjustWindowRect(&rc, dwStyle, FALSE);
	uint nAdjustWidth = rc.right - rc.left;
	uint nAdjustHeight = rc.bottom - rc.top;

	// create the window
	g_hWnd = CreateWindow("SPANK3D",
		"Spank 3D",
		dwStyle,
		(nScreenWidth-nAdjustWidth)/2,
		(nScreenHeight-nAdjustHeight)/2,
		nAdjustWidth,
		nAdjustHeight,
		NULL,
		NULL,
		hInst,
		NULL);
	if (!g_hWnd)
	{
		LOG(_T("Device_Impl::InternalCreateWindow Failed"));
		return false;
	}

 	LOG(_T("Device_Impl::InternalCreateWindow OK"));
	return true;
}

void Device_Impl::InternalDestroyWindow()
{
	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
 		LOG(_T("Device_Impl::InternalDestroyWindow OK"));
	}
}

bool Device_Impl::InternalCreateOGL()
{
	static PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24 Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(g_hDc = GetDC(g_hWnd)))					// Did We Get A Device Context?
	{
		LOG(_T("Device_Impl::InternalCreateOGL, Can't Create A GL Device Context"));
		return false;								// Return FALSE
	}

	GLuint PixelFormat;								// Holds The Results After Searching For A Match
	if (!(PixelFormat = ChoosePixelFormat(g_hDc, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		LOG(_T("Device_Impl::InternalCreateOGL, Can't Find A Suitable PixelFormat"));
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(g_hDc, PixelFormat, &pfd))	// Are We Able To Set The Pixel Format?
	{
		LOG(_T("Device_Impl::InternalCreateOGL, Can't Set The PixelFormat"));
		return false;								// Return FALSE
	}

	if (!(g_hRc = wglCreateContext(g_hDc)))			// Are We Able To Get A Rendering Context?
	{
		LOG(_T("Device_Impl::InternalCreateOGL, Can't Create A GL Rendering Context"));
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(g_hDc, g_hRc))				// Try To Activate The Rendering Context
	{
		LOG(_T("Device_Impl::InternalCreateOGL, Can't Activate The GL Rendering Context"));
		return false;								// Return FALSE
	}

	return true;
}

void Device_Impl::InternalDestroyOGL()
{
	if (g_hRc)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))				// Are We Able To Release The DC And RC Contexts?
		{
			LOG(_T("Device_Impl::InternalDestroyOGL, Release Of DC And RC Failed"));
		}

		if (!wglDeleteContext(g_hRc))					// Are We Able To Delete The RC?
		{
			LOG(_T("Device_Impl::InternalDestroyOGL, Release Rendering Context Failed"));
		}
		g_hRc = NULL;									// Set RC To NULL
	}

	if (g_hDc && !ReleaseDC(g_hWnd, g_hDc))				// Are We Able To Release The DC
	{
		LOG(_T("Device_Impl::InternalDestroyOGL, Release Device Context Failed"));
		g_hDc = NULL;									// Set DC To NULL
	}
}

void Device_Impl::PerformOnce(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	g_pApp->Update(dt);
	g_pApp->Render();

	SwapBuffers(g_hDc);					// Swap Buffers (Double Buffering)
}

void Device_Impl::InitializeOGL()
{
	glewInit();

	const tchar* pszRenderer = (const tchar*)glGetString(GL_RENDERER);
	const tchar* pszVendor = (const tchar*)glGetString(GL_VENDOR);
	const tchar* pszVersion = (const tchar*)glGetString(GL_VERSION);
	const tchar* pszGlslVersion = (const tchar*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	LOG(_T("GL Renderer : %s"), pszRenderer);
	LOG(_T("GL Vendor : %s"), pszVendor);
	LOG(_T("GL Version : %s"), pszVersion);
	LOG(_T("GLSL Version : %s"), pszGlslVersion);

	// Black Background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Depth Buffer Setup
	glClearDepth(1.0f);

	// enable Depth Testing
	glEnable(GL_DEPTH_TEST);
	// The Type Of Depth Testing To Do
	glDepthFunc(GL_LEQUAL);

	// enable cull mode
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// enable blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Really Nice Perspective Calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Reset The Current Viewport
	glViewport(0, 0, int(m_WindowSize.x), int(m_WindowSize.y));
}
