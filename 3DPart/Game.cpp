#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "Timer.h"
#include "Log.h"
#include "Assets.h"
#include "BGSpriteComponent.h"
#include "Asteroid.h"
#include "Ship.h"

bool Game::Initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.Initialize(window);

	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

void Game::Load() {
	
	//Load Textures
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Ship01.png", "Ship01");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Ship02.png", "Ship02");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Ship03.png", "Ship03");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Ship04.png", "Ship04");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Farback01.png", "Farback01");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Farback02.png", "Farback02");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Stars.png", "Stars");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Asteroid.png", "Asteroid");
	Assets::LoadTexture(renderer, "..\\Ressources\\Shooter_Sprites\\Laser.png", "Laser");

	Assets::LoadShader("Res\\Basic.vert", "Res\\Basic.frag", "", "", "", "Basic");
	Assets::LoadShader("Res\\Transform.vert", "Res\\Basic.frag", "", "", "", "Transform");
	Assets::LoadShader("Res\\Sprite.vert", "Res\\Sprite.frag", "", "", "", "Sprite");

	//Controlled ship
	Ship* ship = new Ship();
	ship->SetPosition(Vector2{ 100, 300 });

	//BG, create the "far back" bg
	vector<Texture*> bgTexsFar{
		&Assets::GetTexture("Farback01"),
		&Assets::GetTexture("Farback02")
	};
	Actor* bgFar = new Actor();
	BGSpriteComponent* bgSpritesFar = new BGSpriteComponent(bgFar, bgTexsFar);
	bgSpritesFar->SetScrollSpeed(-100.0f);

	// and the closest one
	Actor* bgClose = new Actor();
	vector<Texture*> bgTexsClose{
		&Assets::GetTexture("Stars"),
		&Assets::GetTexture("Stars")
	};
	BGSpriteComponent* bgSpritesClose = new BGSpriteComponent(bgClose, bgTexsClose);
	bgSpritesClose->SetScrollSpeed(-200.0f);

	//Asteroids
	const int asteroidNumber = 20;
	for (int i = 0; i < asteroidNumber; ++i) {
		new Asteroid();
	}

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

void Game::AddAsteroid(Asteroid* asteroid) {
	asteroids.emplace_back(asteroid);
}

void Game::RemoveAsteroid(Asteroid* asteroid) {
	auto iter = std::find(begin(asteroids), end(asteroids), asteroid);
	if (iter != asteroids.end()) {
		asteroids.erase(iter);
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