#include "BGSpriteComponent.h"
#include "Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* ownerP, const vector<Texture*>& texturesP, int drawOrderP) :
	SpriteComponent(ownerP, *texturesP[0], drawOrderP),
	scrollSpeed{ 0.0f },
	screenSize{ Vector2(WINDOW_WIDTH, WINDOW_HEIGHT) }
{
	SetTextures(texturesP);
}

void BGSpriteComponent::Update(float dt) {
	SpriteComponent::Update(dt);
	for (auto& bg : textures) {
		bg.offset.x += scrollSpeed * dt;
		//si completement en dehors de l'écran, reset offset à la position la plus à droite de l'autre texture
		if (bg.offset.x < -screenSize.x) {
			bg.offset.x = (textures.size() - 1) * (screenSize.x - 1);
		}
	}
}

void BGSpriteComponent::Draw(Renderer& renderer) {
	//draw chaque bg texture
	for (auto& bg : textures) {
		owner.SetPosition(Vector2(bg.offset.x, bg.offset.y));
		renderer.DrawSprite(owner, bg.texture, Rectangle::nullRect, Vector2::zero, Renderer::Flip::None);
	}
}

void BGSpriteComponent::SetTextures(const vector<Texture*>& texturesP) {
	int count = 0;
	for (auto tex : texturesP) {
		BGTexture temp{ *tex, Vector2(count * screenSize.x, 0) }; //chaque texture a screen W en offset
		textures.emplace_back(temp);
		count++;
	}
}
