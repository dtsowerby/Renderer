#include <stdio.h>
#include <stdlib.h>

#include <glad.h>
#include <glfw3.h>

#include <cglm/cglm.h>

#include "util/files.h"
#include "util/noise.h"

#include "state.h"

#include "gfx/window/window.h"
#include "gfx/camera.h"
#include "gfx/shader.h"
#include "gfx/vao.h"
#include "gfx/texture.h"

#include "game/terrain.h"

GLuint vertexBufferID;
GLuint elementBuffer;

unsigned int textureShaderProgram;
unsigned int depthShaderProgram;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;

Camera camera;
float speed = 1000.0f;

TerrainChunk* chunks;
unsigned int chunkCount = 16;

Texture grass;

//ui
float mouseSpeed = 0.1f;
float xpos;
float ypos;

bool wireFrame = false;

void start()
{      
    vec3 camPos = {0, 20, 0};
    createCamera(&camera, camPos);
    setProjection(&camera);

    chunks = malloc(sizeof(*chunks)*chunkCount);
    for(int x = 0; x < 4; x++)
    {
        for(int y = 0; y < 4; y++)
        {
            generateChunk(&chunks[x*4+y], (x-2)*500, (y-2)*500);
        }
    }

    unsigned int vertexShader = createVertexShader("res/shadowTexture.vert");
    unsigned int fragmentShader = createFragmentShader("res/shadowTexture.frag");

    unsigned int depthVertShader = createVertexShader("res/shadowDepth.vert");
    unsigned int depthFragShader = createFragmentShader("res/shadowDepth.frag");

    //unsigned int tesselationControlShader = createTesselationControlShader();
    //unsigned int tesselationEvaluationShader = createTesselationControlShader();

    textureShaderProgram = createShaderProgram(vertexShader, fragmentShader);

    useShader(textureShaderProgram);
    grass = loadTexture("res/grass.jpg");
    setUniformInt1("diffuseTexture", 0, textureShaderProgram);
    setUniformInt1("shadowMap", 1, textureShaderProgram);

    depthShaderProgram = createShaderProgram(depthVertShader, depthFragShader);
    useShader(depthShaderProgram);
    //Shadow Mapping
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void update()
{   
    mat4 model = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    mat4 lightProjection, lightView;
    mat4 lightSpaceMatrix;
    vec3 lightPos = {0, 10, 0};
    vec3 up = {0, 1, 0};
    float near_plane = 1.0f, far_plane = 7.5f; //performance???
    //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
    glm_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane, &lightProjection);
    glm_lookat(lightPos, glm_vec3_zero, up, lightView);
    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);
    // render scene from light's point of view
    useShader(depthShaderProgram);
    setUniformMat4("lightSpaceMatrix", lightSpaceMatrix[0], depthShaderProgram);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grass.id);
        drawTerrain(chunks, chunkCount);
        //renderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, state.windowWidth, state.windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setView(&camera);
    mat4 mvp;  
    getMVP(mvp, &camera, model);

    useShader(textureShaderProgram);
    setUniformMat4("mvp", mvp[0], textureShaderProgram);
    setUniformVec3("viewPos", camera.position, textureShaderProgram);
    setUniformVec3("lightPos", lightPos, textureShaderProgram);
    setUniformMat4("lightSpaceMatrix", lightSpaceMatrix[0], textureShaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grass.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    drawTerrain(chunks, chunkCount);

    int kjnwe = glGetError();
    if(kjnwe != 0)
        printf("x: %i\n", kjnwe);
    //printf("ms: %f\n", state.deltaTime*1000);
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
    if(glfwGetKey(state.window, GLFW_KEY_TAB))
    {   
        if(wireFrame)
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
        wireFrame = !wireFrame;
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
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
    InitializeWindow(start, update, input);
    return 0;
}