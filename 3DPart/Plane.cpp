#include "Plane.h"
#include "MeshComponent.h"
#include "Assets.h"

Plane::Plane() :Actor() {
	SetScale(10.f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(Assets::GetMesh("Mesh_Plane"));
}