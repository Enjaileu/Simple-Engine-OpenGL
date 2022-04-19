#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Mesh.h"

MeshComponent::MeshComponent(Actor* owner) :
	Component(owner),
	mesh{ nullptr },
	textureIndex{ 0 }{
	owner->GetGame().GetRenderer().AddMesh(this);
}

MeshComponent::~MeshComponent() {
	owner.GetGame().GetRenderer().RemoveMesh(this);
}

void MeshComponent::Draw(Shader& shader) {
	if (mesh) {
		Matrix4 wt = owner.GetWorldTransform();
		shader.setMatrix4("uWorldTransform", wt);
		shader.setFloat("uSpecPower", mesh->GetSpecularPower());
		Texture* t = mesh->GetTexture(textureIndex);
		if (t) {
			t->SetActive();
		}
		VertexArray* va = mesh->GetVertexArray();
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNbIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void MeshComponent::SetMesh(Mesh& meshP) {
	mesh = &meshP;
}

void MeshComponent::SetTextureIndex(size_t textureIndexP) {
	textureIndex = textureIndexP;
}
