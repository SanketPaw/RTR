// Header Files
#include<windows.h>
#include "OGL.h"
#include<stdio.h>	// For FILE_IO()
#include<stdlib.h>	//For Exit()
// Opengl Header files 
#include<GL/glew.h>
#include <Gl/gL.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//OpenGl Liabraries
#pragma comment(lib,"OpenGl32.lib")
#pragma comment(lib,"glew32.lib")

//Global functions declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable
HWND ghwnd = FALSE;
BOOL gbFullScreen = FALSE;
FILE* gpFile = NULL;
BOOL gbActiveWindow = FALSE;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

// Programable pipline global variables
GLuint shaderProgramObject;
GLuint vertexShaderObject = 0;
GLuint fragmentShaderObject = 0;
GLuint vao = 0;
GLuint vbo = 0;

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function Declarion
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	//variable declaration 
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetval = 0;

	// code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Creation Of Log File Failed. EXITTING"), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Susccessfully Created\n");
	}
	// initialization of class WNDCLASSEX struction
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registering WNDCLASSEX
	RegisterClassEx(&wndclass);

	// Create The Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
		TEXT("OpenGl Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// initialize();
	iRetval = initialize();
	if (iRetval == -1)
	{
		fprintf(gpFile, "CHOOSEPIXELFORMAT FAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -2)
	{
		fprintf(gpFile, "SETPIXELFORMATFAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -3)
	{
		fprintf(gpFile, "CREATEOPENGLCONTEXT FAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -4)
	{
		fprintf(gpFile, "MAKINOPENGLCONTEXT AS CURRENT CONTEXT FAILED !!!\n");
		uninitialize();
	}
	if (iRetval == -5)
	{
		fprintf(gpFile, "glewInit() AS CURRENT CONTEXT FAILED !!!\n");
		uninitialize();
	}
	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Foregrounding and Focusing The Window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				// Render The Scene
				display();

				// Update The Scene
				update();
			}
		}
	}
    uninitialize();
	return (int)msg.wParam;
}

// CallBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rc;

	//function declaration
	void ToggleFullScreen();
	void resize(int, int);

	switch (iMsg)
	{
	case WM_CREATE:
	{
		int sspWidth, sspHeight;
		RECT rect;

		// get screen size
		sspWidth = GetSystemMetrics(SM_CXSCREEN);
		sspHeight = GetSystemMetrics(SM_CYSCREEN);

		// get window
		GetWindowRect(hwnd, &rect);

		//Reset the value in rect
		rect.left = (sspWidth - rect.right) / 2;
		rect.top = (sspHeight - rect.bottom) / 2;

		// move the window to the specified position
		SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}
	break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		GetClientRect(hwnd, &rc);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			ToggleFullScreen();
			break;
		case 27:
			if (gpFile)
			{
				fprintf(gpFile, "Log File SuccessFully CLosed\n");
				fclose(gpFile);
				gpFile = NULL;
			}
			PostQuitMessage(0);
		default:
			break;
		}
		break;

	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		return 0;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void ToggleFullScreen()
{
	// variable Declaration
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	// code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}

}

int initialize(void)
{
	// Function Declarations
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);
	
	// Variable Declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	// Code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// Get DC
	ghdc = GetDC(ghwnd);

	// choose pixel Format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		return -1;
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd)==FALSE)
	{
		return -2;
	}

	// Ceate opengl rendering context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		return -3;
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return -4;
	}
	
	//GLEW initializations
	if (glewInit() != GLEW_OK)
	{
		return -5;
	}

	printGLInfo();

	// vertex shader
	const GLchar* vertexShaderSourceCode =
		"#version 400 core" \
		"\n" \
		"void main(void)" \
		"{" \
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);
	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char* log = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "vertexShader Compilation Log : %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	// Fragment  shader
	const GLchar* fragmentShaderSourceCode =
		"#version 400 core" \
		"\n" \
		"void main(void)" \
		"{" \
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "fragmentShader Compilation Log : %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);
	glLinkProgram(shaderProgramObject);

	 infoLogLength = 0;
	 status = 0;
	 log = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);

			if (log != NULL)
			{
				GLsizei written = 0;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				fprintf(gpFile, "CG: Shader program link log: %s\n", log);
				free(log);
				uninitialize();
			}
		}
	}

	// here stars opengl code
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Depth Related Functions
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	

	resize(WIN_WIDTH, WIN_HEIGHT);		// WarmUp Resize Call
	return 0;
}

void printGLInfo(void)
{
	void uninitialize(void);

	// Local variable declarations
	GLint NUMEXTENTIONS, numExtentions = 0;

	// fetch OpenGL related details
	fprintf(gpFile, "OpenGL Vendor:   %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version:  %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version:    %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// fetch OpenGL enabled extensions
	GLint numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	fprintf(gpFile, "OpenGL Extensions: \n");
	for (int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "  %s\n", glGetStringi(GL_EXTENSIONS, i));
	}

	
}

void resize(int width, int height)
{
	// Code
	if (height == 0)
		height = 1;
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// using shader program object in display
	glUseProgram(shaderProgramObject);

	// here there should be drawing code

	// unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code
}

void uninitialize(void)
{
	// Function Declaration
	void ToggleFullScreen(void);
	
	// Code
	if (gbFullScreen == TRUE)
	{
		ToggleFullScreen();
	}
	
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (shaderProgramObject)
	{
		if (vertexShaderObject)
		{
			glDetachShader(shaderProgramObject, vertexShaderObject);
		}

		if (fragmentShaderObject)
		{
			glDetachShader(shaderProgramObject, fragmentShaderObject);
		}
	}

	if (vertexShaderObject)
	{
		glDeleteShader(vertexShaderObject);
		vertexShaderObject = 0;
	}

	if (fragmentShaderObject)
	{
		glDeleteShader(fragmentShaderObject);
		fragmentShaderObject = 0;
	}

	if (shaderProgramObject)
	{
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

	glUseProgram(0);

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;	
	}

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	if (gpFile)
	{
		fprintf(gpFile, "Log File SuccessFully CLosed\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
