///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////
#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "vectors.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "SOIL.h"
#include "LoadShaders.h"
#include "Planet.h"
#include "List.h"
#include <ctime>

using namespace std;

GLuint location2;
GLuint location3;

GLuint texture[1];

List<Planet*> * planets;

float stopWatch = 0.0f, zoom = 2.0f;
float dragX = -1, dragY = -1;
float mouseX, mouseY = 90;
const float halfpi = 3.1415f / 180;

void initPlanets(GLuint program) {
	planets = new List<Planet*>();

	planets->Add(new Planet("Sun", 0.0f, 0.0f, "Sun.bmp", 1.0f, 3.63f, 1.0f, 1.0f, 0.0f, program));
	planets->Add(new Planet("Mercury", 50, 0.240f, "Mercury.bmp", 0.38f, 1.0f, 0.827f, 0.827f, 0.827f, program));
	planets->Add(new Planet("Venus", 100, 0.615f, "Venus.bmp", 0.94f, 1.0f, 1.0f, 0.647f, 0.0f, program));
	planets->Add(new Planet("Earth", 160, 1.0f, "Earth.bmp", 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, program));
	planets->Add(new Planet("Mars", 220, 1.880f, "Mars.bmp", 0.53f, 1.0f, 1.0f, 0.0f, 0.0f, program));
	/*planets->Add(new Planet("TheMoon", 10, 0.07f, "Pluto.bmp", 0.18f, 15.0f, planets->Get(3)));
	planets->Add(new Planet("Phobos", 10, 0.06f, "Pluto.bmp", 0.1f, (float)(rand() % 180), planets->Get(4)));
	planets->Add(new Planet("Deimos", 12, 0.05f, "Pluto.bmp", 0.08f, (float)(rand() % 180), planets->Get(4)));*/
}

//---------------------------------------------------------------------
//
// init
//
void init(void) {
	/*glGenTextures(1, texture);

	GLint width, height;
	unsigned char * image = SOIL_load_image("Earth.png", &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up
	initPlanets(program);

	glEnable(GL_DEPTH_TEST);

	/*glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glBindAttribLocation(program, 2, "vTexCoord");
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);*/

	location2 = glGetUniformLocation(program, "camera_matrix");
	location3 = glGetUniformLocation(program, "projection_matrix");
}

//---------------------------------------------------------------------
//
// display
//
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float x, y, z;
	x = sin((-mouseX + dragX) * halfpi) * cos((mouseY - dragY) * halfpi) * zoom;
	y = sin((mouseY - dragY) * halfpi) * zoom;
	z = cos((-mouseX + dragX) * halfpi) * cos((mouseY - dragY) * halfpi) * zoom;

	/*gluLookAt(planetHover->PositionX + x, planetHover->PositionY + y, planetHover->PositionZ + z,
		planetHover->PositionX, planetHover->PositionY, planetHover->PositionZ, 0.0, 1.0, 0.0);*/

	//gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//Camera stuff
	//glm::mat4 camera_matrix = glm::lookAt(glm::vec3(0.0, 0.0, camera_distance), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(location2, 1, GL_FALSE, &camera_matrix[0][0]);
	//glm::mat4 projection_matrix = glm::perspective(45.0f, 1024.0f / 1024.0f, 1.0f, 100.0f);
	glm::mat4 projection_matrix = glm::perspective(40.0f, 1.0f, 1.0f, 1500.0f);
	glUniformMatrix4fv(location3, 1, GL_FALSE, &projection_matrix[0][0]);

	for (int i = 0; i < planets->Length(); i++) {
		planets->Get(i)->Draw();
	}

	glFlush();
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
	case 0:
		if (state == 0.0f) {
			if (dragX == -1.0f) {
				dragX = (float)x;
				dragY = (float)y;
			}
			else {
				dragX += (float)x - mouseX;
				dragY += (float)y - mouseY;
			}
			mouseX = (float)x;
			mouseY = (float)y;
		}
		break;
	case 1:
		break;
	default:
		break;
	}
}

void mouseMove(int x, int y) {
	mouseX = (float)x;
	mouseY = (float)y;
}

void orbit() {
	long newStopWatch = clock();

	for (int i = 0; i < planets->Length(); i++) {
		planets->Get(i)->Orbit(0.1f * ((newStopWatch - stopWatch) / 20.0f), 30.0f);
	}

	stopWatch = (float)newStopWatch;

	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//
void keyboardHandler(unsigned char key, int x, int y) {
	/*if (key == 'a') {
		translate_value -= 0.1f;
	} else if (key == 'd') {
		translate_value += 0.1f;
	} else if (key == 'l') {
		rotate_value += 0.1f;
	} else if (key == 'k') {
		rotate_value -= 0.1f;
	} else */
	if (key == 'f') {
		zoom += 0.1f;
	} else if (key == 'j') {
		zoom -= 0.1f;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardHandler);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouse);
	glutIdleFunc(orbit);

	glutMainLoop();
}

#endif