#pragma once

#include <cglm/cglm.h>

typedef struct Camera
{
    vec3 position;

    mat4 view;
    mat4 projection;

    float yaw;
    float pitch;

    float fov;
    float nearZ;
    float farZ;

    vec3 forward;
    vec3 right;
} Camera;

const static vec3 WorldUp = {0, 1, 0};

void createCamera(Camera* camera, vec3 position);

void setView(Camera* camera);

void setProjection(Camera* camera);

void getMVP(mat4* mvp, Camera* camera, mat4* model);


