#include "vao.h"

void createVAO(VAO* vao, GLfloat* vertices, unsigned int vertexCount, GLuint* indices, unsigned int indexCount) 
{   
    vao->vertexCount = vertexCount;
    vao->indexCount = indexCount;

    glGenVertexArrays(1, &vao->id);
    glBindVertexArray(vao->id);

    glGenBuffers(1, &vao->vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vao->vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * vertexCount, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vao->elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices) * indexCount, indices, GL_STATIC_DRAW);

    //Only will work for basic
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void bindVAO(VAO* vao) 
{
    glBindVertexArray(vao->id);
}

void deleteVAO(VAO* vao) 
{
    glDeleteBuffers(1, &vao->vertexBufferID);
    glDeleteBuffers(1, &vao->elementBufferID);
    glDeleteVertexArrays(1, &vao->id);
}