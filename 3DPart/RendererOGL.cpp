#include "RendererOGL.h"
#include "Assets.h"
#include "Log.h"
#include "SpriteComponent.h"
#include <iostream>

#include <SDL_image.h>

RendererOGL::RendererOGL() :
	window{nullptr},
	vertexArray{nullptr},
	context{nullptr},
	shader(nullptr),
	viewProj(Matrix4::createSimpleViewProj(WINDOW_WIDTH, WINDOW_HEIGHT)){}

RendererOGL::~RendererOGL(){}

bool RendererOGL::Initialize(Window& windowP) {
	window = &windowP;
	
	//Set OpenGl attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	//request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	//enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	context = SDL_GL_CreateContext(windowP.getSDLWindow());
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Log::error(LogCategory::Video, "Failed to initialize GLEW.");
		return false;
	}
	/*
	glewExperimental = GL_TRUE;
	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		Log::error(LogCategory::Video, "Failed to initialize GLEW");
		return false;
	}*/

	//On some platform, GLEW will emit a begin error code so clear it
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}

	vertexArray = new VertexArray(vertices, 4, indices, 6);
	shader = &Assets::GetShader("Basic");
	return true;
}

void RendererOGL::BeginDraw() {
	glClearColor(0.45f, 0.45f, 1.0f, 1.0f);
	//clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	//enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Active shader and vertex array
	shader->use();
	vertexArray->SetActive();
}

void RendererOGL::Draw() {
	DrawSprites();
}

void RendererOGL::DrawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const {
	//Matrix4 scaleMat = Matrix4::createScale((float)tex.getWidth(), (float)tex.getHeight(), 1.0f);
	//Matrix4 world = scaleMat * actor.getWorldTransform();
	//Matrix4 pixelTranslation = Matrix4::createTranslation(Vector3(-WINDOW_WIDTH / 2 - origin.x, -WINDOW_HEIGHT / 2 - origin.y, 0.0f)); // Screen pixel coordinates
	//shader->setMatrix4("uWorldTransform", world * pixelTranslation);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::EndDraw() {
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::Close() {
	SDL_GL_DeleteContext(context);
	delete vertexArray;
}

void RendererOGL::AddSprite(SpriteComponent* sprite)
{
	//insert the sprite at the right place in function of drawOrder
	int spriteDrawOrder = sprite->GetDrawOrder();
	auto iter = begin(sprites);
	for (; iter != end(sprites); iter++) {
		if (spriteDrawOrder < (*iter)->GetDrawOrder()) { break; }
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
	for (auto sprite : sprites) {
		sprite->Draw(*this);
	}
}



