#include "camera.h"

void createCamera(Camera* camera, vec3 position)
{
    camera->position[0] = position[0];
    camera->position[1] = position[1];
    camera->position[2] = position[2];

    camera->fov = 45;

    camera->yaw = 0;
    camera->pitch = 0;

    camera->nearZ = 0.1f;
    camera->farZ = FLT_MAX;
}

void setView(Camera* camera)
{   
    camera->forward[0] = (float)(cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)));
    camera->forward[1] = (float)sin(glm_rad(camera->pitch));
    camera->forward[2] = (float)(sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)));

    glm_normalize(camera->right);
    glm_cross(WorldUp, camera->forward, camera->right);
    glm_normalize(camera->right);
    glm_vec3_negate(camera->right);
    //glm_cross(right, forward, up);
    //glm_normalize(up);

    vec3 final;
    glm_vec3_add(camera->forward, camera->position, final);
    glm_vec3_copy(camera->forward, camera->lookingAt);
    glm_cross(WorldUp, camera->right, camera->forward);
    glm_normalize(camera->forward);
    glm_lookat(camera->position, final, WorldUp, camera->view);
}

void setProjection(Camera* camera)
{
    glm_perspective(glm_rad(45), 4.0f/3.0f, camera->nearZ, camera->farZ, camera->projection);
}

void getMVP(mat4 mvp, Camera* camera, mat4 model)
{
    glm_mul(camera->projection, camera->view, mvp);
    glm_mul(mvp, model, mvp);
}
