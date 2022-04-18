#pragma once
#include <vector>
#include "Window.h"
#include "RendererOGL.h"
#include "Vector2.h"
#include "Actor.h"
#include "SpriteComponent.h"

using std::vector;

class Game
{
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	RendererOGL& GetRenderer() { return renderer; }

private:
	Game(){};

public:
	bool Initialize();
	void Load();
	void Loop();
	void Unload();
	void Close();

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

private:
	void ProcessInput();
	void Update(float dt);
	void Render();

	bool isRunning{ true };
	Window window;
	RendererOGL renderer;

	bool isUpdatingActors {false}; //sert � lock pour empecher d'ajouter des actors quand c'est pas la bonne situation
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

};

