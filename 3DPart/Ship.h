#pragma once
#include "Actor.h"
#include <SDL_stdinc.h>

class Ship : public Actor
{
public:
	Ship();

	void ActorInput(const Uint8* keyState) override;
	void UpdateActor(float dt) override;

private:
	float laserCooldown;
};

