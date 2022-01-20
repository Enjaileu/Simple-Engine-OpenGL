#pragma once
#include "MoveComponent.h"
#include <SDL_stdinc.h>

class InputComponent :  public MoveComponent
{
public:
	InputComponent(Actor* ownerP);
	InputComponent() = delete;
	InputComponent(const InputComponent&) = delete;
	InputComponent& operator=(const InputComponent&) = delete;

	void ProcessInput(const Uint8* keyState);

	void SetMaxForwardSpeed(float value) { maxForwardSpeed = value; }
	void SetMaxAngularSpeed(float value) { maxAngularSpeed = value; }
	void SetForwardKey(int key) { forwardKey = key; }
	void SetBackKey(int key) { backKey = key; }
	void SetClockwiseKey(int key) { clockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { counterClockwiseKey = key; }

private:
	float maxForwardSpeed;
	float maxAngularSpeed;
	int forwardKey;
	int backKey;
	int clockwiseKey;
	int counterClockwiseKey;
};

