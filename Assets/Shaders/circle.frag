#version 330 core

in vec2 Value;
out vec4 FragColor;

void main()
{
    float R = 1.0;
    float R2 = 0.95;

    float dist = sqrt(dot(Value,Value));
    if (dist >= R || dist <= R2)
    {
        discard;
    }

    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}