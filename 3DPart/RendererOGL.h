#pragma once
#include "IRenderer.h"
#include "VertexArray.h"
#include "Vector2.h"
#include "glew.h"

#include <vector>

class RendererOGL : public IRenderer
{
public:
	RendererOGL();
	virtual ~RendererOGL();
	RendererOGL(const RendererOGL&) = delete;
	RendererOGL& operator=(const RendererOGL&) = delete;

	bool Initialize(Window& window);
	void BeginDraw();
	void Draw();
	void EndDraw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void DrawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const;

	void Close();
	IRenderer::Type GetType() { return Type::OGL; }

private:
	void DrawSprites();

	Window* window;
	VertexArray* vertexArray;
	SDL_GLContext context;
	std::vector<class SpriteComponent*> sprites;
};

