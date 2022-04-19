#pragma once
#include "IRenderer.h"
#include "VertexArray.h"
#include "Vector2.h"
#include "glew.h"
#include "Shader.h"
#include "Rectangle.h"
#include "MeshComponent.h"

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

	void AddMesh(class MeshComponent* mesh);
	void RemoveMesh(class MeshComponent* mesh);
	void SetViewMatrix(const Matrix4& viewP);

	void Close();
	IRenderer::Type GetType() { return Type::OGL; }

private:
	void DrawMeshes();
	void DrawSprites();

	Window* window;
	VertexArray* spriteVertexArray;
	SDL_GLContext context;
	Matrix4 spriteViewProj;
	Matrix4 view;
	Matrix4 projection;

	std::vector<class MeshComponent*> meshes;
	std::vector<class SpriteComponent*> sprites;
};

