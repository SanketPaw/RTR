// Header files
#include<windows.h>
#include<stdio.h> // For file I/O functions
#include<stdlib.h> // For exit()
#include"OGL.h"
#include"vmath.h"

// OpenGL header files
#include<C:/glew/include/GL/glew.h>
#include<GL/gl.h>
//cpp supporting
using namespace vmath;


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// OpenGL libraries
#pragma comment(lib,"glew32.lib")//2nd step
#pragma comment(lib, "OpenGL32.lib")

// Global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global variable declarations
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
LPARAM gLOlParam, gHIlParam;
FILE* gpFile = NULL;
BOOL gbActiveWindow = FALSE;

//programmable pipeline related global variables
GLuint shaderProgramObject;
enum
{
	AMC_ATRIBUTE_POSITION = 0,
	AMC_ATRIBUTE_COLOR,
	AMC_ATRIBUTE_NORMAL,
	AMC_ATRIBUTE_TEXTURE0

};

GLuint VAO_Pyramid;
GLuint VBO_Pyramid_Position;
GLuint VBO_Pyramid_Color;

GLuint VAO_Cube;
GLuint VBO_Cube_Position;
GLuint VBO_Cube_Color;
GLfloat anglePyramid=0.0f;
GLfloat angleCube=0.0f;


GLuint mvpMatrixUniform;

mat4 persepectiveProjectionMAtrix;

// Entry-point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Local function declarations
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// Variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;

	// Code
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(
			NULL,
			TEXT("Creation of log file failed. Exitting..."),
			TEXT("File I/O Error"),
			MB_OK
		);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log file successfully created.\n");
	}

	// Ininitialization of WNDCLASSEX structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registering the WNDCLASSEX
	RegisterClassEx(&wndclass);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2;

	// Create the window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("OGL Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		xPos,
		yPos,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ghwnd = hwnd;

	// Initialize
	iRetVal = initialize();
	if (iRetVal == -1)
	{
		fprintf(gpFile, "Choose pixel format failed.\n");
		uninitialize();
	}
	else if (iRetVal == -2)
	{
		fprintf(gpFile, "Set pixel format failed.\n");
		uninitialize();
	}
	else if (iRetVal == -3)
	{
		fprintf(gpFile, "Create OpenGL context failed.\n");
		uninitialize();
	}
	else if (iRetVal == -4)
	{
		fprintf(gpFile, "Making OpenGL context as current context failed.\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		fprintf(gpFile, "Initialization of glew failed.\n");
		uninitialize();
	}
	else
	{
		fprintf(gpFile, "Initialization of PIXELFORMATDESCRIPTOR structure successful.\n");
	}

	// Show window
	ShowWindow(hwnd, iCmdShow);

	// Foregroundinng and focusing the window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
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
				// Render the scene
				display();

				// Update the scene
				update();
			}
		}
	}

	uninitialize();

	return((int)msg.wParam);
}

// Callback function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Local function declarations
	void ToggleFullscreen(void);
	void resize(int, int);

	// Code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		fprintf(gpFile, "Window is in focus.\n");
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		fprintf(gpFile, "Window is not in focus.\n");
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 27:
			DestroyWindow(hwnd);
			break;

		default:
			break;

		}
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'F':
		case 'f':
			ToggleFullscreen();
			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	// Variable declarations
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	// Code
	wp.length = sizeof(WINDOWPLACEMENT);

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if (
				GetWindowPlacement(ghwnd, &wp)
				&& GetMonitorInfo(
					MonitorFromWindow(
						ghwnd,
						MONITORINFOF_PRIMARY
					),
					&mi
				)
				)
			{
				SetWindowLong(
					ghwnd,
					GWL_STYLE,
					dwStyle & ~WS_OVERLAPPEDWINDOW
				);
				SetWindowPos(
					ghwnd,
					HWND_TOP,

					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED
				);
			}

			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{
		SetWindowLong(
			ghwnd,
			GWL_STYLE,
			dwStyle | WS_OVERLAPPEDWINDOW
		);
		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(
			ghwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED
		);
		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}

int initialize(void)
{
	// Local function declarations
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);

	// Variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	// Code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	// memset((void*)&pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR))

	// Initialization of PIXELFORMATDESCRIPTOR structure
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


	// GetDC
	ghdc = GetDC(ghwnd);

	// Choose pixel format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	 
	if (iPixelFormatIndex == 0)
	{
		return(-1);
	}

	// Set chosen pixel format
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2);
	}

	// Create OpenGL rendering context
	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		return(-3);
	}

	// Make the rendering context as the current context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-4);
	}

	//glew initialiation
	if (glewInit()!=GLEW_OK)
	{
		return(-5);
	}
	printGLInfo();

	//vertex shader
	const GLchar* vertexShaderSourcecode =
		"#version 460 core" \
		"\n" \
		"in vec4 a_position;" \
		"in vec4 a_color;" \
		"uniform mat4 u_mvpMatrix;" \
		"out vec4 a_color_out;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_mvpMatrix * a_position;" \
		"a_color_out = a_color;" \
		"}";

	//create the shader
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	//give shader code to shader object
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourcecode, NULL);

	//compile the code(programatically compilation by OpenGL engine (machine code for GPU) (inline compiler)
	glCompileShader(vertexShaderObject);

	//Error checking of shader compilation
	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	//a.getting compilation status
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		//b.getting length of log (message) compilation
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			//c.allocate enough memory
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				//d.get the compilation log into this buffer
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				//e.display the contents of buffer
				fprintf(gpFile, "vertex shader compilation log: %s\n",log);
				//f.free log
				free(log);
				//g.exit the application
				uninitialize();
			}
		}
	}

	//fragment shader
	const GLchar* fragmentShaderSourcecode =
		"#version 460 core" \
		"\n" \
		"in vec4 a_color_out;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = a_color_out;" \
		"}";

	//create the shader
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	
	//give shader code to shader object
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourcecode, NULL);

	//compile the code(programatically compilation by OpenGL engine (machine code for GPU) (inline compiler)
	glCompileShader(fragmentShaderObject);
	
	//reinitialization
	 status=0;
	 infoLogLength=0;
	 log = NULL;

	 //a.getting compilation status
	 glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	 
	 if (status == GL_FALSE)
	 {
		 //b.getting length of log (message) compilation
		 glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 //c.allocate enough memory
			 log = (char*)malloc(infoLogLength);
			 if (log != NULL)
			 {
				 //d.get the compilation log into this buffer
				 GLsizei written;
				 glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				 //e.display the contents of buffer
				 fprintf(gpFile, "fragment shader compilation log: %s\n", log);
				 //f.free log
				 free(log);
				 //g.exit the application
				 uninitialize();
			 }
		 }
	 }
	
	 //d.1.shader program object
	 shaderProgramObject = glCreateProgram();
	 
	 //d.2.attach desire shaders to this shader obj
	 glAttachShader(shaderProgramObject, vertexShaderObject);
	 glAttachShader(shaderProgramObject, fragmentShaderObject);

	 //a1.prelink binding of shader program with vertex attributes
	 glBindAttribLocation(shaderProgramObject,AMC_ATRIBUTE_POSITION,"a_position");
	 glBindAttribLocation(shaderProgramObject, AMC_ATRIBUTE_COLOR, "a_color");


	 //d.3.link shader obj
	 glLinkProgram(shaderProgramObject);

	 //d.4.linking status
	 
	 status = 0;
	 infoLogLength = 0;
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
				 GLsizei written;
				 glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				 fprintf(gpFile, "shader pprogram link log:%s\n", log);
				 free(log);

				 uninitialize();
			 }
		 }
	 }

	 mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

	 
	 //declaration of vertex data array
	 const GLfloat pyramidPosition[] =
	 {
		// front
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		// right
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		// back
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		// left
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f

	 };

	 const GLfloat pyramidColor[] =
	 {
		 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f

	 };

	 const GLfloat cubePosition[] =
	 {
			 // top
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		// bottom
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,

		 // front
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,

		 // back
		 1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		 // right
		 1.0f, 1.0f, -1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,

		 // left
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

	 };

	 const GLfloat cubeColor[]
	 {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f

	 
	 };
	//VAO and VBO related code
	 glGenVertexArrays(1, &VAO_Pyramid);//1 VAO
	 /*1 address dila jato bind n unbind madhe ja lines lihilya jatat tya saglya record hotat*/
	 glBindVertexArray(VAO_Pyramid);

	 //VBO for position
	 glGenBuffers(1, &VBO_Pyramid_Position);
	 glBindBuffer(GL_ARRAY_BUFFER,VBO_Pyramid_Position);
	 //array hold karnara buffer. gpu ch aat memory milali tyat target kele

	 //data bharaychay target cha side ni liha GL_STATIC_DRAW->attach bhar data
	 glBufferData(GL_ARRAY_BUFFER,
		 sizeof(pyramidPosition), pyramidPosition, GL_STATIC_DRAW);

	 //AMC_ATRIBUTE_POSITION=0
	 /*shader madhe me 0th pos la ha data map karat ahe (targetaed data from gpu memory),
		yache tu baghtana 3-3 cha set kar,
		tyala float data type laav(gpu float vegla asto ),not sure data normalize(1,-1) asel he sangto,kiticha stride(dhanga) taku,dhanga takaycha astil tar to data kuthay*/
	 glVertexAttribPointer(AMC_ATRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL );

	 glEnableVertexAttribArray(AMC_ATRIBUTE_POSITION);

	 //VBO ha opec pointer ahe
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //VBO for color
	 glGenBuffers(1, &VBO_Cube_Position);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube_Position);
	 glBufferData(GL_ARRAY_BUFFER,
		 sizeof(pyramidColor), pyramidColor, GL_STATIC_DRAW);
	
	 glVertexAttribPointer(AMC_ATRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATRIBUTE_COLOR);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	 glBindVertexArray(0);

	 //cube
	 //VAO and VBO related code
	 glGenVertexArrays(1, &VAO_Cube);//1 VAO
	 /*1 address dila jato bind n unbind madhe ja lines lihilya jatat tya saglya record hotat*/
	 glBindVertexArray(VAO_Cube);

	 //VBO for Cube position
	 glGenBuffers(1, &VBO_Cube_Position);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube_Position);
	 glBufferData(GL_ARRAY_BUFFER,
		 sizeof(cubePosition), cubePosition, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATRIBUTE_POSITION);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 //VBO for Cube color
	 glGenBuffers(1, &VBO_Cube_Color);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube_Color);
	 glBufferData(GL_ARRAY_BUFFER,
		 sizeof(cubeColor), cubeColor, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATRIBUTE_COLOR);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	 glBindVertexArray(0);


	// Here starts OpenGL code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	persepectiveProjectionMAtrix = mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);
	
	return(0);
}

void printGLInfo(void)
{
	//local vatiavable declaration
	GLint numExtensions=0;

	//code
	fprintf(gpFile,"OpenGL vendor: %s \n",glGetString(GL_VENDOR));
	fprintf(gpFile,"OpenGL renderer: %s \n",glGetString(GL_RENDERER));
	fprintf(gpFile,"OpenGL version: %s \n",glGetString(GL_VERSION));
	fprintf(gpFile,"GLSL version: %s \n",glGetString(GL_SHADING_LANGUAGE_VERSION));//GLSL--?graphic library shading language


	glGetIntegerv(GL_NUM_EXTENSIONS,&numExtensions);
	fprintf(gpFile, "number of supported extensions : %d\n",numExtensions );
	//fprintf(gpFile,"OpenGL version: %s \n",glGetString(GL_VERSION));
	for (int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "GLSL Version :%s\n",glGetStringi(GL_EXTENSIONS,i));
	}
}

void resize(int width, int height)
{
	// Code
	if (height == 0) // To avoid divide by zero instruction (illegal instruction) in future
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	persepectiveProjectionMAtrix = vmath::perspective(45.0f,
		((GLfloat)width / (GLfloat)height),
		0.1f,
		100.0f);
		
	
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//je kahi draw karnare shader madhe means code te ethe use karnar
	//e.1.use shader program object
	glUseProgram(shaderProgramObject);
	//"Triangle"
	//transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(-1.5f, 0.0f, -6.0f);
	rotationMatrix = vmath::rotate(anglePyramid,0.0f,1.0f,0.0f);
	//mat4 modelViewMatrix = translationMatrix*rotationMatrix;
	mat4 modelViewMatrix = mat4::identity();
	//mat4 modelViewMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); 
	modelViewMatrix = translationMatrix * rotationMatrix;
	
	modelViewProjectionMatrix = persepectiveProjectionMAtrix * modelViewMatrix;

	glUniformMatrix4fv(
		mvpMatrixUniform,//initialize madhe jo uniform ghetlay to
		1,
		GL_FALSE,
		modelViewProjectionMatrix
	);

	glBindVertexArray(VAO_Pyramid);

	glDrawArrays(
	GL_TRIANGLES,
		0,
		12
	);

	glBindVertexArray(0);

	glUseProgram(shaderProgramObject);
	//"cube"
	//transformations
	 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	translationMatrix = vmath::translate(1.5f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	 modelViewMatrix = mat4::identity();
	//mat4 modelViewMatrix = mat4::identity();
	 modelViewProjectionMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -4.0f); 
	modelViewMatrix = translationMatrix*scaleMatrix*rotationMatrix;

	modelViewProjectionMatrix = persepectiveProjectionMAtrix * modelViewMatrix;

	glUniformMatrix4fv(
		mvpMatrixUniform,//initialize madhe jo uniform ghetlay to
		1,
		GL_FALSE,
		modelViewProjectionMatrix
	);

	glBindVertexArray(VAO_Cube);

	glDrawArrays(
		GL_TRIANGLE_FAN,
		0,
		4
	);
	glDrawArrays(
		GL_TRIANGLE_FAN,
		4,
		4
	);
	glDrawArrays(
		GL_TRIANGLE_FAN,
		8,
		4
	);
	glDrawArrays(
		GL_TRIANGLE_FAN,
		12,
		4
	);
	glDrawArrays(
		GL_TRIANGLE_FAN,
		16,
		4
	);
	glDrawArrays(
		GL_TRIANGLE_FAN,
		20,
		4
	);
	glBindVertexArray(0);

	//e.2.draw desire graphics here

	
	//e.3.unuse shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code
	anglePyramid += 1.5f;
	if (anglePyramid >= 360.0f)
		anglePyramid -= 360;

	angleCube += 1.5f;
	if (angleCube >= 360.0f)
		angleCube -= 360;
}

void uninitialize(void)
{
	// Local function declarations
	void ToggleFullscreen(void);

	// Code
	if (gbFullScreen)
	{
		ToggleFullscreen();
	}

	//delesion of vbo
	if (VBO_Pyramid_Color)
	{
		glDeleteBuffers(1, &VBO_Pyramid_Color);
		VBO_Pyramid_Color = 0;
	}
	if (VBO_Pyramid_Position)
	{
		glDeleteBuffers(1, &VBO_Pyramid_Position);
		VBO_Pyramid_Position = 0;
	}
	if (VBO_Cube_Position)
	{
		glDeleteBuffers(1, &VBO_Cube_Position);
		VBO_Cube_Position = 0;
	}


	//delesion of VAO
	if (VAO_Pyramid)
	{
		glDeleteVertexArrays(1, &VAO_Pyramid);
			VAO_Pyramid = 0;
	}
	if (VAO_Cube)
	{
		glDeleteVertexArrays(1, &VAO_Cube);
		VAO_Cube = 0;
	}
	if (VBO_Cube_Color)
	{
		glDeleteVertexArrays(1, &VBO_Cube_Color);
		VBO_Cube_Color = 0;
	}
	//shader uninitialization
	if (shaderProgramObject)
	{

		//f.0.again use shader program object
		glUseProgram(shaderProgramObject);
		GLsizei numAttachedShaders;
		//f.1.get no of attach shaders
		glGetProgramiv(shaderProgramObject,GL_ATTACHED_SHADERS,&numAttachedShaders);
		//f.2.create empty buffer to hold array of shader objects
		GLuint* shaderObjects = NULL;
		//f.3.
		shaderObjects = (GLuint*)malloc(numAttachedShaders*sizeof(GLuint));
		//f.4 fill it
		glGetAttachedShaders(shaderProgramObject,numAttachedShaders,&numAttachedShaders,shaderObjects);
		//f.5.
		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);

			shaderObjects[i] = 0;
		}

		//f.6 free memory allocated for buffer
		free(shaderObjects);
		shaderObjects = NULL;

		//f.7.unuse shader program object
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;


	}


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
		fprintf(gpFile, "Log file successfully closed.\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}
