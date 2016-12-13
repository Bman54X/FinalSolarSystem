#include "vgl.h"
#include <math.h>

const GLfloat DivideByZeroTolerance = GLfloat(1.0e-07);

struct vec4 {
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
	GLfloat  w;

	vec4(GLfloat s = GLfloat(0.0)) :
		x(s), y(s), z(s), w(s) {}

	vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) :
		x(x), y(y), z(z), w(w) {}

	vec4(const vec4& v) { x = v.x;  y = v.y;  z = v.z;  w = v.w; }

	vec4 operator + (const vec4& v) const {
		return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	vec4 operator - (const vec4& v) const {
		return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	vec4 operator * (const GLfloat s) const {
		return vec4(s*x, s*y, s*z, s*w);
	}

	vec4 operator * (const vec4& v) const {
		return vec4(x*v.x, y*v.y, z*v.z, w*v.z);
	}

	friend vec4 operator * (const GLfloat s, const vec4& v) {
		return v * s;
	}

	vec4 operator / (const GLfloat s) const {
#ifdef DEBUG
		if (std::fabs(s) < DivideByZeroTolerance) {
			std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
				<< "Division by zero" << std::endl;
			return vec4();
		}
#endif // DEBUG

		GLfloat r = GLfloat(1.0) / s;
		return *this * r;
	}
};

struct vec3 {
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;

	vec3(GLfloat s = GLfloat(0.0)) :
		x(s), y(s), z(s) {}

	vec3(GLfloat x, GLfloat y, GLfloat z) :
		x(x), y(y), z(z) {}

	vec3(const vec3& v) { x = v.x;  y = v.y;  z = v.z; }

	vec3 operator - (const vec3& v) const {
		return vec3(x - v.x, y - v.y, z - v.z);
	}

	vec3 operator * (const GLfloat s) const {
		return vec3(s*x, s*y, s*z);
	}

	vec3 operator * (const vec3& v) const {
		return vec3(x*v.x, y*v.y, z*v.z);
	}

	friend vec3 operator * (const GLfloat s, const vec3& v) {
		return v * s;
	}

	vec3 operator / (const GLfloat s) const {
#ifdef DEBUG
		if (std::fabs(s) < DivideByZeroTolerance) {
			std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
				<< "Division by zero" << std::endl;
			return vec3();
		}
#endif // DEBUG

		GLfloat r = GLfloat(1.0) / s;
		return *this * r;
	}
};

vec3 cross(const vec4& a, const vec4& b) {
	return vec3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

GLfloat dot(const vec3& u, const vec3& v) {
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

GLfloat length(const vec3& v) {
	return sqrt(dot(v, v));
}

vec3 normalize(const vec3& v) {
	return v / length(v);
}