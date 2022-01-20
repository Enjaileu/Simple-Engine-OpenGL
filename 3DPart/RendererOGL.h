#pragma once
#include "IRenderer.h"
#include "VertexArray.h"
#include "Vector2.h"
#include "Shader.h"

#include <vector>

class RendererOGL : public IRenderer
{
public:
	RendererOGL();
	virtual ~RendererOGL(){}
	RendererOGL(const RendererOGL&) = delete;
	RendererOGL& operator=(const RendererOGL&) = delete;

	bool Initialize(Window& window);
	void BeginDraw();
	void Draw();
	void EndDraw();
	void Close();
	IRenderer::Type GetType() { return Type::OGL; }

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void DrawSprite(const Actor& actor, const class Texture& tex, struct Rectangle srcRect, Vector2 origin, Flip flip) const;

private:
	void DrawSprites();

	Window* window;
	SDL_GLContext context;
	VertexArray* vertexArray;

	std::vector<class SpriteComponent*> sprites;
	Shader* shader;
};