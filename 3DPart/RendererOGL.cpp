#include "RendererOGL.h"
#include "Assets.h"
#include "Log.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Rectangle.h"

#include <SDL_image.h>

RendererOGL::RendererOGL() :
	window{nullptr},
	context{nullptr},
	spriteVertexArray{ nullptr },
	spriteViewProj{ Matrix4::createSimpleViewProj(WINDOW_WIDTH, WINDOW_HEIGHT) },
	view{Matrix4::createLookAt(Vector3::zero, Vector3::unitX, Vector3::unitZ)},
	projection{ Matrix4::createPerspectiveFOV(Maths::ToRadians(70.f), WINDOW_WIDTH, WINDOW_HEIGHT, 25.f, 10000.f)}{}

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

	//depth buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

	//On some platform, GLEW will emit a begin error code so clear it
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}

	spriteVertexArray = new VertexArray(spriteVertices, 4, indices, 6);
	return true;
}

void RendererOGL::BeginDraw() {
	glClearColor(0.45f, 0.45f, 1.0f, 1.0f);
	//clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererOGL::Draw() {
	DrawMeshes();
	//DrawSprites();
}

void RendererOGL::EndDraw() {
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::Close() {
	delete spriteVertexArray;
	SDL_GL_DeleteContext(context);
}

void RendererOGL::DrawMeshes() {
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Assets::GetShader("BasicMesh").use();
	// Update view-projection matrix
	Assets::GetShader("BasicMesh").setMatrix4("uViewProj", view * projection);
	for (auto mc : meshes)
	{
		mc->Draw(Assets::GetShader("BasicMesh"));
	}
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
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Active shader and vertex array
	Shader& spriteShader = Assets::GetShader("Sprite");
	spriteShader.use();
	spriteShader.setMatrix4("uViewProj", spriteViewProj);
	spriteVertexArray->SetActive();

	for (auto sprite : sprites) {
		sprite->Draw(*this);
	}
}

void RendererOGL::DrawSprite(const Actor& actor, const class Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const {
	Matrix4 scaleMat = Matrix4::createScale((float)tex.GetWidth(), (float)tex.GetHeight(), 1.0f);
	Matrix4 world = scaleMat * actor.GetWorldTransform();
	Matrix4 pixelTranslation = Matrix4::createTranslation(Vector3(-WINDOW_WIDTH / 2 - origin.x, -WINDOW_HEIGHT / 2 - origin.y, 0.0f)); // Screen pixel coordinates
	Shader& spriteShader = Assets::GetShader("Sprite");
	spriteShader.setMatrix4("uWorldTransform", world * pixelTranslation);
	tex.SetActive();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::AddMesh(MeshComponent* mesh) {
	meshes.emplace_back(mesh);
}

void RendererOGL::RemoveMesh(MeshComponent* mesh) {
	auto iter = std::find(begin(meshes), end(meshes), mesh);
	meshes.erase(iter);
}

void RendererOGL::SetViewMatrix(const Matrix4& viewP) {
	view = viewP;
}


