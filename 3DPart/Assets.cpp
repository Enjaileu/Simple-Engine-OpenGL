#include "Assets.h"
#include <iostream>
#include <fstream>
#include "Log.h"
#include <sstream>

std::map<std::string, Texture> Assets::textures; //because it is static, define it
std::map<std::string, Shader> Assets::shaders;

Texture Assets::LoadTexture(IRenderer& renderer, const string& filename, const string& name) {
	textures[name] = LoadTextureFromFile(renderer, filename.c_str());
	return textures[name];
}

Shader Assets::LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& tcShaderFile, const std::string& teShaderFile, const std::string& gShaderFile, const std::string& name)
{
	shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, tcShaderFile, teShaderFile, gShaderFile);
	return shaders[name];
}

Texture& Assets::GetTexture(const string& name) {
	if (textures.find(name) == end(textures)) {
		std::ostringstream loadError;
		loadError << "Texture " << name << " does not exist in assets manager.";
		Log::error(LogCategory::Application, loadError.str());
	}
	return textures[name];
}

Shader& Assets::GetShader(const std::string& name)
{
	if (shaders.find(name) == end(shaders))
	{
		std::ostringstream loadError;
		loadError << "Shader " << name << " does not exist in assets manager.";
		Log::error(LogCategory::Application, loadError.str());
	}
	return shaders[name];
}

void Assets::Clear() {
	for (auto iter : textures) {
		iter.second.Unload();
	}
	textures.clear();
	for (auto iter : shaders) {
		iter.second.Unload();
	}
	shaders.clear();
}

Texture Assets::LoadTextureFromFile(IRenderer& renderer, const string& filename) {
	Texture texture;
	texture.Load(renderer, filename);
	return texture;
}

Shader Assets::LoadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& tcShaderFile, const std::string& teShaderFile, const std::string& gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string tcCode;
    std::string teCode;
    std::string geometryCode;
    try {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If tess control shader path is present, also load a tess control shader
        if (tcShaderFile != "") {
            std::ifstream tessControlShaderFile(tcShaderFile);
            std::stringstream tcShaderStream;
            tcShaderStream << tessControlShaderFile.rdbuf();
            tessControlShaderFile.close();
            tcCode = tcShaderStream.str();
        }
        // If tess evaluation shader path is present, also load a tess evaluation shader
        if (teShaderFile != "") {
            std::ifstream tessEvalShaderFile(teShaderFile);
            std::stringstream teShaderStream;
            teShaderStream << tessEvalShaderFile.rdbuf();
            tessEvalShaderFile.close();
            teCode = teShaderStream.str();
        }
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != "") {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e) {
        std::ostringstream loadError;
        std::string geomShaderFile = "";
        if (gShaderFile != "")
            geomShaderFile = gShaderFile;

        loadError << "ERROR::SHADER: Failed to read shader files " << vShaderFile << " " << fShaderFile << " "
            << geomShaderFile << "\n"
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
        Log::error(LogCategory::Render, loadError.str());
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* tcShaderCode = tcCode.c_str();
    const GLchar* teShaderCode = teCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode,
        tcShaderFile != "" ? tcShaderCode : nullptr,
        teShaderFile != "" ? teShaderCode : nullptr,
        gShaderFile != "" ? gShaderCode : nullptr);
    return shader;
}