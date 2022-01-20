#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* ownerP, Texture& textureP, int drawOrderP):
	Component{ownerP},
	texture{textureP},
	drawOrder{ drawOrderP },
	texWidth{ textureP.GetWidth() },
	texHeight{ textureP.GetHeight() }
{
	owner.GetGame().GetRenderer().AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	owner.GetGame().GetRenderer().RemoveSprite(this);
}

void SpriteComponent::SetTexture(const Texture& textureP) {
	texture = textureP;
	texture.UpdateInfo(texWidth, texHeight);
}

void SpriteComponent::Draw(Renderer& renderer) {
	Vector2 origin{ texWidth / 2.f, texHeight / 2.f };
	renderer.DrawSprite(owner, texture, Rectangle::nullRect, origin, Renderer::Flip::None);
}
