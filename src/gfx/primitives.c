#include "primitives.h"

void createCube(VAO* cubeVAO)
{   
    createVAO(cubeVAO, (float*)cube_vertices, 24*8, cube_indices, 36);
}

void drawCube(VAO* cubeVAO)
{
    bindVAO(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
}

void createPlane(VAO* planeVAO)
{
    createVAO(planeVAO, (float*)planeVertices, 6*8, planeIndices, 6);
}

void drawPlane(VAO* planeVAO)
{
    bindVAO(planeVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}