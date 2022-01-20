#pragma once
#include "IRenderer.h"
#include "Rectangle.h"
#include <SDL.h>
#include "Window.h"
#include "Vector2.h"
#include "Actor.h"

class RendererSDL : IRenderer
{
public:
	RendererSDL();
	~RendererSDL() {}
	RendererSDL(const RendererSDL&) = delete;
	RendererSDL& operator= (const RendererSDL&) = delete;

	bool Initialize(Window& window);

	void BeginDraw();
	void Draw();
	void EndDraw();

	IRenderer::Type GetType() { return IRenderer::Type::SDL; }
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

