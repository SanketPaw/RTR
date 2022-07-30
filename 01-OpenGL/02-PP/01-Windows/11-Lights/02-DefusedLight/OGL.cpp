/*
1.calculate i coordinates by multiplying position (vector) with modelViewMatrix 
2.calculate normal matrix from modelViewMateix (normal matrix : inverse of transepose of upper left 3X3 modelViewMatrix )
3.(note-OpenGL is Column major) calculate transformed normals by multiplying normals with above calculated normal matrix and normalize the result
4.calculte the light direction by subtracting i coordinates (...step 1) from light position
5.calculate diffused light by following equation ld * kd * (s.n)....(step 4 light direction * step 3 transform normals)

steps for
*/

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

GLuint VAO_Cube;
GLuint VBO_Cube_Position;
GLuint VBO_Cube_Normal;
GLuint VBO_Cube_Color;
GLfloat angleCube=0.0f;


GLuint mvpMatrixUniform;
GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;
GLuint modelMatrix;
GLuint viewMatrix;
GLuint ldUniform;
GLuint kdUniform;
GLuint lightPositionUniform;

GLuint lightingEnabledUniform;
BOOL bLight = FALSE;
mat4 perspectiveProjectionMatrix;
GLfloat lightDefuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 0.0f,0.0f,2.0f,1.0f };
GLfloat materialDefuse[] = { 1.0f,1.0f,1.0f,1.0f };

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

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
			}
			else
			{
				bLight = FALSE;
			}

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
		"#version 400 core" \
		"\n" \
		"in vec4 a_position;" \
		"in vec3 a_normal;" \
		"uniform mat4 u_modelMatrix;" \
		"uniform mat4 u_viewMatrix;" \
		"uniform mat4 u_projectionMatrix;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_kd;" \
		"uniform vec4 u_lightPosition;" \
		"uniform int u_lightingEnabled;" \
		"out vec3 diffused_light_color;" \
		"void main(void)" \
		"{" \
		"if (u_lightingEnabled == 1)" \
		"{" \
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position; " \
		"mat3 normalMatrix =mat3(transpose(inverse( u_viewMatrix * u_modelMatrix)));" \
		"vec3 transformNormals = normalize(normalMatrix * a_normal);" \
		"vec3 lightDirection = normalize(vec3(u_lightPosition - eyeCoordinates));" \
		"diffused_light_color = u_ld * u_kd * max(dot(lightDirection,transformNormals),0.0);" \
		"}" \
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" \
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
		"#version 400 core" \
		"\n" \
		"in vec3 diffused_light_color;" \
		"uniform int u_lightingEnabled;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"if(u_lightingEnabled == 1)" \
		"{" \
		"FragColor = vec4(diffused_light_color,1.0);" \
		"}" \
		"else" \
		"{" \
		"FragColor = vec4(1.0, 1.0, 1.0, 1.0);" \
		"}" \
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
	 glBindAttribLocation(shaderProgramObject,AMC_ATRIBUTE_NORMAL,"a_normal");


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

	 modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
	 viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	 projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
	 ldUniform = glGetUniformLocation(shaderProgramObject,"u_ld");
	 kdUniform = glGetUniformLocation(shaderProgramObject,"u_kd");
	 lightPositionUniform = glGetUniformLocation(shaderProgramObject,"u_lightPosition");
	 lightingEnabledUniform= glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");
	 
	 //declaration of vertex data array
	
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

	 const GLfloat  cubeNormals[] =
	 {
		 // top surface
		 0.0f, 1.0f, 0.0f,  // top-right of top
		 0.0f, 1.0f, 0.0f, // top-left of top
		 0.0f, 1.0f, 0.0f, // bottom-left of top
		 0.0f, 1.0f, 0.0f,  // bottom-right of top

		 // bottom surface
		 0.0f, -1.0f, 0.0f,  // top-right of bottom
		 0.0f, -1.0f, 0.0f,  // top-left of bottom
		 0.0f, -1.0f, 0.0f,  // bottom-left of bottom
		 0.0f, -1.0f, 0.0f,   // bottom-right of bottom

		 // front surface
		 0.0f, 0.0f, 1.0f,  // top-right of front
		 0.0f, 0.0f, 1.0f, // top-left of front
		 0.0f, 0.0f, 1.0f, // bottom-left of front
		 0.0f, 0.0f, 1.0f,  // bottom-right of front

		 // back surface
		 0.0f, 0.0f, -1.0f,  // top-right of back
		 0.0f, 0.0f, -1.0f, // top-left of back
		 0.0f, 0.0f, -1.0f, // bottom-left of back
		 0.0f, 0.0f, -1.0f,  // bottom-right of back

		 // right surface
		 1.0f, 0.0f, 0.0f,  // top-right of right
		 1.0f, 0.0f, 0.0f,  // top-left of right
		 1.0f, 0.0f, 0.0f,  // bottom-left of right
		 1.0f, 0.0f, 0.0f,  // bottom-right of right

		 // left surface
		 - 1.0f, 0.0f, 0.0f, // top-right of left
		 -1.0f, 0.0f, 0.0f, // top-left of left
		 -1.0f, 0.0f, 0.0f, // bottom-left of left
		 -1.0f, 0.0f, 0.0f // bottom-right of left
	 };

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

	 //mormal
	 glGenBuffers(1, &VBO_Cube_Normal);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube_Normal);
	 glBufferData(GL_ARRAY_BUFFER,
		 sizeof(cubeNormals), cubeNormals, GL_STATIC_DRAW);
	 glVertexAttribPointer(AMC_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATRIBUTE_NORMAL);
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);

	// Here starts OpenGL code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//depth related changes
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	perspectiveProjectionMatrix = mat4::identity();

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

	perspectiveProjectionMatrix = vmath::perspective(45.0f,
		((GLfloat)width / (GLfloat)height),
		0.1f,
		100.0f);
		
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//je kahi draw karnare shader madhe means code te ethe use karnar
	//e.1.use shader program object
	glUseProgram(shaderProgramObject);

	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix_X = mat4::identity();
	mat4 rotationMatrix_Y = mat4::identity();
	mat4 rotationMatrix_Z = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	//cube
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	rotationMatrix_X = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	rotationMatrix_Y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	rotationMatrix_Z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

	rotationMatrix = rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

	modelMatrix = translationMatrix * rotationMatrix;


	glUniformMatrix4fv(
		modelMatrixUniform,//initialize madhe jo uniform ghetlay to
		1,
		GL_FALSE,
		modelMatrix
	);

	glUniformMatrix4fv(
		viewMatrixUniform,//initialize madhe jo uniform ghetlay to
		1,
		GL_FALSE,
		viewMatrix
	);

	glUniformMatrix4fv(
		projectionMatrixUniform,//initialize madhe jo uniform ghetlay to
		1,
		GL_FALSE,
		perspectiveProjectionMatrix
	);

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);
		glUniform3fv(ldUniform, 1,lightDefuse);
		glUniform3fv(kdUniform,1,materialDefuse);
		glUniform4fv(lightPositionUniform, 1, lightPosition);

	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

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
	
	if (VBO_Cube_Position)
	{
		glDeleteBuffers(1, &VBO_Cube_Position);
		VBO_Cube_Position = 0;
	}


	//delesion of VAO

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

	if (VBO_Cube_Normal)
	{
		glDeleteVertexArrays(1, &VBO_Cube_Normal);
		VBO_Cube_Normal = 0;
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


