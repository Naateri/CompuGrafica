#define GLUT_DISABLE_ATEXIT_HACK	
#include <math.h>
#include <GL/glut.h>
#include <iostream>

#define KEY_ESC 27
#define KEY_ONE 49
#define KEY_TWO 50
#define KEY_THREE 51
#define KEY_FOUR 52
#define KEY_FIVE 53

int display_exercise = 5;

void exercise_1(float center_x, float center_y, float edge_sz){
	glBegin(GL_LINE_STRIP);
	
	float half_sz = edge_sz/2;
	float botleft_x, botleft_y;
	botleft_x = center_x - half_sz;
	botleft_y = center_y - half_sz;
	
	glColor3d(255,0,0);
	
	glVertex2f(botleft_x, botleft_y); //bottom left
	glVertex2f(botleft_x + edge_sz, botleft_y); //bottom right
	glVertex2f(botleft_x + edge_sz, botleft_y + edge_sz); //top right
	glVertex2f(botleft_x, botleft_y + edge_sz); //top left
	glVertex2f(botleft_x, botleft_y); //bottom left
	
	glEnd();
}

void exercise_2(float center_x, float center_y, float radius){
	glBegin(GL_LINE_STRIP);
	glColor3d(0,255,0);
	
	float x_val, y_val;
	for(int i = 1; i <= 360; i++){
		x_val = cos(i * M_PI/180) * radius; //pythagoras
		y_val = sin(i * M_PI/180) * radius; //theorem
		glVertex2f(center_x + x_val, center_y + y_val);
	}
	
	glEnd();
}

void exercise_3(int circle_num, float radius_reduction, float radius){
	float reduction;
	exercise_2(0.0f, 0.0f, radius);
	glBegin(GL_LINE_STRIP);
	float orig_rad = radius;
	for(int i = 1; i < circle_num; i++){
		reduction = radius * radius_reduction / 100;
		radius = radius - reduction;
		exercise_2(radius - orig_rad, 0.0f, radius);
	}
	glEnd();
}

void exercise_4(int circle_num, float radius_reduction){
	glBegin(GL_LINE_STRIP);
	float old_radius = 10.0f, new_radius;
	float reduction, x, y;
	x = -25.0f; y = 0.0f;
	exercise_2(x, y, old_radius);
	
	for(int i = 1; i < circle_num; i++){
		reduction = old_radius * radius_reduction / 100;
		new_radius = old_radius - reduction;
		x = x + old_radius + new_radius;
		exercise_2(x, y, new_radius);
		old_radius = new_radius;
	}
}

void exercise_5(int circle_num, float radius_reduction, float angle){
	float old_radius = 10.0f, new_radius, reduction, x, y;
	x = -25.0f; y = -25.0f;
	exercise_2(x, y, old_radius);
	float cos_angle = cos(angle * M_PI / 180), sin_angle = sin(angle * M_PI / 180);
	for (int i = 1; i < circle_num; i++){
		reduction = old_radius * radius_reduction / 100;
		new_radius = old_radius - reduction;
		x += cos_angle * (old_radius + new_radius);
		y += sin_angle * (old_radius + new_radius);
		exercise_2(x, y, new_radius);
		old_radius = new_radius;
	}
	glEnd();
}

void idle(){
	glutPostRedisplay();
}

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINE_STRIP);
	if (display_exercise == 1){
		//glBegin(GL_LINE_STRIP);
		exercise_1(18.0f, -5.5f, 11.5f);
		//glEnd();
	}
	else if (display_exercise == 2){
		//glBegin(GL_LINE_STRIP);
		exercise_2(27.0f, -15.0f, 10.0f);
		//glEnd();
	}
	else if (display_exercise == 3) exercise_3(5, 30.0f, 30.0f);
	else if (display_exercise == 4) exercise_4(6, 30.0f);
	else if (display_exercise == 5) exercise_5(6, 20.0f, 30.0f);
	glEnd();
}

//
//funcion llamada a cada imagen
void glPaint(void) {
	
	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
		
	//dibuja el gizmo
	displayGizmo();
	//doble buffer, mantener esta instruccion al fin de la funcion
	glFlush();
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glClearDepth(1.0f);
	
	//modo projeccion 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f); 
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	case KEY_ONE:
		display_exercise = 1;
		break;
	case KEY_TWO:
		display_exercise = 2;
		break;
	case KEY_THREE:
		display_exercise = 3;
		break;
	case KEY_FOUR:
		display_exercise = 4;
		break;
	case KEY_FIVE:
		display_exercise = 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
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
	//glutIdleFunc(&idle);
	
	glutMainLoop(); //bucle de rendering
	
	return 0;
}
