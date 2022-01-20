#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Assets.h"
#include "InputComponent.h"
#include "Maths.h"
#include "Laser.h"
#include "Log.h"

Ship::Ship() : Actor() {

	vector<Texture*> animTextures{
		&Assets::GetTexture("Ship01"),
		&Assets::GetTexture("Ship02"),
		&Assets::GetTexture("Ship03"),
		&Assets::GetTexture("Ship04"),
	};

	AnimSpriteComponent* asc = new AnimSpriteComponent(this, animTextures);
	InputComponent* ic = new InputComponent(this);
	ic->SetMaxAngularSpeed(Maths::twoPi);
	ic->SetMaxForwardSpeed(300.f);
}

void Ship::ActorInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_SPACE] && laserCooldown <= 0.f) {
		Laser* laser = new Laser();
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());
		laserCooldown = 0.2f;
	}
}

void Ship::UpdateActor(float dt) {
	//Log::info("Ship.UdpatdeActor() is running.");
	laserCooldown -= dt;
}