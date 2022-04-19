#include "Camera.h"
#include "MoveComponent.h"
#include "Game.h"

Camera::Camera():
	Actor(),
	moveComponent{ nullptr }{
	moveComponent = new MoveComponent(this);
}

void Camera::UpdateActor(float dt) {
	Actor::UpdateActor(dt);

	//Compute new camera from this actor
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100.f;
	Vector3 up = Vector3::unitZ;

	Matrix4 view = Matrix4::createLookAt(cameraPos, target, up);
	GetGame().GetRenderer().SetViewMatrix(view);
}

void Camera::ActorInput(const Uint8* keys) {
	float forwardSpeed = 0.f;
	float angularSpeed = 0.f;
	//wasd movement
	if (keys[SDL_SCANCODE_W]) {
		forwardSpeed += 300.f;
	}
	if (keys[SDL_SCANCODE_S]) {
		forwardSpeed -= 300.f;
	}
	if (keys[SDL_SCANCODE_A]) {
		angularSpeed -= Maths::twoPi;
	}
	if (keys[SDL_SCANCODE_D]) {
		angularSpeed += Maths::twoPi;
	}

	moveComponent->SetForwardSpeed(forwardSpeed);
	moveComponent->SetAngularSpeed(angularSpeed);
}