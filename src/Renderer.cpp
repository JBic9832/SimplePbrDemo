#include "Renderer.h"

Renderer::Renderer()
{
	m_MainShader = ResourceManager::GetShader("basic");
	m_WaterShader = ResourceManager::GetShader("water");

    m_MainShader.Bind();
    m_MainShader.setUniformInt("albedoMap", 0);
    m_MainShader.setUniformInt("normalMap", 1);
    m_MainShader.setUniformInt("metallicMap", 2);
    m_MainShader.setUniformInt("roughnessMap", 3);
    m_MainShader.setUniformInt("aoMap", 4);
    m_MainShader.UnBind();
}

Renderer::Renderer(Shader mainShader, Shader m_WaterShader)
{
	m_MainShader = mainShader;
	m_WaterShader = m_WaterShader;
}

Renderer::~Renderer()
{
}

#include <iostream>

void Renderer::drawScene(Camera& camera, SkyBox& skybox, PbrBall& object, Light& light, glm::vec4& clipPlane)
{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_CLIP_DISTANCE0);
	
		skybox.Draw(m_ProjectionMatrix, glm::mat4(glm::mat3(camera.GetViewMatrix())));


		m_MainShader.Bind();

		m_MainShader.setUniformVec4f("uWaterClipPlane", clipPlane);
		m_MainShader.setUniformMatrix4f("uProjection", m_ProjectionMatrix);
		m_MainShader.setUniformMatrix4f("uView", camera.GetViewMatrix());
		m_MainShader.setUniformVec3f("camPos", camera.GetCameraPosition());

		light.lightPositionOnCircle.x = (light.lightRadius)*sin(glm::radians(light.lightAngle));
		light.lightPositionOnCircle.y = (light.lightRadius)*cos(glm::radians(light.lightAngle));
		light.lightPos = glm::vec3(light.lightPositionOnCircle.x, light.lightHeight, light.lightPositionOnCircle.y);
		m_MainShader.setUniformVec3f("lightPositions[0]", light.lightPos);
		m_MainShader.setUniformVec3f("lightColors[0]", light.lightColor * light.lightScale);

		object.Draw(m_MainShader);

		Shader lightShader = ResourceManager::GetShader("light");
		lightShader.Bind();
		lightShader.setUniformVec4f("uWaterClipPlane", clipPlane);
		light.drawDebug(lightShader, camera, m_ProjectionMatrix);
}
