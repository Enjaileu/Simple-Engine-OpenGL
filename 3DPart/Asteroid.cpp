#include "Asteroid.h"
#include "Random.h"
#include "Window.h"
#include "Maths.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Assets.h"
#include "Game.h"

Asteroid::Asteroid() : Actor(), collision{ nullptr } {
	Vector2 randPos = Random::GetVector(Vector2::zero, Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.f, Maths::twoPi));

	SpriteComponent* sc = new SpriteComponent(this, Assets::GetTexture("Asteroid"));
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.f);

	collision = new CircleCollisionComponent(this);
	collision->SetRadius(40.f);

	GetGame().AddAsteroid(this);
}

Asteroid::~Asteroid() {
	GetGame().RemoveAsteroid(this);
}

