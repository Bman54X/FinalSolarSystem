#version 430 core

in vec4 myColor;
in vec2 texCoord;

out vec4 fColor;

uniform sampler2D texture;

void main() {
	//fColor = texture2D(texture, texCoord);// * myColor;
	fColor = myColor;
}