#version 330 core

uniform vec4 color;
uniform float minRadius;

in vec2 Value;
out vec4 FragColor;

void main()
{
    float R = 1.0;
    float R2 = 0.95;

    float dist = sqrt(dot(Value,Value));
    if (dist >= R || dist <= minRadius)
    {
        discard;
    }

    FragColor = color;
}