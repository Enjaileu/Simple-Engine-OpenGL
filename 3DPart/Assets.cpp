#include "Assets.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "Log.h"
#include "RendererOGL.h"
#include <document.h>

map<string, Texture> Assets::textures; //because it is static, define it
map<string, Shader> Assets::shaders;
map<string, Mesh> Assets::meshes;

Texture Assets::LoadTexture(IRenderer& renderer, const string& filename, const string& name) {
    textures[name] = LoadTextureFromFile(renderer, filename.c_str());
    return textures[name];
}

Texture& Assets::GetTexture(const string& name) {
    if (textures.find(name) == end(textures)) {
        std::ostringstream loadError;
        loadError << "Texture " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return textures[name];
}

Shader Assets::LoadShader(const string& vShaderFile, const string& fShaderFile, const string& tcShaderFile, const string& teShaderFile, const string& gShaderFile, const string& name)
{
    shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, tcShaderFile, teShaderFile, gShaderFile);
    return shaders[name];
}

Shader& Assets::GetShader(const string& name)
{
    if (shaders.find(name) == end(shaders)) {
        std::ostringstream loadError;
        loadError << "Shader " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return shaders[name];
}

Mesh Assets::LoadMesh(const string& filename, const string& name) {
    meshes[name] = LoadMeshFromFile(filename);
    return meshes[name];
}

Mesh& Assets::GetMesh(const string& name) {
    if (meshes.find(name) == end(meshes)) {
        std::ostringstream loadError;
        loadError << "Mesh " << name << " does not exist in assets manager";
        Log::error(LogCategory::Application, loadError.str());
    }
    return meshes[name];
}

void Assets::Clear() {
	// delete properly all textures
	for (auto iter : textures) {
		iter.second.Unload();
	}
	textures.clear();
	//same for shader
	for (auto iter : shaders) {
		iter.second.unload();
	}
	shaders.clear();
    // meshes
    for (auto iter : meshes) {
        iter.second.Unload();
    }
    meshes.clear();
}

Texture Assets::LoadTextureFromFile(IRenderer& renderer, const string& filename) {
	Texture texture;
    if (renderer.GetType() == IRenderer::Type::SDL) {
        //texture.LoadSDL(dynamic_cast<RendererSDL&>(renderer), filename);
    }
    else if (renderer.GetType() == IRenderer::Type::OGL) {
        texture.LoadOGL(dynamic_cast<RendererOGL&>(renderer), filename);
    }
    return texture;
}

Shader Assets::LoadShaderFromFile(const string& vShaderFile, const string& fShaderFile, const string& tcShaderFile, const string& teShaderFile, const string& gShaderFile)
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
    shader.compile(vShaderCode, fShaderCode,
        tcShaderFile != "" ? tcShaderCode : nullptr,
        teShaderFile != "" ? teShaderCode : nullptr,
        gShaderFile != "" ? gShaderCode : nullptr);
    return shader;
}

Mesh Assets::LoadMeshFromFile(const string& filename)
{
    Mesh mesh;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        Log::error(LogCategory::Application, "File not found: Mesh " + filename);
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject())
    {
        std::ostringstream s;
        s << "Mesh " << filename << " is not valid json";
        Log::error(LogCategory::Application, s.str());
    }

    mesh.SetShaderName(doc["shader"].GetString());

    // Skip the vertex format/shader for now
    // (This is changed in a later chapter's code)
    size_t vertSize = 8;

    // Load textures
    const rapidjson::Value& textures = doc["textures"];
    if (!textures.IsArray() || textures.Size() < 1)
    {
        std::ostringstream s;
        s << "Mesh " << filename << " has no textures, there should be at least one";
        Log::error(LogCategory::Application, s.str());
    }

    mesh.SetSpecularPower(static_cast<float>(doc["specularPower"].GetDouble()));

    for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
    {
        std::string texName = textures[i].GetString();
        Texture& t = GetTexture(texName);
        mesh.AddTexture(&t);
    }

    // Load in the vertices
    const rapidjson::Value& vertsJson = doc["vertices"];
    if (!vertsJson.IsArray() || vertsJson.Size() < 1)
    {
        std::ostringstream s;
        s << "Mesh " << filename << " has no vertices";
        Log::error(LogCategory::Application, s.str());
    }

    std::vector<float> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);
    float radius = 0.0f;
    for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
    {
        // For now, just assume we have 8 elements
        const rapidjson::Value& vert = vertsJson[i];
        if (!vert.IsArray() || vert.Size() != 8)
        {
            std::ostringstream s;
            s << "Unexpected vertex format for " << filename;
            Log::error(LogCategory::Application, s.str());
        }

        Vector3 pos(static_cast<float>(vert[0].GetDouble()), static_cast<float>(vert[1].GetDouble()), static_cast<float>(vert[2].GetDouble()));
        radius = Maths::max(radius, pos.lengthSq());

        // Add the floats
        for (rapidjson::SizeType i = 0; i < vert.Size(); i++)
        {
            vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
        }
    }

    // We were computing length squared earlier
    mesh.SetRadius(Maths::sqrt(radius));

    // Load in the indices
    const rapidjson::Value& indJson = doc["indices"];
    if (!indJson.IsArray() || indJson.Size() < 1)
    {
        std::ostringstream s;
        s << "Mesh " << filename << " has no indices";
        Log::error(LogCategory::Application, s.str());
    }

    std::vector<unsigned int> indices;
    indices.reserve(indJson.Size() * 3.0);
    for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
    {
        const rapidjson::Value& ind = indJson[i];
        if (!ind.IsArray() || ind.Size() != 3)
        {
            std::ostringstream s;
            s << "Invalid indices for " << filename;
            Log::error(LogCategory::Application, s.str());
        }

        indices.emplace_back(ind[0].GetUint());
        indices.emplace_back(ind[1].GetUint());
        indices.emplace_back(ind[2].GetUint());
    }

    // Now create a vertex array
    mesh.SetVertexArray(new VertexArray(vertices.data(), static_cast<unsigned int>(vertices.size()) / vertSize, indices.data(), static_cast<unsigned int>(indices.size())));

    Log::info("Loaded mesh " + filename);

    return mesh;
}
