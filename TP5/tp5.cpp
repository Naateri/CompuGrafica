#define GLUT_DISABLE_ATEXIT_HACK	
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <fstream>
#include <string>
#include <vector>
#include "TextureManager.h"

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
float delta_x = -49.0; 
float delta_y = 16.0;
float mouse_x, mouse_y;
float var_x = 310.0;
float var_z = -830.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

bool light = true;
bool normals = true;
bool texture = true;

typedef struct // Definicion de un punto del terreo
{
	GLfloat s, t; // Coordenadas de texturas
	GLfloat nx, ny, nz; // Coordenadas de la normal
	GLfloat x, height, z; // posición del vértice
} PuntoTerreno;

vector<GLfloat> TriangleNormal(PuntoTerreno A, PuntoTerreno B, PuntoTerreno C){
	GLfloat nx, ny, nz, ax, ay, az, bx, by, bz;
	
	ax = B.x - A.x;
	ay = B.height - A.height;
	az = B.z - A.z;
	
	bx = C.x - A.x;
	by = C.height - A.height;
	bz = C.z - A.z;
	
	nx = (ay * bz) - (by * az);
	ny = (bx * az) - (ax * bz);
	nz = (ax * by) - (bx * ay);
	
	GLfloat sum = sqrt(pow(nx,2) + pow(ny,2) + pow(nz,2));
	nx /= sum; ny /= sum; nz /= sum;
	
	vector<GLfloat> normal;
	normal.push_back(nx); normal.push_back(ny); normal.push_back(nz);
	
	return normal;
}

class Terreno{
private:
	int nb_pt_x, nb_pt_z; // quantidad de punto en X y Z
	float dist_x, dist_z; // distancia entre dos puntos segun X y Z
	PuntoTerreno *lista_puntos; // Tabla que contiene los puntos
	GLuint *lista_indices ; // Tabla que contiene los indicess
	void build_triangles();
public :
	Terreno(){
		lista_puntos = new PuntoTerreno[102*101];
		lista_indices = new GLuint[102*101*6];
		dist_x = 50.0f; 
		dist_z = 50.0f;
		nb_pt_x = 101;
		nb_pt_z = 101;
	}
	~Terreno();
	bool load(char *filename); // carga de un archivo de modelo
	void display(); // Visualizacion del terreno
	void computeNormals(); // calcula las normales de cada vertice
};

void Terreno::build_triangles(){
	int cur_index = 0;
	for(int i = 0; i < nb_pt_z-1; i++){
		for (int j = 0; j < nb_pt_x-1; j++){
			int position = i*nb_pt_x + j;
			//right
			/*lista_indices[cur_index] = i*nb_pt_x + (j+1);
			lista_indices[cur_index+1] = (i+1)*nb_pt_x + j;
			lista_indices[cur_index+2] = (i+1)*nb_pt_x + (j+1);*/
			lista_indices[cur_index] = position + 1;
			lista_indices[cur_index+1] = position + nb_pt_x;
			lista_indices[cur_index+2] = position + nb_pt_x + 1;
			cur_index += 3;
			
			//left
			/*lista_indices[cur_index] = i*nb_pt_x + j;
			lista_indices[cur_index+1] = i*nb_pt_x + (j+1);
			lista_indices[cur_index+2] = (i+1)*nb_pt_x + (j);*/
			lista_indices[cur_index] = position;
			lista_indices[cur_index+1] = position + 1;
			lista_indices[cur_index+2] = position + nb_pt_x;
			
			cur_index += 3;
		}
	}
	cout << "triangle vertices " << cur_index << endl;
}

void Terreno::computeNormals(){
	int cur_index = 0;
	PuntoTerreno cur_pt;
	PuntoTerreno TriA, TriB, TriC;
	vector<GLfloat> normals;
	
	GLfloat cur_nx, cur_ny, cur_nz;
	
	for(int i = 0; i < nb_pt_z; i++){
		for (int j = 0; j < nb_pt_x; j++){
			
			cur_nx = cur_ny = cur_nz = 0.0;
			
			cur_pt = lista_puntos[cur_index];
			normals.clear();
			if (j == 0){ //points at the left
				if (i == 0){  //top-left corner
					TriA = lista_puntos[0];
					TriB = lista_puntos[1];
					TriC = lista_puntos[nb_pt_x];
					
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_pt.nx = normals[0];
					cur_pt.ny = normals[1];
					cur_pt.nz = normals[2];
				} else if (i == nb_pt_z - 1){ //bottom-left corner
					//two triangles
					//above
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index - nb_pt_x];
					TriC = lista_puntos[cur_index - nb_pt_x + 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//right
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index + 1];
					TriC = lista_puntos[cur_index - nb_pt_x + 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					cur_nx = cur_nx / 2; cur_ny = cur_ny / 2; cur_nz = cur_nz / 2;
					
					cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
					
				} else { //left points, 3 triangles to consider
					//above-triangle
					TriA = lista_puntos[cur_index - nb_pt_x];
					TriB = lista_puntos[cur_index - nb_pt_x + 1];
					TriC = lista_puntos[cur_index];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//above-right-triangle
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index - nb_pt_x + 1];
					TriC = lista_puntos[cur_index + 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//below triangle
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index + 1];
					TriC = lista_puntos[cur_index + nb_pt_x];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					cur_nx = cur_nx / 3; cur_ny = cur_ny / 3; cur_nz = cur_nz / 3;
					
					cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
				}
			} else if (i == 0){ //first row
				if (j == nb_pt_x - 1){ //top right corner
					//two triangles
					//below
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index + nb_pt_x];
					TriC = lista_puntos[cur_index + nb_pt_x - 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//left
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index - 1];
					TriC = lista_puntos[cur_index + nb_pt_x - 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					cur_nx = cur_nx / 2; cur_ny = cur_ny / 2; cur_nz = cur_nz / 2;
					
					cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
				} else { //rest of first row
					//left triangle
					TriA = lista_puntos[cur_index]; //three triangles
					TriB = lista_puntos[cur_index - 1];
					TriC = lista_puntos[cur_index + nb_pt_x - 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//below
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index + nb_pt_x - 1];
					TriC = lista_puntos[cur_index + nb_pt_x];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//right
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index + 1];
					TriC = lista_puntos[cur_index + nb_pt_x];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					cur_nx = cur_nx / 3; cur_ny = cur_ny / 3; cur_nz = cur_nz / 3;
					
					cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
				}
			} else if (j == nb_pt_x - 1){ //right side
				if (i == nb_pt_z - 1){ //bottom-right corner
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index - 1];
					TriC = lista_puntos[cur_index - nb_pt_x];
					
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_pt.nx = normals[0];
					cur_pt.ny = normals[1];
					cur_pt.nz = normals[2];
				} else { //rest of right side
					//three triangles
					//above
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index - nb_pt_x];
					TriC = lista_puntos[cur_index - 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//left
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index - 1];
					TriC = lista_puntos[cur_index + nb_pt_x - 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					//below
					TriA = lista_puntos[cur_index];
					TriB = lista_puntos[cur_index + nb_pt_x];
					TriC = lista_puntos[cur_index + nb_pt_x - 1];
					normals = TriangleNormal(TriA, TriB, TriC);
					cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
					
					cur_nx = cur_nx / 3; cur_ny = cur_ny / 3; cur_nz = cur_nz / 3;
					
					cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
				}
			} else if (i == nb_pt_z - 1){ //last row
				//three triangles
				//left
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index - 1];
				TriC = lista_puntos[cur_index - nb_pt_x];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//above
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index - nb_pt_x];
				TriC = lista_puntos[cur_index - nb_pt_x + 1];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//right
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index + 1];
				TriC = lista_puntos[cur_index - nb_pt_x + 1];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				cur_nx = cur_nx / 3; cur_ny = cur_ny / 3; cur_nz = cur_nz / 3;
				
				cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
			} else { //regular case: 6 triangles to consider
				//left-above triangle
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index - 1];
				TriC = lista_puntos[cur_index - nb_pt_x];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//above triangle
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index - nb_pt_x];
				TriC = lista_puntos[cur_index - nb_pt_x + 1];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//right-above triangle
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index - nb_pt_x + 1];
				TriC = lista_puntos[cur_index + 1];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//right-below Triangle
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index + 1];
				TriC = lista_puntos[cur_index + nb_pt_x];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//below Triangle
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index + nb_pt_x];
				TriC = lista_puntos[cur_index + nb_pt_x - 1];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				//left-below triangle 
				TriA = lista_puntos[cur_index];
				TriB = lista_puntos[cur_index - 1];
				TriC = lista_puntos[cur_index + nb_pt_x - 1];
				normals = TriangleNormal(TriA, TriB, TriC);
				cur_nx += normals[0]; cur_ny += normals[1]; cur_nz += normals[2];
				
				cur_nx = cur_nx / 6; cur_ny = cur_ny / 6; cur_nz = cur_nz / 6;
				
				cur_pt.nx = cur_nx; cur_pt.ny = cur_ny; cur_pt.nz = cur_nz;
			}
			
			lista_puntos[cur_index] = cur_pt;
			
			cur_index++;
		}
		
	}
	cout << "cur_index: " << cur_index << endl;
}

bool Terreno::load(char* filename){
	ifstream myfile (filename);
	string line, cur_num;
	PuntoTerreno* temp;
	
	int x = 0, z = 0;
	
	float start_x = 0.0f, start_z = 0.0f;
	float height;
	
	if (myfile.is_open()){
		int cur_line = 0;
		while ( getline (myfile,line) ){
			
			string last_cur_num;
			if (cur_line++ == 0) continue;
			//cout << line << endl;
			start_x = 0.0f;
			x = 0;
			
			cur_num.clear();
			
			for(int i = 0; i <= line.size(); i++){
				if (line[i] == ' '){
					temp = new PuntoTerreno;
					height = stof(cur_num);
					temp->x = start_x;
					temp->height = height;
					temp->z = start_z;
					
					temp->t = (float)x/101.0f;
					temp->s = (float)z/101.0f;
					
					lista_puntos[cur_line-1] = *temp;
					
					//last_cur_num = cur_num;
					
					cur_num.clear();
					cur_line++;
			
					start_x += 50.0f;
					x++;
				} else {
					cur_num += line[i];
				}
			}
			
			start_z += dist_z;
			z++;
		}
		cout << "total points: " << cur_line-1 << endl;
		
		build_triangles();
		cout << "Triangles built\n";
		
	} else {
		return false;
	}
}

void Terreno::display(){
	
	GLfloat ambient[] = {0.0f, 0.1f, 0.0f, 1.0f};
	GLfloat difuse[] = {0.0f, 0.5f, 0.0f, 1.0f};
	GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat emission[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat shininess = 50.0f;
	
	glEnable(GL_COLOR_MATERIAL);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	
	int count = 101*101*6;
	
	glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(PuntoTerreno), &lista_puntos[0].s);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)lista_indices);
}


Terreno* terreno;
GLuint terrain_texture;

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 20.0;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 20.0;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 20.0;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 20.0;
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
GLvoid callback_motion(int x, int y){
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
	glutCreateWindow("TP 5");
	
	initGL();
	init_scene();

	terreno = new Terreno;
	if (texture) terrain_texture = TextureManager::Inst()->LoadTexture("fontvieille.tga", GL_BGR_EXT, GL_RGB);
	terreno->load("fontvieille.txt");
	if (normals) terreno->computeNormals();
	
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

	GLfloat position[] = { 2500.0f, 300.0f, 2500.0f, 0.0 };
	GLfloat light0Ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat light0Diffuse[] = {5.0f, 5.0f, 5.0f, 1.0f};
	GLfloat light0Specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	
	if (light){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, 1.0f, 0.01f, 10000.0f);
	gluLookAt(0.0f, 0.0f, 7500.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);
	
	terreno->display();

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





