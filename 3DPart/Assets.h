#pragma once
#include <map>
#include <string>
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
using std::map;
using std::string;

// A static singleton Assets class that hosts several
// functions to load resources. Each loaded resource is also 
// stored for future reference by string handles. All functions 
// and resources are static and no public constructor is defined.

class Assets
{
public:
	static map<string, Texture> textures;
	static map<string, Shader> shaders;
	static map<string, Mesh> meshes;

	//Loads a texture from file
	static Texture LoadTexture(IRenderer& renderer, const string& filename, const string& name);

	//retrieves a stored texture
	static Texture& GetTexture(const std::string& name);

	//loads and generate a shader from file loading (shader's source code)
	static Shader LoadShader(const string& vShaderFile, const string& fShaderFile,
		const string& tcShaderFile, const string& teShaderFile,
		const string& gShaderFile, const string& name);

	//retrieve a stored Shader
	static Shader& GetShader(const string& name);

	//Load a mesh from file
	static Mesh LoadMesh(const string& filename, const string& name);

	//retrieves a stored mesh
	static Mesh& GetMesh(const string& filename);

	//Properly de-allocates all loaded resources
	static void Clear();
	

private:
	Assets(){}

	//Loads a single texture from file
	static Texture LoadTextureFromFile(IRenderer& renderer, const string& filename);
	static Mesh LoadMeshFromFile(const string& filename);

	//load and generate a shader from file
	static Shader LoadShaderFromFile(const string& vShaderFile, const string& fShaderFile,
		const string& tcShaderFile = "", const string& teShaderFile = "",
		const string& gShaderFile = "");
};

