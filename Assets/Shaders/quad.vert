#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out float v_TexIndex;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}