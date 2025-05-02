#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "ResourceManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Renderer.h"

struct WaterData
{
    unsigned int reflectionTexture;
    unsigned int refractionTexture;
    unsigned int dudvMapTexture;
    unsigned int normalMapTexture;
    float waveStrength;
    float moveFactor;
};

class Water
{
public:
    Water(glm::vec3 position);

    void Draw(Shader &shader, WaterData waterData, Camera &camera, Light &light, glm::mat4 projection);

    float GetHeight();

private:
    unsigned int m_WaterVao;
    glm::vec3 m_Position = glm::vec3(0);
    std::vector<float> m_WaterVertices = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                                           // positions        // texCoords
        1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f};
    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
};
