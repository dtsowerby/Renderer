#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad.h>
#include "../util/files.h" 

unsigned int createVertexShader(const char* file)
{   
    int  success;
    char infoLog[512];
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = file2buf(file);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "vertex compilation failed -> %s -> %s\n", infoLog, file);
    }
    if (vertexShader == 0) {
        fprintf(stderr, "glCreateShader(GL_VERTEX_SHADER) failed\n");
        return 0;
    }
    return vertexShader;
}

unsigned int createFragmentShader(const char* file)
{   
    int  success;
    char infoLog[512];
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = file2buf(file);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "vertex compilation failed -> %s -> %s\n", infoLog, file);
    }
    if (fragmentShader == 0) {
        fprintf(stderr, "glCreateShader(GL_VERTEX_SHADER) failed\n");
        return 0;
    }
    return fragmentShader;
}

unsigned int createTesselationControlShader(const char* file)
{
    int  success;
    char infoLog[512];
    unsigned int tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    const char* tcShaderSource = file2buf(file);
    glShaderSource(tcShader , 1, &tcShaderSource, NULL);
    glCompileShader(tcShader );
    glGetShaderiv(tcShader , GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(tcShader , 512, NULL, infoLog);
        fprintf(stderr, "tesselation control compilation failed -> %s -> %s\n", infoLog, file);
    }
    return tcShader;
}

unsigned int createTesselationEvaluatationShader(const char* file)
{
    int  success;
    char infoLog[512];
    unsigned int teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    const char* teShaderSource = file2buf(file);
    glShaderSource(teShader , 1, &teShaderSource, NULL);
    glCompileShader(teShader );
    glGetShaderiv(teShader , GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(teShader , 512, NULL, infoLog);
        fprintf(stderr, "tesselation evaluation compilation failed -> %s -> %s\n", infoLog, file);
    }
    return teShader;
}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{   
    int  success;
    char infoLog[512];
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "shader linking failed -> %s\n", infoLog);
    }
    if (shaderProgram == 0) {
        fprintf(stderr, "glCreateProgram() failed\n");
        return 0;
    }
    return shaderProgram;
}

unsigned int createShaderProgramS(const char* vertexShaderPath, const char* fragmentShaderPath)
{   
    unsigned int vertexShader = createVertexShader(vertexShaderPath);
    unsigned int fragmentShader = createFragmentShader(fragmentShaderPath);

    int  success;
    char infoLog[512];
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "shader linking failed -> %s\n", infoLog);
    }
    if (shaderProgram == 0) {
        fprintf(stderr, "glCreateProgram() failed\n");
        return 0;
    }
    return shaderProgram;
}