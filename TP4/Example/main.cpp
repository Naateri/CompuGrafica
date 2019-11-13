#define GLUT_DISABLE_ATEXIT_HACK	
#include <math.h>
#include <iostream>


#include <GL/glut.h>
#include "TextureManager.h"
#define KEY_ESC 27

using namespace std;


GLint sprites;
GLint texture;
int _time = 0;
int timebase = 0;
int anim = 0;
int i = 0;
double x = 0.1678;
double y = 0.665;//factor de cambio de posicion en la textura
//para recuperar los sprites, no es una solucion muy precisa
//lo mejor seria de pre-calcular cual es la posicion del sprite en la textura
//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
	glEnable(GL_BLEND);//utilizar transparencia
	_time = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	int dt = _time -timebase;// delta time
	timebase = _time;
	anim += dt;//duracion de la animacion entre dos cambios de Sprite
	
	if (anim / 1000.0 > 0.15)// si el tiempo de animacion dura mas 0.15s cambiamos de sprite
	{
		i++;
		anim = 0.0;
	}
	
	if (i == 6) i = 0;

	glBindTexture(GL_TEXTURE_2D, sprites);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f + x*i, y);//coordenadas de textura
	glVertex3d(-3, -5, 0);

	glTexCoord2f(0.0f + x*i, 1.0f);
	glVertex3d(-3, 5, 0);

	glTexCoord2f(x*(i + 1.0), 1.0f);
	glVertex3d(3, 5, 0);

	glTexCoord2f(x*(i + 1.0), y);
	glVertex3d(3, -5, 0);
	glEnd();

	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);//coordenadas de textura
	glVertex3d(8, -2, 0);

	glTexCoord2f(0.0, 1.0f);
	glVertex3d(8, 2, 0);

	glTexCoord2f(1.0, 1.0f);
	glVertex3d(16, 2, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex3d(16, -2, 0);
	glEnd();


	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.4f, 0.4f, 0.4f, 0.2f); //(R, G, B, transparencia) en este caso un fondo negro
	glEnable(GL_TEXTURE_2D);
	//modo projeccion 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Get a handle for our "myTextureSampler" uniform
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 50.0f, width / 50.0f, -height / 50.0f, height / 50.0f, -1.0f, 1.0f);

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
//function called on each frame
GLvoid window_idle()
{


	glutPostRedisplay();
}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Textura: Animacion"); //titulo de la ventana

	init_GL(); //fucnion de inicializacion de OpenGL

	sprites = TextureManager::Inst()->LoadTexture("sprites.png", GL_BGRA, GL_RGBA);
	//GL_RGBA : en caso de una textura con transparencia sino GL_RGB
	//GL_BGRA_EXT o GL_BGR_EXT : utilizar en caso de inversion de colores sino GL_RGBA o GL_RGB
	texture = TextureManager::Inst()->LoadTexture("background.png", GL_BGR_EXT, GL_RGB);
	cout << sprites << endl;
	cout << texture << endl;
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutIdleFunc(&window_idle);
	glutMainLoop(); //bucle de rendering

	return 0;
}
