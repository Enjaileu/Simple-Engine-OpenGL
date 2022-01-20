#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* ownerP, const vector<Texture*>& textureP, int drawOrderP):
	SpriteComponent{ownerP, *textureP[0], drawOrderP},
	currentFrame{ 0.f },
	animFPS{ 24.0f }
{
	SetAnimTextures(textureP);
}

void AnimSpriteComponent::SetAnimTextures(const vector<Texture*>& texturesP) {
	animTextures = texturesP;
	if (animTextures.size() > 0) {
		currentFrame = 0.f;
		SetTexture(*animTextures[0]);
	}
}

void AnimSpriteComponent::Update(float dt) {
	SpriteComponent::Update(dt);

	if (animTextures.size() > 0) {
		currentFrame += animFPS * dt;
		while (currentFrame >= animTextures.size()) {
			currentFrame -= animTextures.size();
		}
		SetTexture(*animTextures[static_cast<int>(currentFrame)]);
	}
}
