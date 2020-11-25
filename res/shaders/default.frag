#version 330 core

uniform vec4 u_Color = vec4(1, 0, 1, 1);

out vec4 color;

void main()
{
    color = u_Color;
}
