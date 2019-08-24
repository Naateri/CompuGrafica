#define GLUT_DISABLE_ATEXIT_HACK	
#include <math.h>
#include <GL/glut.h>
#include <iostream>

#define KEY_ESC 27


void drawRectangle(float height, float width){
	glBegin(GL_QUADS);
	glVertex2f(0, -height / 2.0f);
	glVertex2f(width, -height / 2.0f);
	glVertex2f(width, height / 2.0f);
	glVertex2f(0, height / 2.0f);
	glEnd();
}

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINE_STRIP);
	drawRectangle(15.0f, 20.0f);
	glTranslatef(20.0f, 10.0f, 0.0f);
	drawRectangle(10.0f, 50.0f);
	glTranslatef(50.0f, 10.0f, 0.0f);
	drawRectangle(10.0f, 40.0f);
	glEnd();
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
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana
	
	init_GL(); //funcion de inicializacion de OpenGL
	
	glutDisplayFunc(glPaint); 
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	
	glutMainLoop(); //bucle de rendering
	
	return 0;
}
