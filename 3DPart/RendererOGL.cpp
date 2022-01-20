#include "RendererOGL.h"
#include <glew.h>
#include "Rectangle.h"
#include "Vector2.h"
#include "Log.h"
#include "SpriteComponent.h"
#include "Assets.h"

#include <SDL_image.h>

RendererOGL::RendererOGL() :
	window{ nullptr },
	vertexArray{nullptr},
	context{ nullptr },
	shader{nullptr}
{
}

bool RendererOGL::Initialize(Window& windowP)
{
	window = &windowP;

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	context = SDL_GL_CreateContext(windowP.getSDLWindow());
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Log::error(LogCategory::Video, "Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code, so clear it
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}

	vertexArray = new VertexArray(vertices, 4, indices, 6);
	shader = &Assets::GetShader("Basic");
	return true;
}

void RendererOGL::BeginDraw()
{
	glClearColor(0.45f, 0.45f, 1.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//on active le shader et le vertex array
	shader->Use();
	vertexArray->SetActive();
}

void RendererOGL::Draw()
{
	DrawSprites();
}

void RendererOGL::DrawSprite(const Actor& actor, const Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const
{
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::EndDraw()
{
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::Close()
{
	SDL_GL_DeleteContext(context);
	delete vertexArray;
}

void RendererOGL::AddSprite(SpriteComponent* sprite)
{
	// Insert the sprite at the right place in function of drawOrder
	int spriteDrawOrder = sprite->GetDrawOrder();
	auto iter = begin(sprites);
	for (; iter != end(sprites); ++iter)
	{
		if (spriteDrawOrder < (*iter)->GetDrawOrder()) break;
	}
	sprites.insert(iter, sprite);
}

void RendererOGL::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	sprites.erase(iter);
}

void RendererOGL::DrawSprites()
{
	for (auto sprite : sprites)
	{
		sprite->Draw(*this);
	}
}