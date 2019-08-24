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
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

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

	glutMainLoop();

	return 1;
}



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

float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
float spin_rate = 1.0f;
float x = -8.0f, y = -8.0f, z = -8.0f;
bool r = true, l = false, r_y = true, l_y = false, r_z = true, l_z = false;

void move_on_x(float limit){
	if (r){
		x += 0.1f;
		if (x >= limit){
			r = false; l = true;
		}
	} else {
		x -= 0.1f;
		if (x <= -limit){
			r = true; l = false;
		}
	}
}

void move_on_y(float limit){
	if (r_y){
		y += 0.1f;
		if (y >= limit){
			r_y = false; l_y = true;
		}
	} else {
		y -= 0.1f;
		if (y <= -limit){
			r_y = true; l_y = false;
		}
	}
}

void move_on_z(float limit){
	if (r_z){
		z += 0.1f;
		if (z >= limit){
			r_z = false; l_z = true;
		}
	} else {
		z -= 0.1f;
		if (z <= -limit){
			r_z = true; l_z = false;
		}
	}
}

void exercise_1(){
	glColor3f(0.0f, 1.0f, 0.0f);
	glRotatef(a, 0.0f, 0.0f, 1.0f);
	glTranslatef(4.0f, 4.0f, 4.0f);
	glutSolidTeapot(5.0f);
	a += spin_rate;
}

void exercise_2(){
	glColor3f(0.0f, 0.0f, 1.0f);
	glPushMatrix();
		glTranslatef(x, 0.0f, 0.0f);
		glutSolidSphere(0.5f, 8, 8);
	glPopMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
		glRotatef(a, 0.0f, 0.0f, 1.0f);
		glTranslatef(x + 4.0f, 4.0f, 4.0f);
		glutSolidTeapot(2.0f);
	glPopMatrix();
	a += spin_rate;
	move_on_x(8.0f);
}

void exercise_3(){
	exercise_2();
	glPopMatrix();
	glPushMatrix();
	//torus
	glRotatef(b, 0.0f, 1.0f, 0.0f);
	glTranslatef(x, 3.0f, 3.0f);
	glutSolidTorus(0.5f, 2.0f, 10, 100);
	b += 3 * spin_rate;
	move_on_x(8.0f);
	
	glPopMatrix();
	//cube
	glRotatef(c, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -5.0f, 0.0f);
	glutSolidCube(2.0f);
	c += 1.5f;
	
}

float Vt = 1.0f, VS = 2.0f;
float Vl = 2.0f*Vt;
float VT = 3.0f*VS, VL = 1.5f * VS, VM = VS;

void exercise_4(){
	//sun
	glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
		glRotatef(a, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(4.0f, 8, 8);
	glPopMatrix();
	a += VS;
	
	//earth
	glColor3f(0.0f, 0.0f, 1.0f);
	glPushMatrix();
		glTranslatef(x, y, 10.0f);
		glRotatef(b, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(2.0f, 8, 8);
	glPopMatrix();
	move_on_x(10.0f); move_on_y(10.0f); move_on_z(10.0f);
	b += VT;
	
	//moon
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glTranslatef(x + 2.5f, y + 2.5f, 15.0f);
		glRotatef(c, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.67f, 8, 8);
	glPopMatrix();
	c += VL;
	//mars
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
		glTranslatef(18.0f, 18.0f, 18.0f);
		glRotatef(d, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(1.5f, 8, 8);
	glPopMatrix();
	d += VM;
}

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	/*dibujar aqui*/
	
	/*glRotatef(a, 0.0f, 1.0f, 0.0f);
	glutSolidTeapot(5.0f);
	a += 2.0f;*/
	
	//exercise_1();
	exercise_2();
	//exercise_3();
	//exercise_4();

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

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
