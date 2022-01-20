#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"

class Laser : public Actor
{
public:
	Laser();

	void UpdateActor(float dt) override;

private:
	CircleCollisionComponent* collision;
	float deathTimer;
};

