#version 330 core

layout (location = 0) in vec2 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// out vec4 opColor;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 0.0f, 1.0f);
    // opColor = vec4(position, 0.0f, 1.0f);
}
