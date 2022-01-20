#pragma once
#include <string>
#include "Renderer.h"
using std::string;

class Texture
{
public:
	Texture(){}
	~Texture(){}

	void Unload();
	bool Load(Renderer& rendererP, const string& filenameP);
	inline SDL_Texture* toSDLTexture() const { return SDLTexture; }
	void UpdateInfo(int& widthOut, int& heightOut);

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

private:
	string filename{ "" };
	SDL_Texture* SDLTexture{ nullptr };
	int width{ 0 };
	int height{ 0 };
};

