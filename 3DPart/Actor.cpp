#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "Component.h"
#include "Maths.h"

Actor::Actor() :
	state{ Actor::ActorState::Active },
	position{ Vector3::zero },
	scale{ 1.0f },
	rotation{Quaternion::identity},
	mustRecomputeWorldTransform{true},
	game{ Game::instance() }
{
	game.AddActor(this);
}

Actor::~Actor() {
	game.RemoveActor(this);

	while (!components.empty()) {
		delete components.back();
	}
}

void Actor::SetPosition(Vector3 positionP) { 
	position = positionP; 
	mustRecomputeWorldTransform = true;
}
void Actor::SetScale(float scaleP) { 
	scale = scaleP;
	mustRecomputeWorldTransform = true;
}
void Actor::SetRotation(Quaternion rotationP) { 
	rotation = rotationP;
	mustRecomputeWorldTransform = true;
}

void Actor::SetState(ActorState stateP) {
	state = stateP;
}

Vector3  Actor::GetForward() const {
	return Vector3::transform(Vector3::unitX, rotation);
}

void Actor::ComputeWorldTransform() {
	if (mustRecomputeWorldTransform) {
		mustRecomputeWorldTransform = false;
		worldTransform = Matrix4::createScale(scale);
		worldTransform *= Matrix4::createFromQuaternion(rotation);
		worldTransform *= Matrix4::createTranslation(position);

		for (auto component : components) {
			component->OnUpdateWorldTransform();
		}
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

void Actor::Update(float dt) {
	if (state == Actor::ActorState::Active) {
		ComputeWorldTransform();
		UpdateComponents(dt);
		UpdateActor(dt);
		ComputeWorldTransform();
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



