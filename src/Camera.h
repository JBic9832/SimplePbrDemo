#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(GLFWwindow* window);

	void update(GLFWwindow* window, float deltaTime);

	glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
	glm::vec3 GetCameraPosition() { return m_CameraPosition; }
	void setDisableRotation(bool disable) { m_Disabled = disable; }
	void setCameraPosition(glm::vec3 position) { m_CameraPosition = position; };
	float getCameraPitch() { return m_Pitch; }
	void invertPitch() { m_Pitch = -m_Pitch; };
	void updateViewMatrix(GLFWwindow* window) {
		updateCameraRotation(window);
		m_ViewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);
	}
	

private:
	void updateCameraRotation(GLFWwindow* window);
	void processInput(GLFWwindow* window, float deltaTime);

private:
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::vec3 m_CameraPosition = glm::vec3(0.0f, 1.0f, 3.0f);
	glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_CameraSpeed = 5.0f;
	float m_SprintMultiplier = 1.0f;
	float m_LastMouseX;
	float m_LastMouseY;
	float m_Pitch = 0.0f;
	float m_Yaw = -90.0f;
	bool m_Disabled = false;


};
