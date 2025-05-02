#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	createShader(vertexFilePath, fragmentFilePath);
}

void Shader::Bind()
{
	glUseProgram(m_RendererID);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

void Shader::setUniformMatrix4f(const std::string& uniformName, glm::mat4 matrix)
{
	unsigned int transformLoc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformMatrix3f(const std::string& uniformName, glm::mat3 matrix)
{
	unsigned int transformLoc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniformMatrix3fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec4f(const std::string& uniformName, glm::vec4 vector4)
{
	unsigned int transformLoc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniform4fv(transformLoc, 1, glm::value_ptr(vector4));
}

void Shader::setUniformVec3f(const std::string& uniformName, glm::vec3 vector3)
{
	unsigned int vecLoc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniform3fv(vecLoc, 1, glm::value_ptr(vector3));
}

void Shader::setUniformVec2f(const std::string& uniformName, glm::vec2 vector2)
{
	unsigned int vecLoc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniform3fv(vecLoc, 1, glm::value_ptr(vector2));
}

void Shader::setUniformInt(const std::string& uniformName, int value)
{
	unsigned int loc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniform1i(loc, value);
}

void Shader::setUniformFloat(const std::string& uniformName, float value)
{
	unsigned int loc = glGetUniformLocation(m_RendererID, uniformName.c_str());
	glUniform1f(loc, value);
}

void Shader::checkShaderCompilationStatus(unsigned int shader, const std::string& shaderName)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::cout << "Failed to compile shader: " << shaderName << std::endl;
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}
}

void Shader::checkShaderProgramLinkStatus(unsigned int shaderProgram)
{
	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link program: " << infoLog << std::endl;
	}
}

void Shader::createShader(const std::string vertexPath, const std::string fragmentPath)
{
	unsigned int vertexShader, fragmentShader;

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkShaderCompilationStatus(vertexShader, "Vertex Shader");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompilationStatus(fragmentShader, "Fragment Shader");

	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);
	glLinkProgram(m_RendererID);
	checkShaderProgramLinkStatus(m_RendererID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}