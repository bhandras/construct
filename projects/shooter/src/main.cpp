#include "construct.h"
#ifdef WIN32
#include <windows.h>
#include <WindowsX.h>

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


float r = 0.0f;
Triangle t;
BitmapFont font;
Quad q;
TextureAtlas* atlas;
Sprite sprite;
Circle c;

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

	GL_Render::get().init();
	font.load("vector_skin_font.bff");
	atlas = new TextureAtlas("peasant.xml");
	atlas->load();

	sprite.setAtlas(atlas);
	sprite.addKeyFrameImage("peasant_walk0");
	sprite.addKeyFrameImage("peasant_walk1");
	sprite.addKeyFrameImage("peasant_walk2");
	sprite.addKeyFrameImage("peasant_walk3");
	sprite.addKeyFrameImage("peasant_walk4");
	sprite.addKeyFrameImage("peasant_walk5");
	sprite.addKeyFrameImage("peasant_walk6");
	sprite.addKeyFrameImage("peasant_walk7");
	sprite.setFPS(10);
	sprite.setLooped(true);

	Context::push();
	Context::setFillColor(Color4(0, 100, 0, 255));
	Context::setOutlineColor(Color4(0, 255, 0, 255));
	Context::setFilled(true);
	//Context::setOutlined(true);

	return TRUE; 
}

#include <sstream>

unsigned int deltaMS = 0;
float totalDrawTimeSec = 0.0f;
unsigned frames = 0;
int mouseX = 0;
int mouseY = 0;

void Render()
{
	unsigned int startTick = GetTickCount();
	glViewport (0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	GL_Render& gl = GL_Render::get();
	int drawCalls = gl.getNumDrawCalls();

	gl.beginFrame();
	gl.setOrthoProjection(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, 1.0f, -1.0f);

	Affine2df tr;
	tr.create(MathUtil::Numeric::deg2Rad(r), mouseX, mouseY, 1.0f, 1.0f);
	t.setTransformation(tr);
	t.setSize(100);
	t.update();
	

	tr.create(MathUtil::Numeric::deg2Rad(r), 150, 150, 1.0f, 1.0f);
	c.setTransformation(tr);
	c.setRadius(100);
	c.update();
	
	if (t.intersects(c))
	{
		Context::push();
		Context::setFillColor(Color4(100, 0, 0, 128));
		Context::setFilled(true);
		c.draw();
		Context::pop();
	}
	else
	{
		c.draw();
	}

	t.draw();

	q.setSize(100, 100);
	tr.create(MathUtil::Numeric::deg2Rad(r), 300, 500, 1.0f, 1.0f);

	q.setTransformation(tr);
	q.update();

	if (t.intersects(q))
	{
		Context::push();
		Context::setFillColor(Color4(100, 0, 0, 128));
		Context::setFilled(true);
		q.draw();
		Context::pop();
	}
	else
	{
		q.draw();
	}
	
	//for (int i = 0; i < 100; ++i)
	//{
	//	sprite.setPosition(rand() % 1024, rand() % 1024);
	//	sprite.update(deltaMS);
	//	sprite.draw();
	//}

	sprite.setPosition(150, 400);
	sprite.update(deltaMS);
	sprite.draw();

	std::stringstream strStream;
	
	strStream << "Draw calls: " << drawCalls;
	font.drawString(strStream.str(), 0, 50, BitmapFont::ALIGN_TL);
	
	Vector2f A(mouseX, mouseY);
	Vector2f B(300, 500);
	B = A + ((B - A) * 1.5);

	Vector2f N(B - A);
	N.normalize();
	N.rotate(MathUtil::Numeric::deg2Rad(90));
	Vector2f P0 = B - (N * 150);
	
	for (int i = 0; i < 300; ++i)
	{
		Vector2f end(P0 + (N * i));
		float L = (end - A).length();
		Vertex_Vector_XYZ_RGBA line;
		line.resize(2);
		line[0].setPosition(Vector2f(mouseX, mouseY));
		line[0].setColor(1.0f, 1.0f, 1.0f, 1.0f);

		Vector2f intersection;
		q.intersect(Vector2f(mouseX, mouseY), end, intersection);

		line[1].setPosition(intersection);
		float x = (L - (intersection - A).length());
		if (x < 0.01f) x = 0.0f;
		float col = x / L;
		line[1].setColor(col, col, col, 1.0f);

		Index_Vector ind;
		ind.resize(2);
		ind[0] = 0;
		ind[1] = 1;
		gl.setDrawMode(GL_LINES);
		gl.draw_XYZ_RGBA(line, ind);
	}

	gl.endFrame();
	r += 0.5f;

	eglSwapBuffers(glesDisplay, glesSurface);
	unsigned int endTick = GetTickCount();
	deltaMS = (endTick - startTick);
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

	// initConstruct(w, h);

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			 if (msg.message == WM_MOUSEMOVE)
			 {
				 mouseX = GET_X_LPARAM(msg.lParam);
				 mouseY = GET_Y_LPARAM(msg.lParam);
			}
			else if (msg.message==WM_QUIT)
			{
				done = true;
			}
			else if (msg.message == WM_KEYDOWN )
			{
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
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
#else
int main()
{
	return 0;
}
#endif // WIN32

