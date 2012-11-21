#include "construct.h"
#include <windows.h>
#include <WindowsX.h>

#include "game.h"

HINSTANCE hInst;
HWND hWnd;
HDC hDC;
COLORREF g_KeyColor = RGB(0x00, 0x00, 0x00);

char appName[] = "OpenGL ES Sansbox";
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

unsigned int x = 200, y = 200, w = 1024, h = 768;
EGLDisplay glesDisplay;
EGLSurface glesSurface;
EGLContext glesContext;

int mouseX = 0;
int mouseY = 0;
bool button1 = false;
bool button2 = false;
unsigned int deltaTimeMs = 0;
Game game;

bool InitOGLES()
{

	EGLConfig configs[10];
	EGLConfig config;
	EGLint matchingConfigs;

#if USE_16BPP // 16BPP: RGB565
	const EGLint configAttribs[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     16,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
		EGL_NONE,           EGL_NONE
	};
#else // 24BPP: RGB888
	const EGLint configAttribs[] =
	{
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     16,
		EGL_STENCIL_SIZE,   EGL_DONT_CARE,
		EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
		EGL_NONE,           EGL_NONE
	};
#endif

	EGLint aEGLContextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	// Get EGL display
	hDC = GetWindowDC(hWnd);
	glesDisplay = eglGetDisplay(hDC);

	// Initialize EGL
	if (!eglInitialize(glesDisplay, NULL, NULL))
	{
		return false;
	}

	// Choose EGL config
	if (!eglChooseConfig(glesDisplay, configAttribs, &configs[0], 10,  &matchingConfigs))
	{
		return false;
	}

	if (matchingConfigs < 1)
	{
		return false;
	}

	// Choose EGL config
#if USE_16BPP
	for ( int i=0; i < matchingConfigs; i++ )
	{
		EGLint value;

		/* Use this to explicitly check that the EGL config has the expected color depths */
		eglGetConfigAttrib( glesDisplay, configs[i], EGL_RED_SIZE, &value );
		if ( 5 != value ){
			continue;
		}
		eglGetConfigAttrib( glesDisplay, configs[i], EGL_GREEN_SIZE, &value );
		if ( 6 != value ){
			continue;
		}
		eglGetConfigAttrib( glesDisplay, configs[i], EGL_BLUE_SIZE, &value );
		if ( 5 != value ){
			continue;
		}
		eglGetConfigAttrib( glesDisplay, configs[i], EGL_ALPHA_SIZE, &value );
		if ( 0 != value ){
			continue;
		}
		eglGetConfigAttrib( glesDisplay, configs[i], EGL_SAMPLES, &value );
		if ( 4 != value ){
			continue;
		}

		config = configs[i];
		break;
	}

#else
	config = configs[0];
#endif

	// Create EGL window surface
	glesSurface = eglCreateWindowSurface(glesDisplay, config, hWnd, NULL );

	if (!glesSurface)
	{
		return false;
	}

	// Create EGL rendering context
	glesContext = eglCreateContext(glesDisplay, config, 0, aEGLContextAttributes);

	// Attach the EGL rendering context to EGL surfaces
	eglMakeCurrent(glesDisplay, glesSurface, glesSurface, glesContext);

	return TRUE; 
}


void Render()
{
	unsigned int startTick = GetTickCount();
	glViewport (0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game.step(mouseX, mouseY, button1, button2, deltaTimeMs);

	eglSwapBuffers(glesDisplay, glesSurface);
	unsigned int endTick = GetTickCount();
	deltaTimeMs = (endTick - startTick);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASSEX wc;
	hInst = hInstance;

	bool done = false;
	HBRUSH hbrColorKey = CreateSolidBrush(g_KeyColor);

	if (hWnd = FindWindow(appName, appName))
	{
		return 0;
	}

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC) WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hIcon         = LoadIcon(NULL, NULL);
	wc.hCursor       = LoadCursor(NULL, NULL);
	wc.hbrBackground = hbrColorKey;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = appName;
	wc.hInstance     = hInstance;
	wc.hIconSm       = LoadIcon(NULL, NULL);

	if (!RegisterClassEx(&wc))
	{
		return false;
	}

	hWnd = CreateWindow(appName, appName, WS_POPUP | /*WS_CLIPCHILDREN | WS_CLIPSIBLINGS |*/ WS_VISIBLE | /*WS_OVERLAPPED |*/ WS_CAPTION | WS_SYSMENU | /*WS_THICKFRAME |*/ WS_MINIMIZEBOX | WS_MAXIMIZEBOX, x, y, w, h, NULL, NULL, hInst, NULL);
	if (!hWnd)
	{
		return false;
	}

	if (!InitOGLES())
	{
		MessageBox(hWnd, "OpenGL ES init error.", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Bring the window to front, focus it and refresh it
	SetWindowText(hWnd, appName);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	game.init(w, h);

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_MOUSEMOVE:
				mouseX = GET_X_LPARAM(msg.lParam);
				mouseY = GET_Y_LPARAM(msg.lParam);
				break;
			case WM_LBUTTONDOWN:
				button1 = true;
				break;
			case WM_LBUTTONUP:
				button1 = false;
				break;
			case WM_RBUTTONDOWN:
				button2 = true;
				break;
			case WM_RBUTTONUP:
				button2 = false;
				break;
			case WM_QUIT:
				done = true;
				break;

			case WM_KEYDOWN:
				break;

			default:
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}

		Render();
	}

	//_CrtDumpMemoryLeaks();
	DestroyWindow(hWnd);
	UnregisterClass(appName, hInst);

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC = GetWindowDC(hWnd);

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	};

	return DefWindowProc(hWnd, message, wParam, lParam);
}
