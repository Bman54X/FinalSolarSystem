///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////
#include "vectors.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "SOIL.h"
#include "LoadShaders.h"

using namespace std;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[3];
GLuint location;
GLuint location2;
GLuint location3;

GLuint texture[1];

float translate_value = 0;
float rotate_value = 0;
float camera_distance = 2.0;

const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices = 3 * NumTriangles;

typedef vec4 point4;
typedef vec4 color4;

point4 points[NumVertices];
vec3 normals[NumVertices];
GLfloat textureCoordinates[NumVertices][2];

// Model-view and projection matrices uniform location
GLuint ModelView, Projection;

int Index = 0;

/*void triangle(const point4& a, const point4& b, const point4& c) {
	vec3 normal = normalize(cross(b - a, c - b));

	normals[Index] = normal;  points[Index] = a;  Index++;
	normals[Index] = normal;  points[Index] = b;  Index++;
	normals[Index] = normal;  points[Index] = c;  Index++;
}*/

void triangle(const point4& a, const point4& b, const point4& c) {
	vec3 normal = normalize(cross(b - a, c - b));

	normals[Index] = normal;  points[Index] = a;
	
	textureCoordinates[Index][0] = 0.5f + (atan2(normals[Index].x, normals[Index].z) / (2 * 3.14f));
	textureCoordinates[Index][1] = 0.5f - (asin(normals[Index].y) / 3.14f);
	//textureCoordinates[Index][0] = 0;
	//textureCoordinates[Index][1] = 0;
	Index++;

	normals[Index] = normal;  points[Index] = b;
	textureCoordinates[Index][0] = 0.5f + (atan2(normals[Index].x, normals[Index].z) / (2 * 3.14f));
	textureCoordinates[Index][1] = 0.5f - (asin(normals[Index].y) / 3.14f);
	//textureCoordinates[Index][0] = 0.5f;
	//textureCoordinates[Index][1] = 1;
	Index++;

	normals[Index] = normal;  points[Index] = c;
	textureCoordinates[Index][0] = 0.5f + (atan2(normals[Index].x, normals[Index].z) / (2 * 3.14f));
	textureCoordinates[Index][1] = 0.5f - (asin(normals[Index].y) / 3.14f);
	//textureCoordinates[Index][0] = 1;
	//textureCoordinates[Index][1] = 0;
	Index++;
}

point4 unit(const point4& p) {
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void divide_triangle(const point4& a, const point4& b, const point4& c, int count) {
	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	} else {
		triangle(a, b, c);
	}
}

void tetrahedron(int count) {
	point4 v[4] = {
		vec4(0.0, 0.0, 1.0, 1.0),
		vec4(0.0, 0.942809, -0.333333, 1.0),
		vec4(-0.816497, -0.471405, -0.333333, 1.0),
		vec4(0.816497, -0.471405, -0.333333, 1.0)
	};

	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);
}

//---------------------------------------------------------------------
//
// init
//
void init(void) {
	tetrahedron(NumTimesToSubdivide);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);

	glGenTextures(1, texture);

	GLint width, height;
	unsigned char * image = SOIL_load_image("Earth.png", &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

	GLfloat colorData[NumVertices][3];
	for (int x = 0; x < NumVertices; x+=3) {
		colorData[x][0] = (float)((rand() % 101)) / 100;
		colorData[x][1] = (float)((rand() % 101)) / 100;
		colorData[x][2] = (float)((rand() % 101)) / 100;

		colorData[x+1][0] = (float)((rand() % 101)) / 100;
		colorData[x+1][1] = (float)((rand() % 101)) / 100;
		colorData[x+1][2] = (float)((rand() % 101)) / 100;

		colorData[x+2][0] = (float)((rand() % 101)) / 100;
		colorData[x+2][1] = (float)((rand() % 101)) / 100;
		colorData[x+2][2] = (float)((rand() % 101)) / 100;
	}
	
	glEnable(GL_DEPTH_TEST);
	glGenBuffers(2, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
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
	location2 = glGetUniformLocation(program, "camera_matrix");
	location3 = glGetUniformLocation(program, "projection_matrix");
}

//---------------------------------------------------------------------
//
// display
//
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Die 1 texture
	glm::mat4 model_view = glm::translate(glm::mat4(1.0), glm::vec3(translate_value, 0.0, 0.0));
	model_view = glm::rotate(model_view, rotate_value, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//Camera stuff
	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(0.0, 0.0, camera_distance), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(location2, 1, GL_FALSE, &camera_matrix[0][0]);
	glm::mat4 projection_matrix = glm::perspective(45.0f, 1024.0f / 1024.0f, 1.0f, 100.0f);
	glUniformMatrix4fv(location3, 1, GL_FALSE, &projection_matrix[0][0]);

	glFlush();
}

void idle() {
	//translate_value += 0.0001;
	//rotate_value += 0.0001f;
	//glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//
void keyboardHandler(unsigned char key, int x, int y) {
	if (key == 'a') {
		translate_value -= 0.1f;
	} else if (key == 'd') {
		translate_value += 0.1f;
	} else if (key == 'l') {
		rotate_value += 0.1f;
	} else if (key == 'k') {
		rotate_value -= 0.1f;
	} else if (key == 'f') {
		camera_distance += 0.1f;
	} else if (key == 'j') {
		camera_distance -= 0.1f;
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

	glutIdleFunc(idle);

	glutMainLoop();
}