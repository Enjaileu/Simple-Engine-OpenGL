#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "Component.h"
#include "Maths.h"
#include "Log.h"

Actor::Actor() : game(Game::instance()) {
	game.AddActor(this);
	Log::info("Actor added.");
}

Actor::~Actor() {
	game.RemoveActor(this);

	while (!components.empty()) {
		delete components.back();
	}
}

Vector2  Actor::GetForward() const {
	return Vector2(Maths::cos(rotation), -Maths::sin(rotation)); // sin inversé car dans jeu 3D, les axes positifs sont dans des positions negatives
}

void Actor::SetPosition(Vector2 positionP) { position = positionP; }
void Actor::SetScale(float scaleP) { scale = scaleP; }
void Actor::SetRotation(float rotationP) { rotation = rotationP; }
void Actor::SetState(ActorState stateP) {state = stateP;}

void Actor::Update(float dt) {
	if (state == Actor::ActorState::Active) {
		UpdateComponents(dt);
		UpdateActor(dt);
	}
}
void Actor::UpdateComponents(float dt) {
	for (Component* component : components) {
		component->Update(dt);
	}
}
void Actor::UpdateActor(float dt){}

void Actor::AddComponent(Component* component) {
	//find the insertion point in the sorted vector
	//(the fist element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = begin(components);
	for (; iter != end(components); ++iter) { //; = parce que la 1ere condition est vide
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	//inserts elements before position iterator
	components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
	auto iter = std::find(begin(components), end(components), component);
	if (iter != end(components)) {
		components.erase(iter);
	}
}

void Actor::ProcessInput(const Uint8* keyState) {
	if (state == Actor::ActorState::Active) {
		for (auto component : components) {
			component->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const Uint8* keyState) {

}
