#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 pos;
out vec3 normal;

uniform mat4 mvp;

void main()
{   
    pos = aPos;
    normal = aNormal;
    gl_Position = mvp * vec4(pos, 1.0);
}
