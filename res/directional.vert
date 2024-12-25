#version 450 core
layout (location = 0) in vec3 pos;

out vec4 vertexColor;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(pos, 1.0);
    vertexColor = vec4(pos.y/100., pos.y/100., pos.y/100., 1.0);
}
