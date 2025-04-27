#pragma once

#include <cglm.h>
#include "vao.h"

typedef struct {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} Vertex;

static Vertex cube_vertices[24] = {
    // Back face
    {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},

    // Front face
    {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},

    // Left face
    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},

    // Right face
    {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},

    // Bottom face
    {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},

    // Top face
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
};

static unsigned int cube_indices[36] = {
    0, 1, 2, 2, 3, 0,        // back
    4, 5, 6, 6, 7, 4,        // front
    8, 9,10,10,11, 8,        // left
   12,13,14,14,15,12,        // right
   16,17,18,18,19,16,        // bottom
   20,21,22,22,23,20         // top
};


static float planeVertices[] = {
    // positions            // normals         // texcoords
     1.0f, 0.5f,  1.0f,  0.0f, 1.0f, 0.0f,   1.0f,  1.0f,
    -1.0f, 0.5f,  1.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,
    -1.0f, 0.5f, -1.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,

     1.0f, 0.5f,  1.0f,  0.0f, 1.0f, 0.0f,   1.0f,  1.0f,
    -1.0f, 0.5f, -1.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
     1.0f, 0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

// Indices for two triangles
static unsigned int planeIndices[] = {
    0, 1, 2,
    0, 2, 5
};

void createCube(VAO* cubeVAO);
void drawCube(VAO* cubeVAO);

void createPlane(VAO* planeVAO);
void drawPlane(VAO* planeVAO);
