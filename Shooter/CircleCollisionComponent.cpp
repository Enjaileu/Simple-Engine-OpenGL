#include "CircleCollisionComponent.h"
#include "Actor.h"

CircleCollisionComponent::CircleCollisionComponent(Actor* ownerP): Component{ownerP}, radius{1.f} {}

float CircleCollisionComponent::GetRadius() const {
	return owner.GetScale() * radius;
}

const Vector2 CircleCollisionComponent::GetCenter() const {
	return owner.GetPosition();
}

bool IsIntersect(const CircleCollisionComponent& a, const CircleCollisionComponent& b) {
	Vector2 aCenter = a.GetCenter();
	Vector2 bCenter = b.GetCenter();
	Vector2 ab = bCenter - aCenter;
	float distSq = ab.LengthSq();
	float sumOfRadius = a.GetRadius() + b.GetRadius();
	return distSq <= sumOfRadius * sumOfRadius;
}

