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