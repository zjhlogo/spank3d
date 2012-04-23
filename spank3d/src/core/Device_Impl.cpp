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

Device_Impl::Device_Impl()
{
	m_fPrevTime = 0.0f;
	m_fCurrTime = 0.0f;
	m_fDetailTime = 0.0f;
	m_fFPS = DEFAULT_FPS;
	m_WindowWidth = DEFAULT_WINDOW_WIDTH;
	m_WindowHeight = DEFAULT_WINDOW_HEIGHT;

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
	LOG(_("StartPerforming ..."));

	// Create Render signal event
	HANDLE hTickEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ResetEvent(hTickEvent);

	// Create time event
	UINT nDelay = (UINT)(1000.0f/m_fFPS);
	if (nDelay < 1) nDelay = 1;
	MMRESULT hEventTimer = timeSetEvent(nDelay, 1, (LPTIMECALLBACK)hTickEvent, 0, TIME_PERIODIC|TIME_CALLBACK_EVENT_SET);

	// reset time
	m_fPrevTime = (float)timeGetTime()/1000.0f;
	m_fCurrTime = m_fPrevTime;
	m_fDetailTime = 0.0f;

	// notify start perform
	IEvent event(EID_START_PERFORM, this);
	DispatchEvent(event);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if(WAIT_OBJECT_0 == MsgWaitForMultipleObjects(1, &hTickEvent, FALSE, 1000, QS_ALLINPUT))
		{
			// calculate time
			m_fCurrTime = (float)timeGetTime()/1000.0f;
			m_fDetailTime = m_fCurrTime - m_fPrevTime;
			PerformOnce(m_fDetailTime);
			m_fPrevTime = m_fCurrTime;
		}
		else
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{
				if(msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	if(hEventTimer)
	{
		timeKillEvent(hEventTimer);
		hEventTimer = NULL;
	}

	if(hTickEvent)
	{
		CloseHandle(hTickEvent);
		hTickEvent = NULL;
	}
}

void Device_Impl::EndPerform()
{
	PostQuitMessage(0);
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
			MouseEvent event(EID_LBUTTON_DOWN, g_pDevice);
			event.SetPosition(x, y);
			g_pDevice->DispatchEvent(event);
		}
		break;
	case WM_LBUTTONUP:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// dispatch event
			MouseEvent event(EID_LBUTTON_UP, g_pDevice);
			event.SetPosition(x, y);
			g_pDevice->DispatchEvent(event);

			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:
		{
			short x = LOWORD(lParam);
			short y = HIWORD(lParam);

			// dispatch event
			MouseEvent event(EID_MOUSE_MOVE, g_pDevice);
			event.SetPosition(x, y);
			event.SetPosition((int)x-s_nLastMousePosX, (int)y-s_nLastMousePosY);
			g_pDevice->DispatchEvent(event);

			s_nLastMousePosX = x;
			s_nLastMousePosY = y;
		}
		break;
	case WM_KEYDOWN:
		{
			// dispatch event
			KeyboardEvent event(EID_KEY_DOWN, g_pDevice);
			event.SetKeyCode(wParam);
			g_pDevice->DispatchEvent(event);
		}
		break;
	case WM_KEYUP:
		{
			// dispatch event
			KeyboardEvent event(EID_KEY_UP, g_pDevice);
			event.SetKeyCode(wParam);
			g_pDevice->DispatchEvent(event);
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
	RECT rc = {0, 0, m_WindowWidth, m_WindowHeight};
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
		LOG(_("Device_Impl::InternalCreateWindow Failed"));
		return false;
	}

 	LOG(_("Device_Impl::InternalCreateWindow OK"));
	return true;
}

void Device_Impl::InternalDestroyWindow()
{
	if (g_hWnd)
	{
		DestroyWindow(g_hWnd);
		g_hWnd = NULL;
 		LOG(_("Device_Impl::InternalDestroyWindow OK"));
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
		24,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(g_hDc = GetDC(g_hWnd)))					// Did We Get A Device Context?
	{
		LOG(_("Device_Impl::InternalCreateOGL, Can't Create A GL Device Context"));
		return false;								// Return FALSE
	}

	GLuint PixelFormat;								// Holds The Results After Searching For A Match
	if (!(PixelFormat = ChoosePixelFormat(g_hDc, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		LOG(_("Device_Impl::InternalCreateOGL, Can't Find A Suitable PixelFormat"));
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(g_hDc, PixelFormat, &pfd))	// Are We Able To Set The Pixel Format?
	{
		LOG(_("Device_Impl::InternalCreateOGL, Can't Set The PixelFormat"));
		return false;								// Return FALSE
	}

	if (!(g_hRc = wglCreateContext(g_hDc)))			// Are We Able To Get A Rendering Context?
	{
		LOG(_("Device_Impl::InternalCreateOGL, Can't Create A GL Rendering Context"));
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(g_hDc, g_hRc))				// Try To Activate The Rendering Context
	{
		LOG(_("Device_Impl::InternalCreateOGL, Can't Activate The GL Rendering Context"));
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
			LOG(_("Device_Impl::InternalDestroyOGL, Release Of DC And RC Failed"));
		}

		if (!wglDeleteContext(g_hRc))					// Are We Able To Delete The RC?
		{
			LOG(_("Device_Impl::InternalDestroyOGL, Release Rendering Context Failed"));
		}
		g_hRc = NULL;									// Set RC To NULL
	}

	if (g_hDc && !ReleaseDC(g_hWnd, g_hDc))				// Are We Able To Release The DC
	{
		LOG(_("Device_Impl::InternalDestroyOGL, Release Device Context Failed"));
		g_hDc = NULL;									// Set DC To NULL
	}
}

void Device_Impl::PerformOnce(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	g_pApp->Update(dt);
	SwapBuffers(g_hDc);					// Swap Buffers (Double Buffering)
}

void Device_Impl::InitializeOGL()
{
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glViewport(0, 0, m_WindowWidth, m_WindowHeight);	// Reset The Current Viewport
}
