#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif
#include<cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>





int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
#define PI 3.14159265358979323846

void display();
void reshape(int w, int h);
void timer(int millisec);
void mouse(int button, int stat, int x, int y);
void mouseMotion(int x, int y);
int  initGLUT(int argc, char** argv);
void circleMidpoint(int radius, int z);
void circleMidpoint2(int radius, int z);
int xCenter = 0;
int yCenter = 0;
int zCenter = 0;



void setPixel(int x, int y, int z)
{
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
	glFlush();
}
//glBegin(GL_LINE_LOOP);

void mySphere(float r, int n, int m) {
	float x, y, z, i, j, t = 0, re;

	for (j = 0; j < n + 1; j += 1) {
		t = (float)(t + PI / (n + 1));
		z = (float)(r * cos(t));
		re = (float)(r * sin(t));
		circleMidpoint(re, z);
		glEnd();
	}

	t = 0;
	for (j = 0; j < m + 1; j += 1) {
		t = (float)(t + PI / (m + 1));
		z = (float)(r * cos(t));
		re = (float)(r * sin(t));
		circleMidpoint2(re, z);
		glEnd();
	}
}







void circlePlotPoints(int x, int y, int z)
{
	setPixel(xCenter + x, yCenter + y, zCenter + z);
	setPixel(xCenter - x, yCenter + y, zCenter + z);
	setPixel(xCenter + x, yCenter - y, zCenter + z);
	setPixel(xCenter - x, yCenter - y, zCenter + z);
	setPixel(xCenter + y, yCenter + x, zCenter + z);
	setPixel(xCenter - y, yCenter + x, zCenter + z);
	setPixel(xCenter + y, yCenter - x, zCenter + z);
	setPixel(xCenter - y, yCenter - x, zCenter + z);
}

void circleMidpoint(int radius, int z)
{

	int x = 0;
	int y = radius;
	int f = 1 - radius;

	circlePlotPoints(x, y, z);
	while (x < y) {
		x++;
		if (f < 0)
			f += 2 * x + 1;
		else {
			y--;
			f += 2 * (x - y) + 1;
		}
		circlePlotPoints(x, y, z);
	}
}




void circlePlotPoints2(int x, int y, int z)
{
	setPixel(xCenter + x, zCenter + z, yCenter + y);
	setPixel(xCenter - x, zCenter + z, yCenter + y);
	setPixel(xCenter + x, zCenter + z, yCenter - y);
	setPixel(xCenter - x, zCenter + z, yCenter - y);
	setPixel(xCenter + y, zCenter + z, yCenter + x);
	setPixel(xCenter - y, zCenter + z, yCenter + x);
	setPixel(xCenter + y, zCenter + z, yCenter - x);
	setPixel(xCenter - y, zCenter + z, yCenter - x);
}
void circleMidpoint2(int radius, int z)
{


	int x = 0;
	int y = radius;
	int f = 1 - radius;

	circlePlotPoints2(x, y, z);
	while (x < y) {
		x++;
		if (f < 0)
			f += 2 * x + 1;
		else {
			y--;
			f += 2 * (x - y) + 1;
		}
		circlePlotPoints2(x, y, z);
	}
}




int main(int argc, char** argv)
{


	initGLUT(argc, argv);


	glutMainLoop();

	return 0;
}



int initGLUT(int argc, char** argv)
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glutInitWindowSize(800, 800);

	glutInitWindowPosition(500, 0);


	int handle = glutCreateWindow(argv[0]);


	glutDisplayFunc(display);
	glutTimerFunc(50, timer, 50);
	glutReshapeFunc(reshape);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	return handle;
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.1, 1, 0.1, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(2);
	glPushMatrix();
	glTranslatef(0, 0, -cameraDistance - 600);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);


	mySphere(100, 50, 50);

	glPopMatrix();
	glutSwapBuffers();
}



void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)(screenWidth) / screenHeight, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#ifdef _WIN32
#endif
}


void timer(int millisec)
{
	glutTimerFunc(millisec, timer, millisec);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}

}


void mouseMotion(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistance -= (y - mouseY) * 0.2f;
		mouseY = y;
	}

}