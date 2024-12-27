#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad.h"

typedef struct
{
    unsigned int id;
    int width, height, nrChannels;
} Texture;

Texture loadTexture(const char* name)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0); 

    unsigned int texID;
    glGenTextures(1, &texID);  

    glBindTexture(GL_TEXTURE_2D, texID);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Texture failed to load: %s\n", name);
    }

    stbi_image_free(data);

    Texture texture;
    texture.id = texID;
    texture.width = width;
    texture.height = height;
    texture.nrChannels = nrChannels;

    return texture;
}