#pragma once
#include "SpriteComponent.h"
#include <vector>
using std::vector;

class BGSpriteComponent : public SpriteComponent
{
public:
	BGSpriteComponent(Actor* ownerP, const vector<Texture*>& texturesP, int drawOrperP = 10);
	virtual ~BGSpriteComponent(){}
	BGSpriteComponent() = delete;
	BGSpriteComponent(const BGSpriteComponent&) = delete;
	BGSpriteComponent& operator =(const BGSpriteComponent&) = delete;

	float GetScrollSpeed() const { return scrollSpeed; }

	void SetTextures(const vector<Texture*>& textures);
	void SetScreenSize(const Vector2& screenSizeP) { screenSize = screenSizeP; }
	void SetScrollSpeed(float scrollSpeedP) { scrollSpeed = scrollSpeedP; }

	void Update(float dt) override;
	void Draw(Renderer& renderer) override;

private:
	struct BGTexture
	{
		BGTexture(Texture& t, Vector2 o): texture{t}, offset{o} {}
		Texture& texture;
		Vector2 offset;
	};
	std::vector<BGTexture> textures;
	Vector2 screenSize;
	float scrollSpeed;
};

