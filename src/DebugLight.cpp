#include "DebugLight.h"

DebugLight::DebugLight()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void DebugLight::Draw(Shader& shader, Camera* camera, glm::mat4 proj, glm::vec3 pos, glm::vec3 color)
{
	glBindVertexArray(m_VAO);
	shader.Bind();
	m_Model = glm::mat4(1.0f);
	m_Model = glm::translate(m_Model, pos);
	shader.setUniformMatrix4f("uModel", m_Model);
	shader.setUniformMatrix4f("uView", camera->GetViewMatrix());
	shader.setUniformMatrix4f("uProjection", proj);
	shader.setUniformVec3f("uColor", color);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}