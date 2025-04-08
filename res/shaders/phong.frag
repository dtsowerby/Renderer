#version 450 core

out vec4 FragColor;

in vec3 pos;
in vec3 normal;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NUM_POINT_LIGHTS 2
uniform Material material;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirLight dirLight;

uniform vec3 camPos;
uniform vec3 lightPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0);
    vec3 specular = light.specular * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * material.ambient;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0);
    vec3 specular = light.specular * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return ((ambient + diffuse +specular) * attenuation);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * max(dot(normal, lightDir), 0.0);
    vec3 specular = light.specular * pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    return ((ambient + diffuse +specular) * attenuation * intensity);
}

void main()
{   
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camPos - pos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NUM_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, pos, viewDir);
    }

    result += CalcSpotLight(spotLight, norm, pos, viewDir);

    FragColor = vec4(result, 0.0);
}