#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad.h>
#include "state.h"

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

void InitializeWindow(void (*start)(), void (*update)(), void (*input)())
{
    //Init GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state.window = glfwCreateWindow( 1024, 768, "Game", NULL, NULL);
    if( state.window == NULL ){
        fprintf( stderr, "GLFW not happy" );
        glfwTerminate();
        return;
    }

    glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(state.window);
    glfwSetCursorPosCallback(state.window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf( stderr, "GLAD not happy" );
        return;
    }

    glViewport(0, 0, 1024, 768);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // I don't really know
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glClearColor(0.8f, 0.2f, 0.4f, 0.0f);

    state.windowHeight = 768;
    state.windowWidth = 1024;
    state.deltaTime = 0.0f;
    state.time = 0;

    start();

    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(state.window))
    {      
        float currentFrame = glfwGetTime();
        state.deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
        state.time += state.deltaTime;
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        input();

        update();

        glfwSwapBuffers(state.window);
        glfwPollEvents();
    }
}