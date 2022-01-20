#include "Laser.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Asteroid.h"
#include "Log.h"

Laser::Laser() :
	Actor(),
	deathTimer{ 1.f }, 
	collision{ nullptr } {

	new SpriteComponent(this, Assets::GetTexture("Laser"));
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(800.f);
	collision = new CircleCollisionComponent(this);
	collision->SetRadius(11.f);
}

void Laser::UpdateActor(float dt) {
	Log::info("Laser.Udpate() si running.");
	deathTimer -= dt;
	if (deathTimer <= 0.f) {
		Log::info("Laser is dead");
		SetState(Actor::ActorState::Dead);
	}
	else {
		auto asteroids = GetGame().GetAsteroids();
		for (auto asteroid : asteroids) {
			if (IsIntersect(*collision, asteroid->GetCollision())) {
				SetState(Actor::ActorState::Dead);
				asteroid->SetState(ActorState::Dead);
				break;
			}
		}
	}
}