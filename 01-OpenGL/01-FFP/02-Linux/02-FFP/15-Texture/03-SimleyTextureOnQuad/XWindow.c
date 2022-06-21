#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include <GL/gl.h>   // for OpenGL APIs
#include <GL/glu.h>  // for OpenGL utility functions
#include <GL/glx.h>  // for GLX APIs (bridging API)

#include <SOIL/SOIL.h> // Simple OpenGL Image Library for texture loading

// global variable declarations
Display *gpDisplay = NULL;
Window gWindow;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;

GLXContext gGLXContext;

Bool bFullscreen = False;
int giWindowWidth = 800;
int giWindowHeight = 600;

unsigned int pressedDigit;
GLuint texture_smiley;

// entry-point function
int main(void)
{
	// function prototypes
	void CreateWindow(void);
	void ToggleFullscreen(void);

	void initialize(void);
	void display(void);
	void update(void);
	void resize(int, int);
	void uninitialize(void);

	// variable declarations
	int winWidth = giWindowWidth;
	int winHeight = giWindowHeight;

	Bool bDone = False;

	// code
	CreateWindow();
	initialize();

	// game loop
	XEvent event;
	KeySym keysym;

	while (bDone == False)
	{
		while (XPending(gpDisplay))
		{
			XNextEvent(gpDisplay, &event);
			switch (event.type)
			{
				case MapNotify:
					break;

				case KeyPress:
					keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
					switch (keysym)
					{
						case XK_Escape:
							bDone = True;
							break;

						case XK_F:
						case XK_f:
							if (bFullscreen == False)
							{
								ToggleFullscreen();
								bFullscreen = True;
							}
							else
							{
								ToggleFullscreen();
								bFullscreen = False;
							}
							break;
					
						default:
							break;
					}
					
					char key;
					XLookupString(&event.xkey, &key, 1, NULL, NULL);
					switch (key)
					{
					case '0':
						pressedDigit = 0;
						break;

					case '1':
						pressedDigit = 1;
						break;

					case '2':
						pressedDigit = 2;
						break;

					case '3':
						pressedDigit = 3;
						break;

					case '4':
						pressedDigit = 4;
						break;
					
					default:
						break;
					}

					break;

				case ButtonPress:
					switch (event.xbutton.button)
					{
						case 1:
							break;

						case 2:
							break;

						case 3:
							break;
						
						default:
							break;
					}
					break;

				case MotionNotify:
					break;

				case ConfigureNotify:
					winWidth = event.xconfigure.width;
					winHeight = event.xconfigure.height;
					resize(winWidth, winHeight);
					break;

				case Expose:
					break;

				case DestroyNotify:
					break;

				case 33:
					bDone = True;
					break;
				
				default:
					break;
			}
		}

		update();
		display();
	}

	uninitialize();
	return(0);
}

void CreateWindow(void)
{
	// function prototypes
	void uninitialize(void);

	// variable declarations
	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int styleMask;

	// ~ pixel format descriptor
	static int frameBufferAttributes[] = {
		GLX_DOUBLEBUFFER, True,
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		None // here '0' is also possible
	};

	// code
	gpDisplay = XOpenDisplay(NULL);
	if (gpDisplay == NULL)
	{
		printf("ERROR: Unable to open XDisplay.\nExiting..\n");
		uninitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);

	gpXVisualInfo = glXChooseVisual(gpDisplay, defaultScreen, frameBufferAttributes);
	if (gpXVisualInfo == NULL)
	{
		printf("ERROR: Unable to get a Visual.\nExiting..\n");
		uninitialize();
		exit(1);
	}

	winAttribs.border_pixel = 0;
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		gpXVisualInfo->visual,
		AllocNone);
	gColormap = winAttribs.colormap;
	winAttribs.background_pixel = BlackPixel(gpDisplay, gpXVisualInfo->depth);

	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
	
	gWindow = XCreateWindow(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		0,
		0,
		giWindowWidth,
		giWindowHeight,
		0,
		gpXVisualInfo->depth,
		InputOutput,
		gpXVisualInfo->visual,
		styleMask,
		&winAttribs);
	if (!gWindow)
	{
		printf("ERROR: Failed to create Main Window.\nExiting..\n");
		uninitialize();
		exit(1);
	}
	
	XStoreName(gpDisplay, gWindow, "SSP : OGL");

	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete, 1);

	XMapWindow(gpDisplay, gWindow);
}

void ToggleFullscreen(void)
{
	// variable declarations
	Atom wm_state;
	Atom fullscreen;
	XEvent xev = {0};

	// code
	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
	memset(&xev, 0, sizeof(XEvent));

	xev.type = ClientMessage;
	xev.xclient.window = gWindow;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = bFullscreen ? 0:1;

	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		False,
		StructureNotifyMask,
		&xev);
}

void initialize(void)
{
	// function declarations
	void resize(int, int);
	void uninitialize(void);
	GLuint loadBitmapAsTexture(const char *);

	// code
	gGLXContext = glXCreateContext(gpDisplay, gpXVisualInfo, NULL, GL_T);
	glXMakeCurrent(gpDisplay, gWindow, gGLXContext);

	// opengl
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);

	// texture
	glEnable(GL_TEXTURE_2D);
	texture_smiley = loadBitmapAsTexture("smiley.bmp");

	// warmup resize
	resize(giWindowWidth, giWindowHeight);
}

void resize(int width, int height)
{
	// code
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// RECTANGLE
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);

	if(pressedDigit == 0)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
		glBindTexture(GL_TEXTURE_2D, texture_smiley);

	if (pressedDigit == 1)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
	}
	else if (pressedDigit == 2)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.5f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
	}
	else if (pressedDigit == 3)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(2.0f, 2.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 2.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(2.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
	}
	else if (pressedDigit == 4)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.5f);
			glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();
	}

	glXSwapBuffers(gpDisplay, gWindow);
}

void update(void)
{
	// code
}

void uninitialize(void)
{
	if (texture_smiley)
	{
		glDeleteTextures(1, &texture_smiley);
		texture_smiley = 0;
	}

	GLXContext currentGLXContext = glXGetCurrentContext();
	if (currentGLXContext != NULL && currentGLXContext == gGLXContext)
	{
		glXMakeCurrent(gpDisplay, 0, 0);
	}

	if (gGLXContext)
	{
		glXDestroyContext(gpDisplay, gGLXContext);
	}

	if (gWindow)
	{
		XDestroyWindow(gpDisplay, gWindow);
	}

	if (gColormap)
	{
		XFreeColormap(gpDisplay, gColormap);
	}

	if (gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}

	if (gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}

// texture helper function
GLuint loadBitmapAsTexture(const char *path)
{
	int width, height;
	unsigned char *imageData = NULL;
	GLuint textureID = 0;

	imageData = SOIL_load_image(path, &width, &height, NULL, SOIL_LOAD_RGB);

	// opengl code
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// setting texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// push data into graphics memory with the help of driver
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	SOIL_free_image_data(imageData);
	return(textureID);
}
