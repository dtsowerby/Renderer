#pragma once
#include "../gfx/vao.h"
#include "../gfx/camera.h"

// Terrain Settings
const static int chunkSize = 501; //unsigned messes up terrain gen

typedef struct TerrainChunk
{
    unsigned int verticesCount;
    unsigned int indicesCount;
    GLfloat* vertices;
    GLuint* indices;
    // include most data twice -> probs not good
    VAO vao;
} TerrainChunk;

void generateChunk(TerrainChunk* terrainChunk, int firstX, int firstZ)
{   
    terrainChunk->verticesCount = 8*chunkSize * chunkSize;
    terrainChunk->indicesCount = 3*2*(chunkSize-1)*(chunkSize-1);
    terrainChunk->indices = malloc(sizeof(terrainChunk->indices)*terrainChunk->indicesCount);
    terrainChunk->vertices = malloc(sizeof(terrainChunk->vertices)*terrainChunk->verticesCount);

    int vertexIndex = 0;
    int indexIndex = 0;
    
    for(int z = firstZ; z < (firstZ+chunkSize); z++)
    {
        for(int x = firstX; x < (firstX+chunkSize); x++)
        {   
            float perlin = (4*(perlin2d((float)(x)/2000, (float)(z)/2000, 4, 12)-0.25));

            //position
            terrainChunk->vertices[vertexIndex++] = ((float)(x)*2);
            terrainChunk->vertices[vertexIndex++] = perlin*perlin*perlin*perlin*10.f;
            terrainChunk->vertices[vertexIndex++] = ((float)(z)*2);

            // normals -> for now
            terrainChunk->vertices[vertexIndex++] = 0.0f;
            terrainChunk->vertices[vertexIndex++] = 1.0f;
            terrainChunk->vertices[vertexIndex++] = 0.0f;

            // Texture Coordinates
            int texSize = 48.0f;
            terrainChunk->vertices[vertexIndex++] = ((float)(x - firstX) / (chunkSize)) * texSize; // U
            terrainChunk->vertices[vertexIndex++] = ((float)(z - firstZ) / (chunkSize)) * texSize; // V

            int adjustedX = x-firstX;
            int adjustedZ = z-firstZ;

            if (adjustedX < (chunkSize-1) && adjustedZ < chunkSize-1) {

                int topLeft = adjustedZ * chunkSize + adjustedX;
                int topRight = topLeft + 1;
                int bottomLeft = (adjustedZ + 1) * chunkSize + adjustedX;
                int bottomRight = bottomLeft + 1;

                terrainChunk->indices[indexIndex++] = topLeft;
                terrainChunk->indices[indexIndex++] = bottomLeft;
                terrainChunk->indices[indexIndex++] = topRight;
                terrainChunk->indices[indexIndex++] = topRight;
                terrainChunk->indices[indexIndex++] = bottomLeft;
                terrainChunk->indices[indexIndex++] = bottomRight;
            }
        }
    }
    // Accumulate normals
    for (int i = 0; i < terrainChunk->indicesCount; i += 3)
    {
        GLuint i0 = terrainChunk->indices[i];
        GLuint i1 = terrainChunk->indices[i + 1];
        GLuint i2 = terrainChunk->indices[i + 2];

        GLfloat* v0 = &terrainChunk->vertices[i0 * 8];
        GLfloat* v1 = &terrainChunk->vertices[i1 * 8];
        GLfloat* v2 = &terrainChunk->vertices[i2 * 8];

        // Position vectors
        float x0 = v0[0], y0 = v0[1], z0 = v0[2];
        float x1 = v1[0], y1 = v1[1], z1 = v1[2];
        float x2 = v2[0], y2 = v2[1], z2 = v2[2];

        // Edges
        float ux = x1 - x0, uy = y1 - y0, uz = z1 - z0;
        float vx = x2 - x0, vy = y2 - y0, vz = z2 - z0;

        // Cross product (normal)
        float nx = uy * vz - uz * vy;
        float ny = uz * vx - ux * vz;
        float nz = ux * vy - uy * vx;

        // Add the normal to each vertex's normal
        for (int j = 0; j < 3; j++) {
            GLuint idx = terrainChunk->indices[i + j];
            terrainChunk->vertices[idx * 8 + 3] += nx;
            terrainChunk->vertices[idx * 8 + 4] += ny;
            terrainChunk->vertices[idx * 8 + 5] += nz;
        }
    }
    createVAO(&terrainChunk->vao, terrainChunk->vertices, terrainChunk->verticesCount, terrainChunk->indices, terrainChunk->indicesCount);
}

void drawTerrain(TerrainChunk* chunks, unsigned int chunkCount)
{
    for(int i = 0; i < chunkCount; i++)
    {
        bindVAO(&chunks[i].vao);
        glDrawElements(GL_TRIANGLES, chunks[i].vao.indexCount, GL_UNSIGNED_INT, (void*)0);
    }
}