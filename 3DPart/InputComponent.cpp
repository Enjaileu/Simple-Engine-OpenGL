#include "InputComponent.h"
#include <SDL_scancode.h>

InputComponent::InputComponent(Actor* ownerP) :
	MoveComponent{ownerP},
	maxForwardSpeed{100.f},
	maxAngularSpeed{1.f},
	forwardKey{SDL_SCANCODE_W},
	backKey{SDL_SCANCODE_S},
	clockwiseKey{SDL_SCANCODE_D},
	counterClockwiseKey{SDL_SCANCODE_A}
{}

void InputComponent::ProcessInput(const Uint8* keyState) {
	float forwardSpeed = 0.f;
	if (keyState[forwardKey]) {
		forwardSpeed += maxForwardSpeed;
	}
	if (keyState[backKey]) {
		forwardSpeed -= maxForwardSpeed;
	}
	SetForwardSpeed(forwardSpeed);

	float angularSpeed = 0.f;
	if (keyState[clockwiseKey]) {
		angularSpeed -= maxAngularSpeed;
	}
	if (keyState[counterClockwiseKey]) {
		angularSpeed += maxAngularSpeed;
	}
	SetGetAngularSpeed(angularSpeed);
}
