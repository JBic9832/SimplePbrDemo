#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "ResourceManager.h"

struct PbrMaterial
{
	Texture albedo;
	Texture normal;
	Texture metallic;
	Texture roughness;
	Texture ao;
};

class PbrBall
{
public:
	PbrBall(PbrMaterial &material);

	void Draw(Shader &pbrShader);
	const glm::vec3 getPosition() { return m_Position; }
	void setPosition(glm::vec3 position) { m_Position = position; }

private:
	void constructSphere(unsigned int xSegs, unsigned int ySegs);

private:
	unsigned int m_Vao, m_IndexCount;
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	std::vector<float> m_Vertices;
	glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
	PbrMaterial m_Material;
};
