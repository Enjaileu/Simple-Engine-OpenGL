#pragma once
#include <SDL_stdinc.h>

class Actor;

class Component
{
public:
	Component(Actor* ownerP, int updateOrderP = 100);
	Component() = delete;
	virtual ~Component();
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	int GetUpdateOrder() const { return updateOrder; }

	virtual void Update(float dt);
	virtual void ProcessInput(const Uint8* keyState);

protected:
	Actor& owner;
	int updateOrder;
};

