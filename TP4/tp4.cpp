#define GLUT_DISABLE_ATEXIT_HACK	
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <FreeImage.h>
//#include "Example/TextureManager.h"


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
float shininess = 50.0f;
int id = 1;

GLuint wall_texture, wall_texture2;

GLuint textures[7];// = {0,0,0,0,0,0,0,0,0,0,0};

//wall_1, wall_2, triangle_1, roof_1, floor, tronco, hojas

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

GLuint LoadTexture(const char* filename, GLenum image_format, GLint internal_format, GLint level = 0, GLint border = 0)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint textureID;
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;
	
	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;
	
	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &textureID);
	
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, bits);
	
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
	//glGenerateMipmap(GL_TEXTURE_2D);
	FreeImage_Unload(dib);
	
	//return success
	return textureID;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 4 : Transformations 3D Part 2");

	initGL();
	
	wall_texture = LoadTexture("bricks.jpeg", GL_RGB, GL_RGB);
	textures[0] = wall_texture;
	
	//cur_texture = LoadTexture("woody_m8.png", GL_RGBA, GL_RGBA);
	wall_texture2 = LoadTexture("lickitung.jpeg", GL_RGB, GL_RGB);
	textures[1] = wall_texture2;
	
	textures[4] = LoadTexture("grass.jpeg", GL_RGB, GL_RGB);
	
	cout << textures[0] << endl;
	cout << textures[1] << endl;

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
	GLfloat position[] = { -10.0f, 10.0f, -5.0f, 0.0 };
	GLfloat light0Ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat light0Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light0Specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//glDisable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	//glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	
	//glClearColor(RED, GREEN, BLUE, ALPHA);
	glMatrixMode(GL_PROJECTION);
	glClearColor(0.4f, 0.4f, 0.4f, 0.2f); //(R, G, B,
	glLoadIdentity();
}

void draw_floor(){
	
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_POLYGON);
	//glColor3f(0.0f, 1.0f, 0.0f);
	
	GLfloat floorAmbient[] = {0.0f, 0.3f, 0.0f, 1.0f};
	GLfloat floorDifuse[] = {0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat floorSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat emission[] = {0.0f, 1.0f, 0.0f, 1.0f};
	
	glPushMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, floorDifuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floorAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-20.0f, 0.0f, -20.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-20.0f, 0.0f, 20.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(20.0f, 0.0f, 20.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(20.0f, 0.0f, -20.0f);
	
	/*glVertex3f(-20.0f, 0.0f, -20.0f);
	glVertex3f(-20.0f, 0.0f, 20.0f);
	glVertex3f(20.0f, 0.0f, 20.0f);
	glVertex3f(20.0f, 0.0f, -20.0f);*/
	
	glPopMatrix();
	
	glEnd();
}

void draw_roof(){
	//glColor3f(1.0f, 0.0f, 0.0f);
	
	GLfloat roofAmbient0[] = {0.3f, 0.0f, 0.0f, 1.0f};
	GLfloat roofDifuse0[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat roofSpecular0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat materialEmission[] = {1.0f,1.0f,0, 1.0f};
	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	//glPushMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roofDifuse0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roofAmbient0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roofSpecular0);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);
	
	
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
	
	//glPopMatrix();
	
	GLfloat roofAmbient[] = {0.3f, 0.0f, 0.0f, 0.0f};
	GLfloat roofDifuse[] = {0.5f, 0.5f, 0.5f, 0.0f};
	GLfloat roofSpecular[] = {1.0f, 1.0f, 1.0f, 0.0f};
	
	//glPushMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roofAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, roofDifuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, roofSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, roofDifuse);
	
	//glColor3f(0.5f,0.5f,0.5f);
	
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
	
	//glPopMatrix();
}

void draw_wall(float x1, float y1, float x2, float y2, float z, int axis){
	glBegin(GL_POLYGON);
	//glColor3f(1.0f, 0.0f, 0.0f);
	
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

void draw_house(){
	
	GLfloat wallAmbient[] = {0.3f, 0.0f, 0.0f, 1.0f};
	GLfloat wallDifuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat wallSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wallDifuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wallAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wallSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	//draw_wall(-7.5f, 0.0f, 0.0f, 5.0f, -5.0f, 2);
	
	//texture
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	//draw_wall(-7.5f, 0.0f, 0.0f, 5.0f, -5.0f, 2);
	//(-7.5, 0.0), (-7.5, 5.0), (0.0, 0.0), (0.0, 5.0), z = -5
	glTexCoord2f(0.0f, 0.0f);//coordenadas de textura
	glVertex3f(-7.5f, 0.0f, -5.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-7.5f, 5.0f, -5.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 5.0f, -5.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -5.0f);
	glEnd();
	
	//draw_wall(-7.5f, 0.0f, 0.0f, 5.0f, 0.0f, 2);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	//(-7.5, 0.0), (-7.5, 5.0), (0.0, 0.0), (0.0, 5.0), z = 0
	glTexCoord2f(0.0f, 0.0f);//coordenadas de textura
	glVertex3f(-7.5f, 0.0f, 0.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-7.5f, 5.0f, 0.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 5.0f, 0.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	
	//draw_wall(0.0f, -5.0f, 5.0f, 0.0f, -7.5f, 0);
	
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	//(0.0, 5.0), (0.0, 0.0), (-5.0, 5.0), (-5.0, 0.0), x = -7.5
	glTexCoord2f(0.0f, 0.0f);//coordenadas de textura
	glVertex3f(-7.5f, 5.0f, 0.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-7.5f, 5.0f, -5.0f);
	
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-7.5f, 0.0f, -5.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-7.5f, 0.0f, 0.0f);
	
	glEnd();
	
	draw_wall(0.0f, -5.0f, 5.0f, 0.0f, 0.0f, 0);
	draw_roof();
}

void draw_tree(){
	//glColor3f(0.5f, 0.21f, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	
	GLfloat troncoAmbient[] = {0.65f, 0.3f, 0.0f, 1.0f};
	GLfloat troncoDifuse[] = {0.5f, 0.21f, 0.0f, 1.0f};
	GLfloat troncoSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, troncoAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, troncoDifuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, troncoSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	
	GLUquadricObj *quadratic, *quadratic2;
	quadratic = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(7.5f, -5.0f, 0.0f);
	gluCylinder(quadratic,1.0f,1.0f,10.0f,32,32);
	
	
	GLfloat leafAmbient[] = {0.0f, 0.3f, 0.0f, 1.0f};
	GLfloat leafDifuse[] = {0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat leafSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, leafAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, leafDifuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, leafSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	//glColor3f(0.0f, 1.0f, 0.0f);
	quadratic2 = gluNewQuadric();
	glTranslatef(0.0f, 0.0f, 10.0f);
	gluSphere(quadratic2, 3.0f, 32, 32);
	glPopMatrix();
}

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
	//glEnable(GL_BLEND);//utilizar transparencia

	gluPerspective(45.0f, 1.0f, 0.01f, 100.0f);

	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);
	
	//Textures
	//cout << "textures[0] " << textures[0] << endl;

	//Gizmo3D();
	draw_floor();
	draw_house();
	draw_tree();
	
	glutSwapBuffers();

	//glFlush();
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


