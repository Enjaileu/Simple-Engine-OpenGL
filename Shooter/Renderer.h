#pragma once
#include "Rectangle.h"
#include <SDL.h>
#include "Window.h"
#include "Vector2.h"
#include "Actor.h"

class Renderer
{
public:
	enum class Flip // Wrapper for sdl sprite flipping
	{
		None = SDL_FLIP_NONE,
		Horizontal = SDL_FLIP_HORIZONTAL,
		Vertical = SDL_FLIP_VERTICAL
	};

	Renderer();
	~Renderer(){}
	Renderer(const Renderer&) = delete;
	Renderer& operator= (const Renderer&) = delete;

	bool Initialize(Window& window);

	void BeginDraw();
	void Draw(); 
	void EndDraw();

	void DrawRect(Rectangle& rect);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void DrawSprites();
	void DrawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const;

	SDL_Renderer* toSDLRenderer() const { return SDLRenderer; }
	void Close();

private:
	SDL_Renderer* SDLRenderer = nullptr;
	std::vector<SpriteComponent*> sprites;
};

