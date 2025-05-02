#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "PbrBall.h"
#include "ResourceManager.h"

class Scene
{
public:
    Scene(const int width, const int height);

    void Run(GLFWwindow *window);

private:
    void loadTexturesAndShaders();
    int WIDTH, HEIGHT;
};