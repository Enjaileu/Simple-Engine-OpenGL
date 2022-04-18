#include "MoveComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "Window.h"

MoveComponent::MoveComponent(Actor* ownerP, int updateOrderP)
	: Component(ownerP, updateOrderP), forwardSpeed{0.f}, angularSpeed{0.f}{}

void MoveComponent::Update(float dt) {
	if (!Maths::nearZero(angularSpeed)) {
		Quaternion newRotation = owner.GetRotation();
		float angle = angularSpeed * dt;
		Quaternion increment(Vector3::unitZ, angle);
		newRotation = Quaternion::Concatenate(newRotation, increment);
		owner.SetRotation(newRotation);
	}
	if (!Maths::nearZero(forwardSpeed)) {
		Vector3 newPosition = owner.GetPosition() + owner.GetForward() * forwardSpeed * dt;
		owner.SetPosition(newPosition);
	}

}
