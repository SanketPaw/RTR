#include <GL\glut.h>
#include <iostream>
#include <windows.h>

// Function to initialize the drivers
void myInit(void)
{
	// Clear all the screen color
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Sets background color to black
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Specify the display area
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

// Function to display 
void myDisplay(void)
{
	glColor3f(0.05f, 0.05f, 0.05f);

	// Clear the screen buffer
	glClear(GL_COLOR_BUFFER_BIT);

	
	// Begin the polygon
	glBegin(GL_POLYGON);

	// Create the polygon
	glVertex2i(30, 10);
	glVertex2i(80, 10);

	glVertex2i(30, 20);
	glVertex2i(80, 20);

	glVertex2i(30, 20);
	glVertex2i(30, 10);

	glVertex2i(80, 20);
	glVertex2i(80, 10);

	glEnd();

	// Sends all output to display
	glFlush();
}

// Driver Code
int main(int argc, char** argv)
{
	// Initialize the init function
	glutInit(&argc, argv);

	// Initialize the toolkit;
	glutInitDisplayMode(
		GLUT_SINGLE | GLUT_RGB);

	// Sets the display mode and
	// specify the colour scheme
	glutInitWindowSize(800, 600);

	// Specify the window size
	glutInitWindowPosition(0, 0);

	// Sets the starting position
	// for the window
	glutCreateWindow("Swastik");

	// Creates the window and
	// sets the title
	glutDisplayFunc(myDisplay);
	myInit();

	// Additional initializations
	glutMainLoop();

	// Go into a loop until event
	// occurs
	return 0;
}
