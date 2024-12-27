#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{   
    //FragColor = vec4(TexCoord, 0.0, 1.0);
    FragColor = texture(ourTexture, TexCoord);
}