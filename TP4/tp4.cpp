#define GLUT_DISABLE_ATEXIT_HACK	
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


//variables para el gizmo
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)


///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
		break;


	}
}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	
	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


//function called on each frame
GLvoid window_idle();

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 3 : Transformations 3D Part 2");


	initGL();
	init_scene();

	
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	
	
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events
	
	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}



GLvoid initGL()
{

	glEnable(GL_DEPTH_TEST);


	glClearColor(RED, GREEN, BLUE, ALPHA);
}


void Gizmo3D(){


	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();


	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();


	return;

}

void draw_floor(){
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-20.0f, 0.0f, -20.0f);
	glVertex3f(-20.0f, 0.0f, 20.0f);
	glVertex3f(20.0f, 0.0f, 20.0f);
	glVertex3f(20.0f, 0.0f, -20.0f);
	glEnd();
}

void draw_wall(float x1, float y1, float x2, float y2, float z, int axis){
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 0.0f);
	if (axis == 0){ //x
		glVertex3f(z, x1, y1);
		glVertex3f(z, x1, y2);
		glVertex3f(z, x2, y2);
		glVertex3f(z, x2, y1);
	} else if (axis == 1){ //y
		glVertex3f(x1, z, y1);
		glVertex3f(x1, z, y2);
		glVertex3f(x2, z, y2);
		glVertex3f(x2, z, y1);
	} else { //z
		glVertex3f(x1, y1, z);
		glVertex3f(x1, y2, z);
		glVertex3f(x2, y2, z);
		glVertex3f(x2, y1, z);
	}
	glEnd();
}

void draw_roof(){
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
		glVertex3f(-7.5f, 5.0f, -5.0f);
		glVertex3f(-7.5f, 5.0f, 0.0f);
		glVertex3f(-7.5f, 8.0f, -2.5f);
	glEnd();
	
	glBegin(GL_POLYGON);
		glVertex3f(0.0f, 5.0f, -5.0f);
		glVertex3f(0.0f, 5.0f, 0.0f);
		glVertex3f(0.0f, 8.0f, -2.5f);
	glEnd();
	
	glColor3f(0.5f,0.5f,0.5f);
	
	glBegin(GL_POLYGON);
		glVertex3f(-7.5f, 5.0f, -5.0f);
		glVertex3f(-7.5f, 8.0f, -2.5f);
		glVertex3f(0.0f, 8.0f, -2.5f);
		glVertex3f(0.0f, 5.0f, -5.0f);
	glEnd();
	
	glBegin(GL_POLYGON);
		glVertex3f(-7.5f, 5.0f, 0.0f);
		glVertex3f(-7.5f, 8.0f, -2.5f);
		glVertex3f(0.0f, 8.0f, -2.5f);
		glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();
	
}

void draw_house(){
	draw_wall(-7.5f, 0.0f, 0.0f, 5.0f, -5.0f, 2);
	draw_wall(-7.5f, 0.0f, 0.0f, 5.0f, 0.0f, 2);
	draw_wall(0.0f, -5.0f, 5.0f, 0.0f, -7.5f, 0);
	draw_wall(0.0f, -5.0f, 5.0f, 0.0f, 0.0f, 0);
	draw_roof();
}



GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f, 1.0f, 0.01f, 100.0f);


	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);


	//Gizmo3D();
	draw_floor();
	draw_house();

	

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;
	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{
	glutPostRedisplay();
}





