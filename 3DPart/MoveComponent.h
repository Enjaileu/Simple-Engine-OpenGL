#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(Actor* ownerP, int updateOrder = 10); //par d?faut, update avant d'autre components
	MoveComponent() = delete;
	MoveComponent(const MoveComponent&) = delete;
	MoveComponent& operator =(const MoveComponent&) = delete;

	float GetForwardSpeed() const { return forwardSpeed; }
	float GetAngularSpeed() const { return angularSpeed; }

	void SetForwardSpeed(float forwardSpeedP) { forwardSpeed = forwardSpeedP; }
	void SetAngularSpeed(float angularSpeedP) { angularSpeed = angularSpeedP; }
	
	void Update(float dt) override;

private:
	float forwardSpeed;
	float angularSpeed;
};

