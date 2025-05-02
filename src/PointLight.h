#pragma once

#include <glm/glm.hpp>
#include "DebugLight.h"

struct Light {
	glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
    glm::vec3 lightPos = { 0.0f, 0.0f, 0.0f };
    float lightRadius = 10.0f;
    float lightHeight = 1.5f;
    float lightAngle = 0.0f;
    float rotationSpeed = 0.0f;
    glm::vec2 lightPositionOnCircle = glm::vec2(0.0f);
    float lightScale = 100.0f;

    DebugLight debugger;
    void drawDebug(Shader& shader, Camera& camera, glm::mat4& proj) {
        debugger.Draw(shader, &camera, proj, lightPos, lightColor * lightScale);
    }

};
