#pragma once
#include "Actor.h"
#include "CircleCollisionComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid();
	~Asteroid();

	CircleCollisionComponent& GetCollision() { return *collision; }

private:
	CircleCollisionComponent* collision;
};

