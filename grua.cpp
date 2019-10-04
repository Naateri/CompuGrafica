#define GLUT_DISABLE_ATEXIT_HACK	
#include <math.h>
#include <GL/glut.h>
#include <iostream>

#define KEY_ESC 27

int a = 0, b = 0, rope = 40;

void drawRectangle(float width, float height){
	glBegin(GL_QUADS);
	glVertex2f(0, -height / 2.0f);
	glVertex2f(width, -height / 2.0f);
	glVertex2f(width, height / 2.0f);
	glVertex2f(0, height / 2.0f);
	glEnd();
}

void draw_crane(int angle1, int angle2, int length){
	glBegin(GL_QUADS);
	
	glColor3d(255,0,0);
	glTranslatef(0.0f, 0.0f, 0.0f);
	drawRectangle(40.0f, 30.0f); //base rectangle
	
	glColor3d(255, 255, 0);
	glTranslatef(30.0f, 15.0f, 0.0f);
	glRotatef(angle1, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	drawRectangle(100.0f, 20.0f); //A rectangle
	
	glColor3d(0,255,0);
	glTranslatef(90.0f, 10.0f, 0.0f);
	glRotatef(angle2, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	drawRectangle(80.0f, 20.0f); //B rectangle
	
	//rope
	glColor3d(255, 255, 255);
	glTranslatef(90.0f, -10.0f, 0.0f);
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	drawRectangle(3.0f, (float)length);
	
	//box
	glColor3d(255,0,0);
	glTranslatef(-3.0f, -(float)length+15.0f, 0.0f);
	drawRectangle(10.0f, 10.0f);
	
	glEnd();
}

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_QUADS);
	
	glColor3d(0,0,0);
	drawRectangle(600.0f, 600.0f);
	draw_crane(a,b,rope);
	
	glEnd();
}

void idle(){
	glutPostRedisplay();
}

//
//funcion llamada a cada imagen
void glPaint(void) {
	
	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();
		
	//dibuja el gizmo
	displayGizmo();
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	
	//modo projeccion 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0.0f,  300.0f,0.0f, 300.0f, -1.0f, 1.0f); 
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	case 'w':
		a += 1;
		break;
	case 's':
		a -= 1;
		break;
	case 'q':
		a += 2;
		break;
	case 'a':
		a -= 2;
		break;
	case 'e':
		b += 1;
		break;
	case 'd':
		b -= 1;
		break;
	case 'r':
		b += 2;
		break;
	case 'f':
		b -= 2;
		break;
	case '+':
		rope += 1;
		break;
	case '-':
		rope -= 1;
		break;
	default:
		break;
	}
	
}
//
//el programa principal
//
int main(int argc, char** argv) {
	
	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Mi gruita"); //titulo de la ventana
	
	init_GL(); //funcion de inicializacion de OpenGL
	
	glutDisplayFunc(glPaint); 
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutIdleFunc(&idle);
	
	glutMainLoop(); //bucle de rendering
	
	return 0;
}
