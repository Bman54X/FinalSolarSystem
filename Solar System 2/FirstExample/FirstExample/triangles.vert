#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vTexCoord;

uniform mat4 model_matrix;

out vec4 myColor;
out vec2 texCoord;

void main() {
	myColor = vertexColor;
	gl_Position = model_matrix * vPosition;
	texCoord = vTexCoord;

}