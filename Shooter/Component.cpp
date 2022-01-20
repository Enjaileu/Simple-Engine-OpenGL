#include "Component.h"
#include "Actor.h"

Component::Component(Actor* ownerP, int updateOrderP) : owner{*ownerP}, updateOrder{updateOrderP}
{
	owner.AddComponent(this);
}

Component::~Component() {
	owner.RemoveComponent(this);
}

void Component::Update(float dt) {

}

void Component::ProcessInput(const Uint8* keyState) {

}
