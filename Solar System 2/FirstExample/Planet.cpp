#include <fstream>
#include <iostream>
#include "Planet.h"


Planet::Planet(string name, float radius, float period, char * path, float scale, float inclination,
	float r, float g, float b, GLuint p, Planet * satelliteOf) {
	CurrentLocationInSystem = 0;
	Name = name;
	orbitalRadius = radius;
	orbitalPeriod = period;
	texturePath = path;
	Scale = scale;
	SatelliteOf = satelliteOf;
	InclinationZ = inclination;
	InclinationY = rand() % 180;
	program = p;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	tetrahedron(NumTimesToSubdivide);

	for (int x = 0; x < NumVertices; x++) {
		colorData[x][0] = r;
		colorData[x][1] = g;
		colorData[x][2] = b;
	}

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

	location = glGetUniformLocation(program, "model_matrix");
}

void Planet::Orbit(float degree, float scaleOwnAxisRotation) {
	CurrentLocationOwnAxis += degree * scaleOwnAxisRotation;
	CurrentLocationInSystem += degree / (orbitalPeriod == 0 ? 1 : orbitalPeriod);

	CurrentLocationInSystem = CurrentLocationInSystem > 360 ? CurrentLocationInSystem - 360 : CurrentLocationInSystem;
	CurrentLocationOwnAxis = CurrentLocationOwnAxis > 360 ? CurrentLocationOwnAxis - 360 : CurrentLocationOwnAxis;
}

void Planet::Draw() {
	const float pi = 3.1415f;
	const float degreeToRadian = 57.29577951f;
	float translationInSystem = 0.05f * (orbitalRadius * 2);
	glm::mat4 model_view;

	PositionX = -cos((CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystem;
	PositionY = 0;
	PositionZ = sin((CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystem;

	if (SatelliteOf != NULL) {
		model_view = glm::translate(glm::mat4(1.0), glm::vec3(SatelliteOf->PositionX, SatelliteOf->PositionY, SatelliteOf->PositionZ));
		model_view = glm::translate(model_view, glm::vec3(PositionX, PositionY, PositionZ));
		//glRotated(InclinationY, 0, 1, 0);
		//glRotated(InclinationZ, 1, 0, 0);
	} else {
		model_view = glm::translate(glm::mat4(1.0), glm::vec3(PositionX, PositionY, PositionZ));
	}
	
	model_view = glm::rotate(model_view, CurrentLocationOwnAxis, glm::vec3(0.0f, 0.0f, 1.0f));
	model_view = glm::scale(model_view, glm::vec3(Scale, Scale, Scale));

	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	/*GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	const float pi = 3.1415f;
	const float degreeToRadian = 57.29577951f;
	float translationInSystem = 0.05f * (orbitalRadius * 2);

	PositionX = -cos((CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystem;
	PositionY = 0;
	PositionZ = sin((CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystem;

	if (Name == "Sun") {
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}

	if (SateliteOf != NULL) {
		glPushMatrix();
		glTranslated(SateliteOf->PositionX, SateliteOf->PositionY, SateliteOf->PositionZ);
	}

	glPushMatrix();
	if (SateliteOf != NULL) {
		glRotated(InclinationY, 0, 1, 0);
		glRotated(InclinationZ, 1, 0, 0);
	}
	glTranslated(PositionX, PositionY, PositionZ);

	GLUquadric * qobj = gluNewQuadric();
	gluQuadricTexture(qobj, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotated(CurrentLocationOwnAxis, 0, 0, 1);
	gluSphere(qobj, Scale / 3, 30, 30);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(qobj);

	if (Name == "Sun") {
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	if (SateliteOf != NULL) {
		glPopMatrix();
	}*/
}

void Planet::triangle(const point4& a, const point4& b, const point4& c) {
	vec3 normal = normalize(cross(b - a, c - b));

	normals[Index] = normal;  points[Index] = a;  Index++;
	normals[Index] = normal;  points[Index] = b;  Index++;
	normals[Index] = normal;  points[Index] = c;  Index++;
}

/*void Planet::triangle(const point4& a, const point4& b, const point4& c) {
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
}*/

point4 Planet::unit(const point4& p) {
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void Planet::divide_triangle(const point4& a, const point4& b, const point4& c, int count) {
	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}

void Planet::tetrahedron(int count) {
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