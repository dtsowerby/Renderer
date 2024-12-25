#pragma once

unsigned int createVertexShader(const char* file); 
unsigned int createFragmentShader(const char* file);
unsigned int createTesselationControlShader(const char* file);
unsigned int createTesselationEvaluatationShader(const char* file);
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

//make more efficient
#include <cglm/cglm.h>
#include <glad.h>
inline void setUniformMat4(const char* name, mat4 m, unsigned int shaderProgram)
{
    GLint location = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, m[0]);
}

inline void useShader(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
}