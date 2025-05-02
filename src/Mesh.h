#pragma once

#include<string>
#include <vector>

#include "Shader.h"
#include"Camera.h"

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	//std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	unsigned int m_VAO;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

	// Draws the mesh
	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};