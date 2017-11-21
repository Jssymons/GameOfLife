#version 430 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 inColour;

out vec3 fColour;

uniform float time;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	gl_Position = vec4(vPosition, 1.0, 1.0);
	if (vPosition.x < 0.99 && vPosition.x > -0.99)
	{
		if (vPosition.y < 0.99 && vPosition.y > -0.99)
		{
			//gl_Position = vec4(vPosition.x + (rand(vec2(vPosition.xy))*0.01) * sin(time), vPosition.y + (rand(vec2(vPosition.yx))*0.01) * sin(time), 1, 1);
			gl_Position = vec4(vPosition, 1, 1);
		}
	}
		
	fColour = inColour;
}
