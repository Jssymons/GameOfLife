#version 430 core

in vec3 fColour;
out vec4 outColour;


void main() {
	outColour = vec4(fColour, 1.0);
}