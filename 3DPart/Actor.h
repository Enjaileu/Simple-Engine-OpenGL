#pragma once
#include <vector>
#include "Vector2.h"
#include <SDL_stdinc.h>
#include "Matrix4.h"

using std::vector;

class Game;
class Component;

class Actor
{
public:

	enum class ActorState {
		Active, Paused, Dead
	};

	Actor();
	virtual ~Actor();
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	Game& GetGame() const { return game; }
	const ActorState GetState() const { return state; }
	const Vector3 GetPosition() const { return position; }
	const float GetScale() const { return scale; }
	const Quaternion GetRotation() const { return rotation; }
	const Matrix4& GetWorldTransform() const { return worldTransform; }

	void SetPosition(Vector3 positionP);
	void SetScale(float scaleP);
	void SetRotation(Quaternion rotationP);
	void SetState(ActorState stateP);

	Vector3 GetForward() const;
	void ComputeWorldTransform();


	void ProcessInput(const Uint8* keyState);
	virtual void ActorInput(const Uint8* keyState);
	void Update(float dt);
	void UpdateComponents(float dt);
	virtual void UpdateActor(float dt);
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

private:
	Game& game;
	ActorState state;
	Vector3 position;
	float scale;
	Quaternion rotation;

	Matrix4 worldTransform;
	bool mustRecomputeWorldTransform;

	vector<Component*> components;
};

