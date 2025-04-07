#version 450 core

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

uniform vec3 camPos;
uniform vec3 lightPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;

void main()
{   
    //ambient
    float ambient = 0.1;

    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos-pos);
    float diffuse = max(dot(norm, lightDir), 0.0);

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32);

    FragColor = vec4((ambient+diffuse+specular)*lightColour*objectColour, 0.0);
}