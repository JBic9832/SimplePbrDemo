#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "ResourceManager.h"
#include "Camera.h"
#include "PbrBall.h"
#include "Skybox.h"
#include "Water.h"
#include "PointLight.h"

class Renderer
{
public:
	Renderer();
	Renderer(Shader mainShader, Shader waterShader);
	~Renderer();

	void drawScene(Camera &camera, SkyBox &skybox, PbrBall &object, Light &light, glm::vec4 &clipPlane);
	const glm::mat4 getProjection() { return m_ProjectionMatrix; };

private:
	Shader m_MainShader;
	Shader m_WaterShader;
	glm::mat4 m_ProjectionMatrix = glm::perspective(glm::radians(60.0f), 1600.0f / 800.0f, 0.1f, 1000.0f);
};
