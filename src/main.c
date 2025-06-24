#include <stdio.h>
#include <stdlib.h>

#include <glad.h>
#include <glfw3.h>

#include <cglm/cglm.h>

#include "util/files.h"
#include "util/noise.h"

#include "state.h"

#include "window/window.h"
#include "gfx/camera.h"
#include "gfx/shader.h"
#include "gfx/vao.h"
#include "gfx/texture.h"

#include "gfx/primitives.h"

#include "game/terrain.h"

GLuint vertexBufferID;
GLuint elementBuffer;

unsigned int blinnphong;
unsigned int depthShaderProgram;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;

Camera camera;
float speed = 1.0f;

TerrainChunk* chunks;
unsigned int chunkCount = 16;

Texture grass;

//ui
float mouseSpeed = 0.1f;
float xpos;
float ypos;

bool wireFrame = false;

unsigned int debugShader;

VAO planeVAO;
VAO cubeVAO;

mat4 cubemodel;
mat4 planemodel;
mat4 debugmodel;
mat4 terrainmodel;

void start()
{      
    vec3 camPos = {0, 2, 0};
    createCamera(&camera, camPos);

    // Terrain creation
    chunks = malloc(sizeof(*chunks)*chunkCount);
    for(int x = 0; x < 4; x++)
    {
        for(int y = 0; y < 4; y++)
        {
            generateChunk(&chunks[x*4+y], (x-2)*500, (y-2)*500);
        }
    }

    createCube(&cubeVAO);
    glm_mat4_identity(cubemodel);
    glm_translate(cubemodel, (vec3){0.0f, 0.0f, 0.0f});
    glm_mat4_scale(cubemodel, 0.5);

    createPlane(&planeVAO);
    glm_mat4_identity(planemodel);
    glm_translate(planemodel, (vec3){0.0f, -1.0f, 0.0f});

    glm_mat4_identity(debugmodel);
    glm_translate(debugmodel, (vec3){2.0f, -2.0f, 0.0f});

    glm_mat4_identity(terrainmodel);
    glm_mat4_scale(terrainmodel, 0.01f);
    glm_translate(terrainmodel, (vec3){0.0f, -5.0f, 0.0f});

    depthShaderProgram = createShaderProgramS("res/shaders/depth.vert", "res/shaders/depth.frag");
    blinnphong = createShaderProgramS("res/shaders/phong.vert", "res/shaders/phong.frag");
    debugShader = createShaderProgramS("res/shaders/debug.vert", "res/shaders/debug.frag");

    // Shadow Stuff
    glGenFramebuffers(1, &depthMapFBO);  
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    useShader(blinnphong);
    setUniformInt1("directionalShadowMap", 0, blinnphong);
    useShader(debugShader);
    setUniformInt1("depthMap", 0, debugShader);
}

// Change mvp to vp + m
void update()
{   
    // render depth
    mat4 lightProjection;
    mat4 lightView;
    mat4 lightSpaceMatrix;

    //setUniformVec3("dirLight.direction", (vec3){-0.2f, -1.0f, -0.3f}, blinnphong);
    vec3 lightPos = {15.0f * (sin(state.time)-0.5f), 12.0f, 0.0f};

    //glm_ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 100000000.0f, lightProjection);
    glm_ortho(-20.0f, 20.0f, -20.0f, 20.0f, camera.nearZ, camera.farZ, lightProjection);
    //glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, camera.nearZ, camera.farZ, camera.projection);
    vec3 camDir;
    glm_vec3_add(lightPos, (vec3){1.0f, -1.0f, 0.0f}, camDir);
    glm_lookat(lightPos, camDir, WorldUp, lightView);
    //glm_lookat(lightPos, camDir,  WorldUp, camera.view);

    //glm_mat4_copy(camera.view, lightView);
    glm_mul(lightProjection, lightView, lightSpaceMatrix);
    useShader(depthShaderProgram);
    setUniformMat4("lightSpaceMatrix", lightSpaceMatrix, depthShaderProgram); //ignoring model for now, usually do per object
    setUniformInt1("depthMap", 0, depthShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
        
        setUniformMat4("model", terrainmodel, depthShaderProgram);
        drawTerrain(chunks, chunkCount);
    
        //DrawCube
        setUniformMat4("model", cubemodel, depthShaderProgram);
        drawCube(&cubeVAO);
        //EndDrawCube

        // DrawDebugPlane
        setUniformMat4("model", planemodel, depthShaderProgram);
        drawPlane(&planeVAO);
        // EndDebugPlane

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, state.windowWidth, state.windowHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    setView(&camera);
    //glm_mat4_print(camera.view, stdout);
    //getMVP(mvp, &camera, model);

    useShader(blinnphong);
    vec3 lightColour = {1.0f, 1.0f, 1.0f};
    vec3 objectColour = {1.0f, 0.5f, 0.31f};
    setUniformMat4("projection", camera.projection, blinnphong);
    setUniformMat4("view", camera.view, blinnphong);
    setUniformMat4("lightSpaceMatrix", lightSpaceMatrix, blinnphong); //ignoring model for now, usually do per object
    setUniformInt1("directionalShadowMap", 0, blinnphong);

    setUniformVec3("camPos", camera.position, blinnphong);
    setUniformVec3("lightPos", lightPos, blinnphong);
    setUniformVec3("lightColour", lightColour, blinnphong);
    setUniformVec3("objectColour", objectColour, blinnphong);

    setUniformVec3("material.ambient", (vec3){1.0f, 0.5f, 0.31f}, blinnphong);
    setUniformVec3("material.diffuse", (vec3){1.0f, 0.5f, 0.31f}, blinnphong);
    setUniformVec3("material.specular", (vec3){0.5f, 0.5f, 0.5f}, blinnphong);
    setUniformFloat("material.shininess", 32.0f, blinnphong);

    // Directional light
    setUniformVec3("dirLight.direction", (vec3){-0.2f, -1.0f, -0.2f}, blinnphong);
    setUniformVec3("dirLight.ambient", (vec3){0.05f, 0.05f, 0.05f}, blinnphong);
    setUniformVec3("dirLight.diffuse", (vec3){0.4f, 0.4f, 0.8f}, blinnphong);
    setUniformVec3("dirLight.specular", (vec3){0.5f, 0.5f, 0.5f}, blinnphong);

    // Point light 1
    setUniformVec3("pointLights[0].position", (vec3){0.0f, 50.0f, 0.0f}, blinnphong);
    setUniformVec3("pointLights[0].ambient", (vec3){0.05f, 0.05f, 0.05f}, blinnphong);
    setUniformVec3("pointLights[0].diffuse", (vec3){0.8f, 0.8f, 0.8f}, blinnphong);
    setUniformVec3("pointLights[0].specular", (vec3){1.0f, 1.0f, 1.0f}, blinnphong);
    setUniformFloat("pointLights[0].constant", 1.0f, blinnphong);
    setUniformFloat("pointLights[0].linear", 0.09f, blinnphong);
    setUniformFloat("pointLights[0].quadratic", 0.032f, blinnphong);

    // Point light 2
    setUniformVec3("pointLights[1].position", (vec3){100.0f, 100.0f, 100.0f}, blinnphong);
    setUniformVec3("pointLights[1].ambient", (vec3){0.05f, 0.05f, 0.05f}, blinnphong);
    setUniformVec3("pointLights[1].diffuse", (vec3){0.8f, 0.8f, 0.8f}, blinnphong);
    setUniformVec3("pointLights[1].specular", (vec3){1.0f, 1.0f, 1.0f}, blinnphong);
    setUniformFloat("pointLights[1].constant", 1.0f, blinnphong);
    setUniformFloat("pointLights[1].linear", 0.09f, blinnphong);
    setUniformFloat("pointLights[1].quadratic", 0.032f, blinnphong);

    // Spotlight
    setUniformVec3("spotLight.position", camera.position, blinnphong);
    setUniformVec3("spotLight.direction", camera.lookingAt, blinnphong);
    setUniformVec3("spotLight.ambient", (vec3){0.0f, 0.0f, 0.0f}, blinnphong);
    setUniformVec3("spotLight.diffuse", (vec3){1.0f, 1.0f, 1.0f}, blinnphong);
    setUniformVec3("spotLight.specular", (vec3){1.0f, 1.0f, 1.0f}, blinnphong);
    setUniformFloat("spotLight.constant", 1.0f, blinnphong);
    setUniformFloat("spotLight.linear", 0.09f, blinnphong);
    setUniformFloat("spotLight.quadratic", 0.032f, blinnphong);
    setUniformFloat("spotLight.cutOff", cosf(glm_rad(12.5f)), blinnphong);
    setUniformFloat("spotLight.outerCutOff", cosf(glm_rad(15.0f)), blinnphong);

    setUniformMat4("model", terrainmodel, blinnphong);
    setUniformVec3("objectColour", (vec3){0.0f, 0.0f, 0.0f}, blinnphong);
    drawTerrain(chunks, chunkCount);

    //DrawCube
    setUniformMat4("model", cubemodel, blinnphong);
    setUniformVec3("objectColour", (vec3){0.0f, 0.0f, 0.0f}, blinnphong);
    drawCube(&cubeVAO);
    //EndDrawCube

    // DrawPlane
    setUniformMat4("model", planemodel, blinnphong);
    setUniformVec3("objectColour", (vec3){0.0f, 0.0f, 0.0f}, blinnphong);
    drawPlane(&planeVAO);
    // EndPlane

    useShader(debugShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    setUniformInt1("depthMap", 0, debugShader);
    setUniformMat4("model", debugmodel, debugShader);
    setUniformMat4("view", camera.view, debugShader);
    setUniformMat4("projection", camera.projection, debugShader);
    drawPlane(&planeVAO);

    int kjnwe = glGetError();
    if(kjnwe != 0)
        printf("x: %i\n", kjnwe);
    //printf("ms: %f\n", state.deltaTime*1000);
}

void ui_update()
{
    //igBegin("Colour Editor", NULL, 0);
    //igColorPicker3("Light Colour", (float*)&lightColour, ImGuiColorEditFlags_DisplayRGB);
    //igColorPicker3("Object Colour", (float*)&objectColour, ImGuiColorEditFlags_DisplayRGB);
    //igSliderFloat3("Light Position", (float*)&lightPos, -100.0f, 100.0f, NULL, 0);
    //igEnd();
    //igShowDemoWindow(NULL);
}

void input()
{   
    if (glfwGetKey(state.window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwTerminate();
        exit(0);
    }
    if (glfwGetKey(state.window, GLFW_KEY_W) == GLFW_PRESS){
        //CamPos[2] += speed*state.deltaTime;
        vec3 front = {
            camera.forward[0]*speed*state.deltaTime,
            camera.forward[1]*speed*state.deltaTime,
            camera.forward[2]*speed*state.deltaTime
        };
        glm_vec3_add(camera.position, front, camera.position);
    }
    if (glfwGetKey(state.window, GLFW_KEY_S) == GLFW_PRESS){
        vec3 back = {
            -camera.forward[0]*speed*state.deltaTime,
            -camera.forward[1]*speed*state.deltaTime,
            -camera.forward[2]*speed*state.deltaTime
        };
        glm_vec3_add(camera.position, back, camera.position);
    }
    if (glfwGetKey(state.window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 right2 = {
            camera.right[0]*speed*state.deltaTime,
            camera.right[1]*speed*state.deltaTime,
            camera.right[2]*speed*state.deltaTime
        };
        glm_vec3_add(camera.position, right2, camera.position);
    }
    if (glfwGetKey(state.window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 left = {
            -camera.right[0]*speed*state.deltaTime,
            -camera.right[1]*speed*state.deltaTime,
            -camera.right[2]*speed*state.deltaTime
        };
        glm_vec3_add(camera.position, left, camera.position);
    }
    if (glfwGetKey(state.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        vec3 up2 = {
            -WorldUp[0]*speed*state.deltaTime,
            -WorldUp[1]*speed*state.deltaTime,
            -WorldUp[2]*speed*state.deltaTime
        };
        glm_vec3_add(camera.position, up2, camera.position);
    }
    if (glfwGetKey(state.window, GLFW_KEY_SPACE) == GLFW_PRESS){
        vec3 down = {
            WorldUp[0]*speed*state.deltaTime,
            WorldUp[1]*speed*state.deltaTime,
            WorldUp[2]*speed*state.deltaTime
        };
        glm_vec3_add(camera.position, down, camera.position);
    }
    if(glfwGetKey(state.window, GLFW_KEY_TAB) == GLFW_PRESS)
    {   
        if(wireFrame)
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
        wireFrame = !wireFrame;
    }
    if(glfwGetKey(state.window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if(state.cursorDisabled)
            glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        state.cursorDisabled = !state.cursorDisabled;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{   
    if(!state.cursorDisabled){return;}
    xpos = (float)xposIn;
    ypos = (float)yposIn;

    float xoffset = xpos - state.windowWidth/2;
    float yoffset = state.windowHeight/2 - ypos; // reversed since y-coordinates go from bottom to top

    glfwSetCursorPos(state.window, state.windowWidth/2, state.windowHeight/2);

    xoffset *= mouseSpeed;
    yoffset *= mouseSpeed;

    camera.yaw   += xoffset;
    camera.pitch += yoffset;
    
    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;
}

int main(void)
{   
    InitializeWindow(start, update, input, ui_update);
    return 0;
}