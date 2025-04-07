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

#include "game/terrain.h"

GLuint vertexBufferID;
GLuint elementBuffer;

unsigned int shader1;
unsigned int depthShaderProgram;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;

Camera camera;
float speed = 1000.0f;

TerrainChunk* chunks;
unsigned int chunkCount = 16;

vec3 lightPos = {0.0f, 10.0f, 0.0f};
vec3 lightColour = {1.0f, 1.0f, 1.0f};
vec3 objectColour = {1.0f, 0.5f, 0.31f};

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

    unsigned int vertexShader = createVertexShader("res/lighting.vert");
    unsigned int fragmentShader = createFragmentShader("res/lighting.frag");

    shader1 = createShaderProgram(vertexShader, fragmentShader);

    useShader(shader1);
}

void update()
{   
    const mat4 model = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    // reset viewport
    glViewport(0, 0, state.windowWidth, state.windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setView(&camera);
    mat4 mvp;
    getMVP(mvp, &camera, model);
    useShader(shader1);
    setUniformMat4("mvp", mvp[0], shader1);

    setUniformVec3("camPos", camera.position, shader1);
    setUniformVec3("lightPos", lightPos, shader1);
    setUniformVec3("lightColour", lightColour, shader1);
    setUniformVec3("objectColour", objectColour, shader1);

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