#include "Game.h"
#include "Actor.h"
#include "Timer.h"
#include "Assets.h"
#include "MeshComponent.h"

bool Game::Initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.Initialize(window);

	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

void Game::Load() {

	Assets::LoadShader("Res\\Shaders\\Sprite.vert", "Res\\Shaders\\Sprite.frag", "", "", "", "Sprite");
	Assets::LoadShader("Res\\Shaders\\BasicMesh.vert", "Res\\Shaders\\BasicMesh.frag", "", "", "", "BasicMesh");

	Assets::LoadTexture(renderer, "Res\\Textures\\Default.png", "Default");
	Assets::LoadTexture(renderer, "Res\\Textures\\Cube.png", "Cube");
	Assets::LoadTexture(renderer, "Res\\Textures\\HealthBar.png", "HealthBar");
	Assets::LoadTexture(renderer, "Res\\Textures\\Plane.png", "Plane");
	Assets::LoadTexture(renderer, "Res\\Textures\\Radar.png", "Radar");
	Assets::LoadTexture(renderer, "Res\\Textures\\Sphere.png", "Sphere");

	Assets::LoadMesh("Res\\Meshes\\Cube.gpmesh", "Mesh_Cube");
	Assets::LoadMesh("Res\\Meshes\\Plane.gpmesh", "Mesh_Plane");
	Assets::LoadMesh("Res\\Meshes\\Sphere.gpmesh", "Mesh_Sphere");

	camera = new Camera();

	Actor* a = new Actor();
	a->SetPosition(Vector3(200.f, 105.f, 0.f));
	a->SetScale(100.f);
	Quaternion q(Vector3::unitY, -Maths::piOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::unitZ, Maths::pi + Maths::pi / 4.f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(Assets::GetMesh("Mesh_Cube"));

	Actor* b = new Actor();
	b->SetPosition(Vector3(200.f, -75.f, 0.f));
	b->SetScale(3.f);
	MeshComponent* mcb = new MeshComponent(b);
	mcb->SetMesh(Assets::GetMesh("Mesh_Sphere"));
}

void Game::ProcessInput()
{
	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}
	// Keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	// Escape: quit game
	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}

	//actor input
	isUpdatingActors = true;
	for (auto actor : actors) {
		actor->ProcessInput(keyboardState);
	}
	isUpdatingActors = false;
}

void Game::Update(float dt)
{
	//Update actors
	isUpdatingActors = true;
	for (auto actor : actors) {
		actor->Update(dt);
	}
	isUpdatingActors = false;
	for (auto actor : pendingActors) {
		actor->ComputeWorldTransform();
		actors.emplace_back(actor);
	}
	pendingActors.clear();

	//Delete dead actors
	vector<Actor*> deadActors;
	for (auto actor : actors) {
		if (actor->GetState() == Actor::ActorState::Dead) {
			deadActors.emplace_back(actor);
		}
	}
	for (auto deadActor : deadActors) {
		delete deadActor;
	}
}

void Game::Render()
{
	renderer.BeginDraw();
	renderer.Draw();
	renderer.EndDraw();
}

void Game::Loop()
{
	Timer timer;
	float dt = 0;
	while (isRunning){
		float dt = timer.computeDeltaTime() / 1000.0f;
		ProcessInput();
		Update(dt);
		Render();
		timer.delayTime(); 
	}
}

void Game::Unload() {
	//delete actors
	//because actor calls removeActor, have to use different style loop
	while (!actors.empty()) {
		delete actors.back();
	}

	//resources
	Assets::Clear();
}

void Game::Close()
{
	renderer.Close();
	window.close();
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	if (isUpdatingActors) {
		pendingActors.emplace_back(actor);
	}
	else {
		actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	//suppr l'actor des 2 vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors)) {
		//swap
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors)) {
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}