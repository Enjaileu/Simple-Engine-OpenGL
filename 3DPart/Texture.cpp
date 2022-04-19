#include "Texture.h"
#include "Log.h"
#include <SDL_Image.h>
#include <sstream>

Texture::Texture() : 
	textureID{ 0 },
	filename{ "" },
	width{ 0 },
	height{ 0 },
	SDLTexture{ nullptr }
{
}

Texture::~Texture()
{
}

void Texture::Unload() {
	if (SDLTexture) {
		SDL_DestroyTexture(SDLTexture);
	}
	else {
		glDeleteTextures(1, &textureID);
	}
}

bool Texture::LoadOGL(RendererOGL& renderer, const string& filenameP)
{
	filename = filenameP;
	// Load from file
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + filename);
		return false;
	}
	width = surf->w;
	height = surf->h;

	// Create texture from surface
	int format = 0;
	if (surf->format->format == SDL_PIXELFORMAT_RGB24) {
		format = GL_RGB;
	}
	else if (surf->format->format == SDL_PIXELFORMAT_RGBA32) {
		format = GL_RGBA;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf->pixels);
	
	//SDLTexture = SDL_CreateTextureFromSurface(renderer.toSDLRenderer(), surf);
	SDL_FreeSurface(surf);
	Log::info("Loaded texture " + filename);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return true;
}

void Texture::UpdateInfo(int& widthOut, int& heightOut) {
	widthOut = width;
	heightOut = height;
}

void Texture::SetActive() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}