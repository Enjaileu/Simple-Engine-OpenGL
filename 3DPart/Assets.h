#pragma once
#include <map>
#include <string>
#include "Texture.h"
#include "Shader.h"
using std::map;
using std::string;

class Assets
{
public:
	static std::map<std::string, Texture> textures;
	static std::map<std::string, Shader> shaders;

	//Loads a texture from file
	static Texture LoadTexture(IRenderer& renderer, const string& filename, const string& name);

	//retrieves a stored texture
	static Texture& GetTexture(const std::string& name);

	//Load / generate shader program 
	static Shader LoadShader(const std::string& vShaderFile, const std::string& fShaderFile,
							 const std::string& tcShaderFile, const std::string& teShaderFile,
							 const std::string& gShaderFile, const std::string& name);

	//retrieves stored shader
	static Shader& GetShader(const std::string& name);

	//Properly de-allocates all loaded resources
	static void Clear();

private:
	Assets(){}

	//Loads a single texture from file
	static Texture LoadTextureFromFile(IRenderer& renderer, const string& filename);

	//load shader from file
	static Shader LoadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile,
							 		 const std::string& tcShaderFile = "", const std::string& teShaderFile = "",
								     const std::string& gShaderFile = "");
};

