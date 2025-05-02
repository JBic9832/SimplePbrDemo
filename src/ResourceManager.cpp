#include "ResourceManager.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& name)
{
	Shaders[name] = Shader(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name) {
	return Shaders[name];
}

Texture ResourceManager::LoadTexture(const std::string& textureFile, const std::string& name)
{
	Textures[name] = Texture(textureFile);
	return Textures[name];
}

Texture ResourceManager::GetTexture(const std::string& name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.GetRendererId());
}

ResourceManager::~ResourceManager()
{
	for (const auto &currentShader : Shaders) {
		Shader shader = currentShader.second;
		unsigned int shaderID = shader.GetRendererId();
		glDeleteShader(shaderID);
	}
}
