///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////
using namespace std;

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint location;

const GLuint NumVertices = 9;
GLuint texture[1];

float translate_value = 0;
float rotate_value = 0;

//---------------------------------------------------------------------
//
// init
//
void init(void) {
	GLfloat textureData[] = {
		1.0f, 0.0f, 0.0f, // red
		1.0f, 1.0f, 1.0f, // white
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
	};

	GLfloat textureCoordinates[16][2] = {
		0.0f, 0.0f,
		2.0f, 0.0f,
		2.0f, 2.0f,
		0.0f, 2.0f,

		0.0f, 0.0f,
		2.0f, 0.0f,
		2.0f, 2.0f,
		0.0f, 2.0f,

		0.0f, 0.0f,
		2.0f, 0.0f,
		2.0f, 2.0f,
		0.0f, 2.0f,

		0.0f, 0.0f,
		2.0f, 0.0f,
		2.0f, 2.0f,
		0.0f, 2.0f,
	};

	glGenTextures(1, texture);

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, textureData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	GLfloat vertices[NumVertices][3] = {
		//Face 1
		{ 0.0f, 0.7f, 0.0f },
		{ -0.7f, -0.7f, 0.7f },
		{ 0.7f, -0.7f, 0.7f },
		//Face 2
		{ 0.0f, 0.7f, 0.0f },
		{ -0.7f, -0.7f, 0.7f },
		{ 0.0f, -0.7f, -0.7f },
		//Face 3
		{ 0.0f, 0.7f, 0.0f },
		{ 0.0f, -0.7f, -0.7f },
		{ 0.7f, -0.7f, 0.7f }
	};

	GLfloat colorData[NumVertices][3] = {
		//Red Face 1
		{ 1,1,1 },
		{ 1,1,1 },
		{ 1,1,1 },
		//Green Face 2
		{ 1,1,1 },
		{ 1,1,1 },
		{ 1,1,1 },
		//Blue Face 3
		{ 1,1,1 },
		{ 1,1,1 },
		{ 1,1,1 }
	};

	glEnable(GL_DEPTH_TEST);

	glGenBuffers(3, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vertexColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glBindAttribLocation(program, 2, "vTexCoord");
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);

	location = glGetUniformLocation(program, "model_matrix");
}

//---------------------------------------------------------------------
//
// display
//
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 model_view = glm::translate(glm::mat4(1.0), glm::vec3(translate_value, 0.0, 0.0));

	model_view = glm::rotate(model_view, rotate_value, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a': 		
		rotate_value += 0.02f;
		break;
	case 'd': 
		rotate_value -= 0.02f;
		break;
	}
}

void idle() {
	//translate_value += 0.0001;
	glutKeyboardFunc(keyboard);
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
}