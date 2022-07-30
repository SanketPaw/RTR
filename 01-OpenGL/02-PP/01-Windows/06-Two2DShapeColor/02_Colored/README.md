--------------------------------------------------------------------
PREREQUISITES
--------------------------------------------------------------------
* The template for this code is skeleton code.

--------------------------------------------------------------------
CHANGES
--------------------------------------------------------------------
* We are renaming the file to OGL.cpp from Window.c. The reason is, we will be adding a header file of cpp in future.
* We are adding below functions:
	1. initialize()
	2. resize()
	3. display()
	4. update()
	5. uninitialize()
* Need to add some cases in switch inside the WndProc():
	1. WM_ERASEBKGND
	2. WM_SIZE
	3. WM_CLOSE
* In WinMain() we are going to set the window foreground using SetWindowForeground() Win32 API.
* In WinMain() we are going to set the window focus using SetFocus() Win32 API.
* initialize():
	1. We are gooing to call initialize() after creating the window.
* resize():
	1. We are going to call resize() when WndProc() will receive WM_SIZE window message.
* display():
	1. We are going to call display() in the game loop when PeekMessage() will return FALSE.
* update():
	1. We are going to call update() in the game loop after call to display() when PeekMessage() will return FALSE.
* uninitialize():
	1. Uninitialize function will be called from WM_DESTROY case before calling PostQuitMessage().
	2. This can be called from elsewhere also.
	3. This function will:
		a. Take windoww out of full-screen if it is in full-screen mode.
		b. Destroy window in case the call to uninitialize() is not coming from WM_DESTROY.
		c. Close file handle if it is open.

--------------------------------------------------------------------
IMPORTANT NOTES:
--------------------------------------------------------------------
* After clicking close button WM_CLOSE is sent and then WM_DESTROY is also sent by default.
* But we call DestroyWindow() in WM_CLOSE window message handler to deliberately send WM_DESTROY to the window. Its not compulsion but its convention.