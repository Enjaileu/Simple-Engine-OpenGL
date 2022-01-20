#include "Renderer.h"
#include "Log.h"
#include "Texture.h"
#include "Maths.h"
#include "SpriteComponent.h"
#include <SDL_image.h>

Renderer::Renderer() : SDLRenderer(nullptr)
{
}

bool Renderer::Initialize(Window& window)
{
	SDLRenderer = SDL_CreateRenderer(window.getSDLWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!SDLRenderer){
		Log::error(LogCategory::Video, "Failed to create renderer");
		return false;
	}
	if(IMG_Init(IMG_INIT_PNG) == 0){
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;

	}
	return true;
}

void Renderer::BeginDraw()
{
	SDL_SetRenderDrawColor(SDLRenderer, 120, 120, 255, 255);
	SDL_RenderClear(SDLRenderer);
}

void Renderer::EndDraw()
{
	SDL_RenderPresent(SDLRenderer);
}

void Renderer::DrawRect(Rectangle& rect)
{
	SDL_SetRenderDrawColor(SDLRenderer, 255, 255, 255, 255);
	SDL_Rect SDLRect = rect.toSDLRect();
	SDL_RenderFillRect(SDLRenderer, &SDLRect);
}

void Renderer::Close()
{
	SDL_DestroyRenderer(SDLRenderer);
}

void Renderer::Draw() {
	DrawSprites();
}

void Renderer::DrawSprites() {
	for (auto sprite : sprites) {
		sprite->Draw(*this);
	}
}

void Renderer::DrawSprite(const Actor& actor, const Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const {
	SDL_Rect dstRect;
	Vector2 position = actor.GetPosition();
	float rotation = actor.GetRotation();
	float scale = actor.GetScale();
	//Scale the width/height by owner's
	dstRect.w = static_cast<int>(tex.GetWidth() * scale);
	dstRect.h = static_cast<int>(tex.GetHeight() * scale);
	//Center the rectangle around the position of the owner
	dstRect.x = static_cast<int>(position.x - origin.x);
	dstRect.y = static_cast<int>(position.y - origin.y);

	SDL_Rect* srcSDL = nullptr;
	if (srcRect != Rectangle::nullRect) {
		srcSDL = new SDL_Rect{
			Maths::round(srcRect.x),
			Maths::round(srcRect.y),
			Maths::round(srcRect.width),
			Maths::round(srcRect.height)
		};
	}

	SDL_RenderCopyEx(
		SDLRenderer,
		tex.toSDLTexture(),
		srcSDL,
		&dstRect,
		-Maths::ToDegrees(rotation),
		nullptr, //rotation point, center by default
		SDL_FLIP_NONE);
	delete srcSDL;
}

void Renderer::AddSprite(SpriteComponent* sprite) {
	int spriteDrawOrder = sprite->GetDrawOrder();
	auto iter = begin(sprites);
	for (; iter != end(sprites); ++iter) {
		if (spriteDrawOrder < (*iter)->GetDrawOrder()) break;
	}
	sprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	sprites.erase(iter);
}