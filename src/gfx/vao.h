#pragma once

#include <glad.h>
#include <stdlib.h>

typedef struct {
    GLuint id;
    GLuint vertexBufferID;
    GLuint elementBufferID;
    unsigned int vertexCount;
    unsigned int indexCount;
} VAO;

void createVAO(VAO* vao, GLfloat* vertices, unsigned int vertexCount, GLuint* indices, unsigned int indexCount);
void bindVAO(VAO* vao);
void deleteVAO(VAO* vao);