#pragma once
#include <map>
#include <string>
#include "Texture.h"
using std::map;
using std::string;

class Assets
{
public:
	static std::map<std::string, Texture> textures;

	//Loads a texture from file
	static Texture LoadTexture(Renderer& renderer, const string& filename, const string& name);

	//retrieves a stored texture
	static Texture& GetTexture(const std::string& name);

	//Properly de-allocates all loaded resources
	static void Clear();

private:
	Assets(){}

	//Loads a single texture from file
	static Texture LoadTextureFromFile(Renderer& renderer, const string& filename);
};

