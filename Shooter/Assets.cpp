#include "Assets.h"
#include "Log.h"
#include <sstream>

std::map<std::string, Texture> Assets::textures; //because it is static, define it

Texture Assets::LoadTexture(Renderer& renderer, const string& filename, const string& name) {
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

void Assets::Clear() {
	// delete properly all textures
	//c'est ici que ça plante
	for (auto iter : textures) {
		iter.second.Unload();
	}
	textures.clear();
}

Texture Assets::LoadTextureFromFile(Renderer& renderer, const string& filename) {
	Texture texture;
	texture.Load(renderer, filename);
	return texture;
}