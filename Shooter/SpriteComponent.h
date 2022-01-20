#pragma once
#include "Component.h"
#include "Texture.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(Actor* ownerP, Texture& textureP, int drawOrderP = 100);
	virtual ~SpriteComponent();
	SpriteComponent(const SpriteComponent&) = delete;
	SpriteComponent& operator=(const SpriteComponent&) = delete;

	virtual void SetTexture(const Texture& textureP);
	virtual void Draw(Renderer& renderer);

	int GetDrawOrder() const { return drawOrder; }
	int GetTexWidth() const { return texWidth; }
	int getTexHeight() const { return texHeight; }

protected:
	Texture texture;
	int drawOrder;
	int texWidth;
	int texHeight;
};

