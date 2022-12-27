#version 330 core

out vec4 FragColor;

uniform vec3 resolution;
uniform vec4 color;
uniform vec3 pixelPos;
uniform float pixelRadius;

void main()
{

	float dist = distance(pixelPos.xy, gl_FragCoord.xy);

	if (dist > pixelRadius)
		discard;

	FragColor = color;
}