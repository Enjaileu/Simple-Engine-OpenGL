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
	static map<string, Texture> textures;
	static map<string, Shader> shaders;

	//Loads a texture from file
	static Texture LoadTexture(IRenderer& renderer, const string& filename, const string& name);

	//retrieves a stored texture
	static Texture& GetTexture(const std::string& name);

	//Properly de-allocates all loaded resources
	static void Clear();

	//loads and generate a shader from file loading (shader's source code)
	static Shader LoadShader(const string& vShaderFile, const string& fShaderFile,
		const string& tcShaderFile, const string& teShaderFile,
		const string& gShaderFile, const string& name);

	//retrieve a stored Shader
	static Shader& GetShader(const string& name);
	

private:
	Assets(){}

	//Loads a single texture from file
	static Texture LoadTextureFromFile(IRenderer& renderer, const string& filename);

	//load and generate a shader from file
	static Shader LoadShaderFromFile(const string& vShaderFile, const string& fShaderFile,
		const string& tcShaderFile = "", const string& teShaderFile = "",
		const string& gShaderFile = "");
};

