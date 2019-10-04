#define GLUT_DISABLE_ATEXIT_HACK
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

#define CAMERA_OFFSET 5.0f

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

bool perspective = 1;
bool camera = 0;
int exercise = 1;

float bias = 0.5f;

int cursor_x, cursor_y;
float speed = 0.1f;
float ax = 0.0f;
float ay = 0.0f;

float cur_x = 0.0f;
float cur_z = 0.0f;

double fov = 90.0, aspect = 1.0, zNear = 1.0, zFar = 50.0;

//function called on each frame
GLvoid window_idle();

GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };
	
	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);
	
	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	
	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glClearColor(RED, GREEN, BLUE, ALPHA);
}

float a = 0.0f, spin_rate = 1.0f;
float x_position, y_position;

void reset_values(){
	fov = 90.0;
	aspect = 1.0;
	zNear = 1.0; 
	zFar = 50.0;
}

void exercise_1(){
	if (perspective && !camera) glTranslatef(0.0f, 0.0f, -30.0f); //gluPerspective
	float radius = 8.0f;
	glutSolidSphere(0.25f, 8, 8);
	glColor3f(0.0f, 1.0f, 0.0f);
	
	x_position = sin(a * M_PI/180) * radius;
	y_position = cos(a * M_PI/180) * radius;
	glTranslatef(x_position, y_position, 0.0f);
	glutSolidTeapot(2.5f);
	a += spin_rate;
	reset_values();
}

void exercise_2(){
	
	glTranslatef(0.0f, 0.0f, -40.0f);
	//glutSolidTeapot(1.5f);
	
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex3d(0, 0,0);
	glVertex3d(15, 0,0);
	glColor3d(0, 255, 0);
	glVertex3d(0, 0,0);
	glVertex3d(0, 15,0);
	glColor3d(0, 0, 255);
	glVertex3d(0, 0,0);
	glVertex3d(0, 0,15);
	glEnd();
}

//function called on each frame
GLvoid window_idle()
{		
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		cur_z -= bias;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
		
	case GLUT_KEY_DOWN:
		cur_z += bias;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
		
	case GLUT_KEY_LEFT:					
		cur_x -= bias;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
		
	case GLUT_KEY_RIGHT:
		cur_x += bias;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
		cursor_x = x;
		cursor_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	int dx, dy;
	dx = cursor_x - x;
	dy = cursor_y - y;
	
	cursor_x = x;
	cursor_y = y;
	
	ax += dx * speed;
	ay += dy * speed;
	
	glutPostRedisplay();						
}


GLvoid window_display()
{
	
	double eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz;
	
	eyex = 0.0;
	eyey = 0.0;
	eyez = 30.0;
	
	centerx = 0.0;
	centery = 0.0;
	centerz = 0.0;
	
	upx = 0.0;
	upy = 1.0;
	upz = 0.0;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	
	if (perspective || exercise == 2){
		gluPerspective(45.0f, 1.0f, 1.0f, 400.0f);
		if (camera && exercise == 1){
			gluLookAt(eyex, eyey, eyez + CAMERA_OFFSET, centerx, centery, centerz, upx, upy, upz);
		}
	} else {
		glOrtho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
	}
	
	if (exercise == 2){
		glTranslatef(cur_x, 0.0f, cur_z);
		
		glRotatef(ax, 0.0f, 1.0f, 0.0f);
		glRotatef(ay, 1.0f, 0.0f, 0.0f);
		
	}
	
	/*dibujar aqui*/
	
	if (exercise == 1) exercise_1();
	else if (exercise == 2) exercise_2();
	
	glutSwapBuffers();
	
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	
	if (perspective){
		gluPerspective(90.0f, 1.0f, 10.0f, 70.0f);
	} else {
		glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
	}
	
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
	case '9':
		perspective = !perspective;
		if (perspective) printf("Using gluPerspective\n");
		else printf("Using glOrtho\n");
		break;
	case '0':
		camera = !camera;
		if (camera) printf("Using camera\n");
		else printf("Moving -30.0 on z.\n");
		break;
	case '1':
		exercise = 1;
		break;
	case '2':
		exercise = 2;
		camera = 0;
		perspective = 1;
		break;
	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Transformaciones");
	
	
	initGL();
	init_scene();
	
	glutDisplayFunc(&window_display);
	
	glutReshapeFunc(&window_reshape);
	
	glutKeyboardFunc(&window_key);
	
	//function called on each frame
	glutIdleFunc(&window_idle);
	
	glutSpecialFunc(&callback_special);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);
	
	glutMainLoop();
	
	return 1;
}
