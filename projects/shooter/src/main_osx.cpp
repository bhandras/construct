//A couple includes
#include <iostream>//This is just for basic io, you really DON’T need it
#include <GLUT/GLUT.h>//GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h>//OpenGL Library
#include <sys/timeb.h>

#include "construct.h"
#include "game.h"


int GetMilliCount()
{
	// Something like GetTickCount but portable
	// It rolls over every ~ 12.1 days (0x100000/24/60/60)
	// Use GetMilliSpan to correct for rollover
	timeb tb;
	ftime( &tb );
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}


Game game;
int w = 1024;
int h = 768;
int mouseX = 0;
int mouseY = 0;
bool button1 = false;
bool button2 = false;
unsigned int deltaTimeMs = 0;


void myDisplay(void)
{
	unsigned int startTick = GetMilliCount();
	
	glViewport (0, 0, w, h);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	game.step(mouseX, mouseY, button1, button2, deltaTimeMs);
	
	glFlush();
	
	unsigned int endTick = GetMilliCount();
	deltaTimeMs = (endTick - startTick);
}


int main (int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("gl test");
	glutDisplayFunc(myDisplay);

	game.init(w, h);
	
	glutMainLoop();
	return 0;
}

