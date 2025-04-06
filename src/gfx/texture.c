#include "texture.h"
#include "stb_image.h"
#include "glad.h"

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