#pragma once

#include <map>
#include <string>

#include "Shader.h"
#include "Texture.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static Shader LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& name);
	static Shader GetShader(const std::string& name);

	static Texture LoadTexture(const std::string& textureFile, const std::string& name);
	static Texture GetTexture(const std::string& name);

	static void Clear();

private:
	ResourceManager() {};
	~ResourceManager();

private:

};