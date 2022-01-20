#pragma once
#include "Window.h"
#include "vector2.h"

class IRenderer
{
public:
	enum class Flip {
		None = SDL_FLIP_NONE,
		Horizontal = SDL_FLIP_HORIZONTAL,
		Vertical = SDL_FLIP_VERTICAL
	};

	enum class Type {SDL, OGL};

	virtual ~IRenderer() {};
	virtual bool Initialize(Window& window) = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;
	virtual void Close() = 0;
	virtual IRenderer::Type GetType() = 0;

	virtual void DrawSprite(const class Actor&, const class Texture& tex, struct Rectangle srcRect, Vector2 origin, Flip flip) const = 0;
};

