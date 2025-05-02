#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include "Scene.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

const int WIDTH = 1600;
const int HEIGHT = 900;

int main(void)
{
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(WIDTH, HEIGHT, "PBR Demo", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    const char *error = "Failed to load glad";
    std::cout << error << std::endl;
    glfwTerminate();
    return -1;
  }

  Scene scene(WIDTH, HEIGHT);
  scene.Run(window);

  glfwTerminate();
  return 0;
}
