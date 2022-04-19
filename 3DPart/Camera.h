#pragma once
#include "Actor.h"
class Camera :
    public Actor
{
public:
    Camera();

    void UpdateActor(float dt) override;
    void ActorInput(const Uint8* keys) override;

private:
    class MoveComponent* moveComponent;
};

