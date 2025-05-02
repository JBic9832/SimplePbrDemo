#include "Water.h"

Water::Water(glm::vec3 position)
{
	m_Position = position;
	unsigned int vbo;
	glGenVertexArrays(1, &m_WaterVao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(m_WaterVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_WaterVertices.size() * sizeof(float), m_WaterVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void Water::Draw(Shader& shader, WaterData waterData, Camera& camera, Light& light, glm::mat4 projection)
{
	shader.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterData.reflectionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterData.refractionTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, waterData.dudvMapTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, waterData.normalMapTexture);

	shader.setUniformFloat("moveFactor", waterData.moveFactor);

	shader.setUniformMatrix4f("uProjection", projection);
	shader.setUniformMatrix4f("uView", camera.GetViewMatrix());
	shader.setUniformFloat("waveStrength", waterData.waveStrength);
	shader.setUniformVec3f("cameraPosition", camera.GetCameraPosition());
	shader.setUniformVec3f("lightColor", light.lightColor);
	shader.setUniformVec3f("lightPosition", light.lightPos);

	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	float scaleFactor = 1000.0f;
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(scaleFactor, 1.0f, scaleFactor));
	shader.setUniformMatrix4f("uModel", m_ModelMatrix);
	glBindVertexArray(m_WaterVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

float Water::GetHeight()
{
	return m_Position.y;
}
