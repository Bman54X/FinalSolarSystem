#ifndef PLANET_H
#define PLANET_H

#include <string>
#include "vgl.h"
#include "vectors.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

using namespace std;
typedef vec4 point4;

class Planet {
public:
	string Name;
	float orbitalSpeed, orbitalRadius, orbitalPeriod;
	float Scale;
	float CurrentLocationOwnAxis, CurrentLocationInSystem;
	float PositionX, PositionY, PositionZ;
	char * texturePath;
	GLuint Texture;
	Planet * SatelliteOf;
	GLuint vao, program, location;

	const int NumTimesToSubdivide = 5;
	const int NumTriangles = 4096;
	const int NumVertices = 3 * NumTriangles;

	int Index = 0;
	point4 points[3 * 4096];
	vec3 normals[3 * 4096];
	GLfloat textureCoordinates[3 * 4096][2];
	GLfloat colorData[3 * 4096][3];
	GLuint Buffers[3];

	/*The perihelion is the point in the orbit of a celestial body where it is nearest to its orbital focus
	aphelion is the point in the orbit where the celestial body is farthest from its focus.
	orbital period is the time taken for a given object to make one complete orbit around another object*/
	Planet(string name, float radius, float period, char * path, float scale, 
		float r, float g, float b, GLuint p, Planet * sateliteOf = NULL);

	void Orbit(float degree, float scaleOwnAxisRotation);

	void Draw();

	void triangle(const point4& a, const point4& b, const point4& c);

	point4 unit(const point4& p);

	void divide_triangle(const point4& a, const point4& b, const point4& c, int count);

	void tetrahedron(int count);
};

#endif