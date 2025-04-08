#pragma once

#include <glfw3.h>

struct State
{
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    bool cursorDisabled;
    union clearColor
    {
        int i[3];
        struct {int x; int y; int z;};
    };
    float deltaTime;
    float time;
};

struct State state;