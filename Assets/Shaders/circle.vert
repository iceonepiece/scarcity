#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aValue;

out vec2 Value;
uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
	Value = aValue;
	gl_Position = viewProjection * model * vec4(aPos, 1.0);
}