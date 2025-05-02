#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader() {}
	Shader(const std::string &vertexFilePath, const std::string &fragmentFilePath);

	void Bind();
	void UnBind();
	void setUniformMatrix4f(const std::string &uniformName, glm::mat4 matrix);
	void setUniformMatrix3f(const std::string &uniformName, glm::mat3 matrix);
	void setUniformVec4f(const std::string &uniformName, glm::vec4 vector4);
	void setUniformVec3f(const std::string &uniformName, glm::vec3 vector3);
	void setUniformVec2f(const std::string &uniformName, glm::vec2 vector2);
	void setUniformInt(const std::string &uniformName, int value);
	void setUniformFloat(const std::string &uniformName, float value);
	unsigned int GetRendererId() { return m_RendererID; }

private:
	void checkShaderCompilationStatus(unsigned int shader, const std::string &shaderName);
	void checkShaderProgramLinkStatus(unsigned int shaderProgram);
	void createShader(const std::string vertexPath, const std::string fragmentPath);

private:
	unsigned int m_RendererID;
};