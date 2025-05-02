#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture() {}
	Texture(const std::string &textureFile);

	void Bind();
	void UnBind();

	unsigned int GetRendererID() { return m_RendererID; }

private:
	void loadTexture(const std::string &textureFile);

private:
	unsigned int m_RendererID;
};
