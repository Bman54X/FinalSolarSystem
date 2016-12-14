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
Planet * centerPlanet;

float stopWatch = 0.0f, zoom = 20.0f;
float dragX = -1, dragY = -1;
float mouseX, mouseY = 90;
const float halfpi = 3.1415f / 180;

void initPlanets(GLuint program) {
	planets = new List<Planet*>();

	planets->Add(new Planet("Sun", 0.0f, 0.0f, "Sun.bmp", 4.0f, 1.0f, 1.0f, 1.0f, 0.0f, program));
	planets->Add(new Planet("Mercury", 100, 0.240f, "Mercury.bmp", 0.38f, 1.0f, 0.827f, 0.827f, 0.827f, program));
	planets->Add(new Planet("Venus", 200, 0.615f, "Venus.bmp", 0.94f, 1.0f, 1.0f, 0.647f, 0.0f, program));
	planets->Add(new Planet("Earth", 300, 1.0f, "Earth.bmp", 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, program));
	planets->Add(new Planet("Mars", 400, 1.880f, "Mars.bmp", 0.53f, 1.0f, 1.0f, 0.0f, 0.0f, program));
	planets->Add(new Planet("Jupiter", 700, -11.861, "Jupiter.bmp", 3.0f, 1.0f, 1.0f, 0.533f, 0.342f, program));
	planets->Add(new Planet("Saturn", 1000, 29.457, "Saturn.bmp", 2.5f, 1.0f, 0.961f, 0.961f, 0.863f, program));
	planets->Add(new Planet("Uranus", 1300, -84.016, "Uranus.bmp", 2.0f, 1.0f, 0.0f, 1.0f, 0.702f, program));
	planets->Add(new Planet("Neptune", 1700, 164.8, "Neptune.bmp", 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, program));
	planets->Add(new Planet("Pluto", 2200, 247.68, "Pluto.bmp", 0.3f, 1.0f, 0.314f, 0.314f, 0.314f, program));

	planets->Add(new Planet("TheMoon", 20, 0.07f, "Pluto.bmp", 0.18f, 15.0f, 0.827f, 0.827f, 0.827f, program, planets->Get(3)));
	planets->Add(new Planet("Phobos", 20, 0.06f, "Pluto.bmp", 0.08f, (float)(rand() % 180), 0.827f, 0.827f, 0.827f, program, planets->Get(4)));
	planets->Add(new Planet("Deimos", 30, 0.05f, "Pluto.bmp", 0.1f, (float)(rand() % 180), 0.827f, 0.827f, 0.827f, program, planets->Get(4)));

	centerPlanet = planets->Get(0);
}

void JumpToPlanet(int index) {
	Planet * planet;
	planet = planets->Get(index);
	centerPlanet = planet;
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
	glUseProgram(program);
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

	//Camera stuff
	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(centerPlanet->PositionX + x, centerPlanet->PositionY + y, centerPlanet->PositionZ + z), 
		glm::vec3(centerPlanet->PositionX, centerPlanet->PositionY, centerPlanet->PositionZ), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(location2, 1, GL_FALSE, &camera_matrix[0][0]);
	glm::mat4 projection_matrix = glm::perspective(45.0f, 1.0f, 1.0f, 1500.0f);
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

void keyboardHandler(unsigned char key, int A, int B) {
	switch (key) {
		case 'z': zoom++; break;
		case 'x': zoom--; break;
		case '0': JumpToPlanet(0); break;
		case '1': JumpToPlanet(1); break;
		case '2': JumpToPlanet(2); break;
		case '3': JumpToPlanet(3); break;
		case '4': JumpToPlanet(4); break;
		case '5': JumpToPlanet(5); break;
		case '6': JumpToPlanet(6); break;
		case '7': JumpToPlanet(7); break;
		case '8': JumpToPlanet(8); break;
		case '9': JumpToPlanet(9); break;
		default:break;
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