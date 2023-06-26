#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	//gl_Position = model * vec4(0.5, 0.5, 0.5, 1.0);
	gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
}