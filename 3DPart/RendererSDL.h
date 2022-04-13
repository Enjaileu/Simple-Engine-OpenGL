#pragma once
#include <SDL.h>
#include "IRenderer.h"
#include <vector>
#include "Rectangle.h"

class RendererSDL : public IRenderer 
{
public:
	RendererSDL();
	~RendererSDL(){}
	RendererSDL(const RendererSDL&) = delete;
	RendererSDL& operator= (const RendererSDL&) = delete;

	bool Initialize(Window& window);

	void BeginDraw();
	void Draw(); 
	void EndDraw();
	IRenderer::Type GetType(){ return Type::SDL; }

	void DrawRect(Rectangle& rect);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void DrawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const;

	SDL_Renderer* toSDLRenderer() const { return SDLRenderer; }
	void Close();

private:
	void DrawSprites();
	SDL_Renderer* SDLRenderer{nullptr};
	std::vector<SpriteComponent*> sprites;
};

