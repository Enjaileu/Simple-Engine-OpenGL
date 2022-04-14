#include "Actor.h"
#include <algorithm>
#include "Game.h"
#include "Component.h"
#include "Maths.h"
#include "Log.h"

Actor::Actor() :
	game{ Game::instance() },
	state{ Actor::ActorState::Active },
	position{ Vector2::zero },
	rotation{0.0f},
	mustRecomputeWorldTransfrom{true},
	scale{1.0f}
{
	game.AddActor(this);
}

Actor::~Actor() {
	game.RemoveActor(this);

	while (!components.empty()) {
		delete components.back();
	}
}

Vector2  Actor::GetForward() const {
	//return Vector2(Maths::cos(rotation), -Maths::sin(rotation)); // sin inversé car dans jeu 3D, les axes positifs sont dans des positions negatives
	return Vector2(Maths::cos(rotation), Maths::sin(rotation)); // sin positif pour openGL
}

void Actor::SetPosition(Vector2 positionP) { 
	position = positionP; 
	mustRecomputeWorldTransfrom = true;
}
void Actor::SetScale(float scaleP) { 
	scale = scaleP;
	mustRecomputeWorldTransfrom = true;
}
void Actor::SetRotation(float rotationP) { 
	rotation = rotationP;
	mustRecomputeWorldTransfrom = true;
}
void Actor::SetState(ActorState stateP) { state = stateP; }

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

void Actor::ComputeWorldTransform() {
	if (mustRecomputeWorldTransfrom) {
		mustRecomputeWorldTransfrom = false;
		worldTransform = Matrix4::createScale(scale);
		worldTransform *= Matrix4::createRotationZ(rotation);
		worldTransform *= Matrix4::createTranslation(Vector3(position.x, position.y, 0.0f));

		for(auto component : components){
			component->OnUpdateWorldTransform();
		}
	}
}
