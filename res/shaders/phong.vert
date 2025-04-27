#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 pos;
out vec3 normal;
out vec4 fragPosLightSpace;

uniform mat4 mvp;
uniform mat4 lightSpaceMatrix;

void main()
{   
    pos = aPos;
    normal = aNormal;
    fragPosLightSpace = lightSpaceMatrix * vec4(pos, 1.0);
    gl_Position = mvp * vec4(pos, 1.0);
}
